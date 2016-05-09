#include <bitcoin/Store.hpp>

#include "detail/store/Schema.hpp"
#include "detail/store/Schema-odb.hxx"

#include <odb/database.hxx>
#include <odb/session.hxx>
#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/sqlite/database.hxx>

#include <boost/filesystem.hpp>

#include <QString>

namespace joystream {
namespace bitcoin {

//helper methods forward declaration
namespace {

    std::shared_ptr<detail::store::Address> addressFind(std::unique_ptr<odb::database> & db, const std::string & script);
    std::shared_ptr<detail::store::Transaction> transactionSave(std::unique_ptr<odb::database> & db, const Coin::Transaction & coin_tx);
    std::vector<detail::store::TxHasInput> transactionLoadInputs(std::unique_ptr<odb::database> &db, std::string txid);
    std::vector<detail::store::TxHasOutput> transactionLoadOutputs(std::unique_ptr<odb::database> &db, std::string txid);
    bool transactionLoad(std::unique_ptr<odb::database> &db, std::string txid, Coin::Transaction &coin_tx);
    void transactionDelete(std::unique_ptr<odb::database> &db, std::string txid);
}

// open existing store
// used Store::connected() to check if store was opened successfully
Store::Store(std::string file) {
    open(file);
}

Store::~Store(){
    close();
}

// open existing store, return true if opened successfully
bool Store::open(std::string file) {

    close();

    // attempt to open existing database
    try {
        std::lock_guard<std::mutex> lock(_storeMutex);

        _db = std::unique_ptr<odb::database>(new odb::sqlite::database(file.c_str(), SQLITE_OPEN_READWRITE));

        odb::schema_version v(_db->schema_version());
        odb::schema_version bv(odb::schema_catalog::base_version(*_db));
        odb::schema_version cv(odb::schema_catalog::current_version(*_db));

        // database schema is too old, an cannot be migrated
        // user should either use an older version of joystream
        // or regenerate wallet from seed
        if (v < bv) {
            std::cerr << "Store Error: Database no longer supported\n";
            close();
            return false;
        }

        // database schema is newer than application supports
        // updating version of joystream should work
        if (v > cv) {
            std::cerr << "Store Error: Database is from newer version of joystream\n";
            close();
            return false;
        }

        //migrate the database
        if (v < cv) {
            std::cout << "Store: Migrating database...\n";
            odb::transaction t(_db->begin());
            odb::schema_catalog::migrate(*_db);
            t.commit();

            // run any other custom code following a schema change..
        }

        //load metadata
        odb::transaction t(_db->begin());
        std::shared_ptr<detail::store::Metadata> metadata(_db->query_one<detail::store::Metadata>());
        t.commit();

        _network = metadata->network();
        _seed.clear();
        _seed = Coin::Seed(QString::fromStdString(metadata->seed()));
        _rootKeychain = _seed.generateHDKeychain();
        _timestamp = metadata->created();
        return true;

    } catch (odb::exception &e) {
        // failed to open the database file
        std::cerr << e.what() << std::endl;
        close();
    } catch (std::runtime_error &e) {
        std::cerr << "Store Error reading metadata: " << e.what() << std::endl;
        close();
    }

    return false;
}

bool Store::create(std::string file, Coin::Network network) {
    return create(file, network, Coin::Seed::generate(), std::time(nullptr));
}

bool Store::create(std::string file, Coin::Network network, Coin::Seed seed, uint32_t timestamp) {

    close();

    try {
        std::lock_guard<std::mutex> lock(_storeMutex);

        //don't overwrite existing file
        if(boost::filesystem::exists(file)) {
            std::cerr << "Cannot create database over existing file.\n";
            return false;
        }

        _db = std::unique_ptr<odb::database>(new odb::sqlite::database(file.c_str(), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE));

        // Create the database schema. Due to bugs in SQLite foreign key
        // support for DDL statements, we need to temporarily disable
        // foreign keys.
        odb::connection_ptr c (_db->connection ());

        c->execute ("PRAGMA foreign_keys=OFF");
        odb::transaction t (c->begin ());

        //create database schema
        odb::schema_catalog::create_schema (*_db);

        //initialise metadata
        _network = network;
        _seed = seed;
        _rootKeychain = _seed.generateHDKeychain();
        _timestamp = timestamp;
        detail::store::Metadata metadata(seed.toHex().toStdString(), network, timestamp);
        _timestamp = timestamp;
        _db->persist(metadata);

        t.commit ();
        c->execute ("PRAGMA foreign_keys=ON");
        return true;

    } catch (odb::exception &e) {
        // failed to initialise the database
        std::cerr << "Store::create - failed to create database. " << e.what() << std::endl;
        close();

        // cleanup
        if(boost::filesystem::exists(file)) {
            boost::filesystem::remove(file);
        }
    }

    return false;
}

bool Store::connected() const {
    return _db != nullptr;
}

void Store::close() {
    std::lock_guard<std::mutex> lock(_storeMutex);

    if(_db) _db.reset();
    _seed.clear();
}

// Generates a new key (NOT from key pool) so doesn't require synchronization
Coin::PrivateKey Store::getKey(bool createReceiveAddress) {
    if(!connected()) {
        throw NotConnected();
    }

    odb::transaction t(_db->begin());
    Coin::PrivateKey sk(createNewPrivateKey(createReceiveAddress));
    t.commit();

    return sk;
}


std::vector<Coin::PrivateKey> Store::getKeys(uint32_t numKeys, bool createReceiveAddress) {
    if(!connected()) {
        throw NotConnected();
    }

    typedef odb::query<detail::store::key_view_t> query;
    typedef odb::result<detail::store::key_view_t> result;

    // This routine retrieves unused keys (from the key pool) so requires synchronization
    std::lock_guard<std::mutex> lock(_storeMutex);

    std::vector<Coin::PrivateKey> privKeys;
    result r;

    odb::transaction t(_db->begin());
    if(createReceiveAddress) {
        r = _db->query<detail::store::key_view_t>((query::address::id.is_not_null()
                                    && query::key::used == false) + "LIMIT" + query::_val(numKeys));
    } else {
        r = _db->query<detail::store::key_view_t>((query::address::id.is_null()
                                   && query::key::used == false) + "LIMIT" + query::_val(numKeys));
    }

    uint32_t n = 0;
    for(auto &i : r) {
        privKeys.push_back(i.key->getPrivateKey());
        // removing key from the pool
        i.key->used(true);
        _db->update(i.key);
        n++;
        if(n == numKeys) break;
    }

    // if we didn't find enough unused keys in the database, generate the rest
    if(n < numKeys) {
        for(; n < numKeys; n++) {
            privKeys.push_back(createNewPrivateKey(createReceiveAddress));
        }
    }

    t.commit();

    return privKeys;
}

// Generates new keys (NOT from key pool) so doesn't require synchronization
// Not this routine could be refactored to use getKeyChains() to retreive keys from the 'key pool'
std::vector<Coin::KeyPair> Store::getKeyPairs(uint32_t num_pairs, bool createReceiveAddress) {
    if(!connected()) {
        throw NotConnected();
    }

    std::vector<Coin::KeyPair> keyPairs;

    odb::transaction t(_db->begin());
    for(uint32_t i = 0; i < num_pairs; i++){

        Coin::PrivateKey sk(createNewPrivateKey(createReceiveAddress));
        keyPairs.push_back(Coin::KeyPair(sk));
    }
    t.commit();
    return keyPairs;
}

// Generates a new key so doesn't require synchronization
Coin::P2PKHAddress Store::getReceiveAddress() {
    return getKey(true).toPublicKey().toP2PKHAddress(_network);
}

uint32_t Store::numberOfKeysInWallet() {
    if(!connected()) {
        throw NotConnected();
    }

    odb::transaction t(_db->begin());
    detail::store::key_stat_t stat(_db->query_value<detail::store::key_stat_t>());
    uint32_t count = stat.count;

    return count;
}


void Store::releaseKey(const Coin::PrivateKey &sk) {
    if(!connected()) {
        throw NotConnected();
    }

    std::string raw = sk.toHex().toStdString();

    // Releasing a key requires synchronizing access to the key pool
    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());
    odb::result<detail::store::Key> r(_db->query<detail::store::Key>(odb::query<detail::store::Key>::raw == raw &&
                                                                     odb::query<detail::store::Key>::used == true));
    if(!r.empty()) {
        std::shared_ptr<detail::store::Key> key(r.begin().load());
        key->used(false);
        _db->update(key);
        t.commit();
    }
}

void Store::releaseKeys(const std::vector<Coin::PrivateKey> &privateKeys) {
    if(!connected()) {
        throw NotConnected();
    }

    // Releasing a key requires synchronizing access to the key pool
    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());

    for(auto &sk : privateKeys) {
        std::string raw = sk.toHex().toStdString();
        odb::result<detail::store::Key> r(_db->query<detail::store::Key>(odb::query<detail::store::Key>::raw == raw &&
                                                                         odb::query<detail::store::Key>::used == true));
        if(!r.empty()) {
            std::shared_ptr<detail::store::Key> key(r.begin().load());
            key->used(false);
            _db->update(key);
        }
    }

    t.commit();
}

// this method is slow, better to call releaseKey if possible, because it has to lookup
// the key via the address
void Store::releaseAddress(const Coin::P2PKHAddress & p2pkhaddress) {
    if(!connected()) {
        throw NotConnected();
    }

    std::string base58 = p2pkhaddress.toBase58CheckEncoding().toStdString();
    typedef odb::query<detail::store::Address> query;

    // Releasing a key requires synchronizing access to the key pool
    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());
    std::shared_ptr<detail::store::Address> addr(_db->query_one<detail::store::Address>(query::address == base58));
    if(addr) {
        addr->key()->used(false);
        _db->update(addr->key());
        t.commit();
    }
}

std::list<Coin::P2PKHAddress> Store::listReceiveAddresses() {
    std::list<Coin::P2PKHAddress> addresses;

    for(auto &sk : listPrivateKeys()) {
        addresses.insert(addresses.end(), sk.toPublicKey().toP2PKHAddress(_network));
    }

    return addresses;
}

std::vector<Coin::PrivateKey> Store::listPrivateKeys() {
    if(!connected()) {
        throw NotConnected();
    }

    typedef odb::query<detail::store::key_view_t> query;
    typedef odb::result<detail::store::key_view_t> result;

    std::vector<Coin::PrivateKey> keys;

    odb::transaction t(_db->begin());
    result r(_db->query<detail::store::key_view_t>(query::address::id.is_not_null() && query::key::used == true));
    for(auto &record : r) {
        keys.push_back(record.key->getPrivateKey());
    }

    return keys;
}

std::list<Coin::Transaction> Store::listTransactions() {
    if(!connected()) {
        throw NotConnected();
    }

    std::list<Coin::Transaction> transactions;
    odb::transaction t(_db->begin());
    //get all transactions
    odb::result<detail::store::Transaction> r(_db->query<detail::store::Transaction>());
    for(detail::store::Transaction &tx : r) {
        Coin::Transaction coin_tx;
        try{
            transactionLoad(_db, tx.txid(), coin_tx);
            transactions.insert(transactions.end(), coin_tx);
        } catch (std::exception & e) {
            // Ignore error loading one transaction
        }
    }

    return transactions;
}

bool Store::addressExists(const Coin::P2PKHAddress & addr) {
    if(!connected()) {
        throw NotConnected();
    }

    odb::transaction t(_db->begin());
    typedef odb::query<detail::store::Address> query;
    std::string base58 = addr.toBase58CheckEncoding().toStdString();

    // schema allows only one unique address to be stored so this should never throw an exception
    if(_db->query_one<detail::store::Address>(query::address == base58)) {
        return true;
    }

    return false;
}

bool Store::transactionExists(const Coin::TransactionId & txid) {
    if(!connected()) {
        throw NotConnected();
    }

    odb::transaction t(_db->begin());

    odb::result<detail::store::Transaction> r(_db->query<detail::store::Transaction>(odb::query<detail::store::Transaction>::txid == txid.toHex().toStdString()));
    return !r.empty();
}

void Store::addTransaction(const Coin::Transaction & cointx) {
    if(!connected()) {
        throw NotConnected();
    }

    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());
    auto tx = transactionSave(_db, cointx);
    t.commit();

    Q_ASSERT(tx);

    notifyTxUpdated(Coin::TransactionId::fromTx(cointx), 0);
}

void Store::addTransaction(const Coin::Transaction & cointx, const ChainMerkleBlock & chainmerkleblock) {
    if(!connected()) {
        throw NotConnected();
    }

    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());

    typedef odb::result<detail::store::BlockHeader> result;
    typedef odb::query<detail::store::BlockHeader> query;

    result headers(_db->query<detail::store::BlockHeader>(query::id == chainmerkleblock.hash().getHex()));

    if(!headers.empty()) {
        auto header = headers.begin().load();

        //create new tx or retreive existing one
        std::shared_ptr<detail::store::Transaction> tx = transactionSave(_db, cointx);

        Q_ASSERT(tx);

        tx->header(header);
        _db->update(tx);
        t.commit();

        notifyTxUpdated(Coin::TransactionId::fromTx(cointx), 1);

    } else {
        throw BlockHeaderNotFound();
    }
}

void Store::addBlockHeader(const ChainMerkleBlock & chainmerkleblock) {
    if(!connected()) {
        throw NotConnected();
    }

    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());
    typedef odb::query<detail::store::BlockHeader> header_query;
    typedef odb::result<detail::store::BlockHeader> header_result;
    typedef odb::query<detail::store::Transaction> transaction_query;
    typedef odb::result<detail::store::Transaction> transaction_result;

    transaction_result transactions;
    header_result headers;
    std::vector<Coin::TransactionId> deconfirmedTransactions;

    header_result prevHeaders(_db->query<detail::store::BlockHeader>(header_query::id == chainmerkleblock.prevBlockHash().getHex()));
    if(prevHeaders.empty()) {

        // If the block header does not attach to a prev header in the store
        // we will assume we are building a new chain. So purge all existing headers

        // select all confirmed transactions
        transactions = _db->query<detail::store::Transaction>(transaction_query::header.is_not_null());

        // select all headers
        headers = _db->query<detail::store::BlockHeader>();

    } else {
        // Height of block being inserted should be one greater than the block it is connecting to
        std::shared_ptr<detail::store::BlockHeader> prevBlockHeader(prevHeaders.begin().load());
        if(prevBlockHeader->height() + 1 != chainmerkleblock.height) {
            // block height mismatch
            throw BlockHeaderDoesNotConnect();
        }

        // select all transactions mined in blocks with height equal or greater than chainmerkleblock.height
        transactions = _db->query<detail::store::Transaction>(transaction_query::header.is_not_null() &&
                                                              transaction_query::header->height >= chainmerkleblock.height);

        // select all blocks with height equal or greater than chainmerkleblock.height
        headers = _db->query<detail::store::BlockHeader>(header_query::height >= chainmerkleblock.height);
    }

    // Deconfirm selected transactions
    for(detail::store::Transaction &tx : transactions) {
        tx.header(nullptr);
        _db->update(tx);
        deconfirmedTransactions.push_back(Coin::TransactionId::fromRPCByteOrder(tx.txid()));
    }

    // Delete selected headers
    for(const detail::store::BlockHeader &header : headers) {
        _db->erase(header);
    }

    // Insert new header
    std::shared_ptr<detail::store::BlockHeader> block(new detail::store::BlockHeader(chainmerkleblock));
    _db->persist(block);

    // get transactions in blockheader at height chainmerkleblock.height - 1
    transaction_result twoConfirmations(_db->query<detail::store::Transaction>(transaction_query::header.is_not_null() &&
                                                               transaction_query::header->height == chainmerkleblock.height - 1));

    std::vector<Coin::TransactionId> twoConfirmationTransactions;

    for(auto tx : twoConfirmations) {
        twoConfirmationTransactions.push_back(Coin::TransactionId::fromRPCByteOrder(tx.txid()));
    }

    t.commit();

    for(auto txid : deconfirmedTransactions) {
        notifyTxUpdated(txid, 0);
    }

    for(auto txid : twoConfirmationTransactions) {
        notifyTxUpdated(txid, 2);
    }

}

void Store::confirmTransaction(Coin::TransactionId txid, const ChainMerkleBlock &chainmerkleblock) {
    if(!connected()) {
        throw NotConnected();
    }

    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());

    typedef odb::result<detail::store::BlockHeader> result;
    typedef odb::query<detail::store::BlockHeader> query;

    //get header
    result headers(_db->query<detail::store::BlockHeader>(query::id == chainmerkleblock.hash().getHex()));

    if(!headers.empty()) {
        auto header = headers.begin().load();

        //retreive existing transaction and update it
        std::shared_ptr<detail::store::Transaction> tx;
        odb::result<detail::store::Transaction> transactions(_db->query<detail::store::Transaction>(odb::query<detail::store::Transaction>::txid == txid.toHex().toStdString()));
        if(!transactions.empty()) {
            tx = transactions.begin().load();
            tx->header(header);
            _db->update(tx);
            t.commit();

            notifyTxUpdated(txid, 1);
        }else {
            throw TransactionNotFound();
        }
    } else {
        throw BlockHeaderNotFound();
    }
}

bool Store::loadKey(const Coin::P2PKHAddress & address, Coin::PrivateKey & sk) {
    if(!connected()) {
        throw NotConnected();
    }

    bool found = false;
    typedef odb::query<detail::store::Address> query;
    odb::transaction t(_db->begin());
    std::string base58addr = address.toBase58CheckEncoding().toStdString();
    std::shared_ptr<detail::store::Address> addr(_db->query_one<detail::store::Address>(query::address == base58addr));
    if(addr) {
        sk = addr->key()->getPrivateKey();
        found = true;
    }

    return found;
}

std::list<Coin::UnspentP2PKHOutput>
Store::getUnspentTransactionsOutputs(int32_t confirmations, int32_t main_chain_height) const {
    if(!connected()) {
        throw NotConnected();
    }

    if(confirmations > 0  && main_chain_height == 0) {
        throw std::runtime_error("getUnspentTransactionOutputs: must provide main_chain_height");
    }

    if(confirmations > 0 && confirmations > main_chain_height) {
        return std::list<Coin::UnspentP2PKHOutput>();
    }

    std::list<Coin::UnspentP2PKHOutput> utxos;

    uint32_t maxHeight = main_chain_height - confirmations + 1;
    // a transaction has  main_chain_height - block_height  = confirmations
    //                    main_chain_heigth - confirmations = block_height
    //                                          ^ goes up   -->  ^ goes down
    // for a transaction to have at least N confirmations
    // the block it is mined in must have a height no larger than:  main_chain_height - N = maxHeight
    typedef odb::result<detail::store::outputs_view_t> outputs_r;
    typedef odb::query<detail::store::outputs_view_t> outputs_q;
    odb::session s;
    odb::transaction t(_db->begin());
    outputs_r outputs;
    if(confirmations > 0) {
        outputs = _db->query<detail::store::outputs_view_t>(
                          // we control the address
                          (outputs_q::address::id.is_not_null() &&
                          // output exists
                          outputs_q::output_tx::txid.is_not_null() &&
                          // no tx spends it
                          outputs_q::spending_tx::txid.is_null() && //not considering mined status
                          // output transaction is mined
                          outputs_q::output_block::id.is_not_null() &&
                          // meets minimum confirmations requirement
                          outputs_q::output_block::height <= maxHeight) +
                          "ORDER BY" + outputs_q::Output::id.value + "DESC"
                          );
    } else {
        //Zero Confirmations - by defenition output doesn't have to be mined
        outputs = _db->query<detail::store::outputs_view_t>((outputs_q::address::id.is_not_null() &&
                                                            outputs_q::output_tx::txid.is_not_null() &&
                                                            outputs_q::spending_tx::txid.is_null()) +
                                                            "ORDER BY" + outputs_q::Output::id.value + "DESC");
    }

    for(auto & output : outputs) {

        Coin::PrivateKey sk(_rootKeychain.getChild(output.keyIndex()).privkey());
        Coin::KeyPair keypair(sk);
        Coin::TransactionId txid(Coin::TransactionId::fromRPCByteOrder(uchar_vector(output.txid())));
        Coin::typesafeOutPoint outpoint(txid, output.index());

        utxos.insert(utxos.end(), Coin::UnspentP2PKHOutput(keypair, outpoint, output.value()));
    }

    return utxos;
}

uint64_t Store::getWalletBalance(int32_t confirmations, int32_t main_chain_height) const {
    if(!connected()) {
        throw NotConnected();
    }

    if(confirmations > 0  && main_chain_height == 0) {
        throw std::runtime_error("getWalletBalance: must provide main_chain_height");
    }

    if(confirmations > 0 && confirmations > main_chain_height) {
        return 0;
    }

    uint64_t balance = 0;

    for(Coin::UnspentP2PKHOutput & utxo : getUnspentTransactionsOutputs(confirmations, main_chain_height)) {
        balance += utxo.value();
    }

    return balance;
}

Coin::PrivateKey Store::createNewPrivateKey(bool createReceiveAddress) {
    if(!connected()) {
        throw NotConnected();
    }

    //persist a new key
    std::shared_ptr<detail::store::Key> key(new detail::store::Key());
    uint32_t index = _db->persist(key);

    Coin::HDKeychain hdKeyChain = _rootKeychain.getChild(index);
    Coin::PrivateKey sk(hdKeyChain.privkey());
    key->setPrivateKey(sk);
    _db->update(key);

    if(createReceiveAddress) {
        Coin::P2PKHAddress p2pkh_addr = sk.toPublicKey().toP2PKHAddress(_network);
        detail::store::Address addr(key, p2pkh_addr);
        _db->persist(addr);
    }

    return sk;
}

std::vector<std::string> Store::getLatestBlockHeaderHashes() {
    std::vector<std::string> hashes;

    typedef odb::query<detail::store::BlockHeader> query;
    typedef odb::result<detail::store::BlockHeader> result;

    odb::transaction t(_db->begin());

    result headers(_db->query<detail::store::BlockHeader>("ORDER BY"+ query::height + "DESC LIMIT 50"));
    for(auto &header : headers) {
        hashes.push_back(header.id());
    }

    return hashes;
}

uint32_t Store::getBestHeaderHeight() const {
    typedef odb::query<detail::store::BlockHeader> query;
    typedef odb::result<detail::store::BlockHeader> result;

    odb::transaction t(_db->begin());

    result headers(_db->query<detail::store::BlockHeader>("ORDER BY"+ query::height + "DESC LIMIT 1"));
    if(headers.empty()) {
        return 0;
    }
    std::shared_ptr<detail::store::BlockHeader> best(headers.begin().load());
    return best->height();
}

namespace {
    using namespace detail::store;

    std::shared_ptr<Address> addressFind(std::unique_ptr<odb::database> & db, const std::string & script) {
        typedef odb::query<Address> query;
        std::shared_ptr<Address> addr(db->query_one<Address>(query::scriptPubKey == script));
        return addr;
    }

    //if any input, output scripts or hashes are null transactionSave will throw an exception
    std::shared_ptr<Transaction> transactionSave(std::unique_ptr<odb::database> & db,
                                                 const Coin::Transaction & coin_tx) {

        typedef odb::query<Transaction> tx_query;
        odb::result<Transaction> transactions(db->query<Transaction>(tx_query::txid == coin_tx.hash().getHex()));

        //return existing transaction
        if(!transactions.empty()) {
            return transactions.begin().load();
        }

        // Persist new transaction
        std::shared_ptr<Transaction> tx(new Transaction(coin_tx));
        db->persist(tx);

        typedef odb::query<Input> input_query;

        //inputs
        uint32_t i = 0;
        for(const Coin::TxIn &txin : coin_tx.inputs) {

            std::shared_ptr<Input> input(new Input(txin));
            detail::store::Input_id id = input->id();
            odb::result<Input> inputs(db->query<Input>(
                       input_query::id.op_txid == id.op_txid_ &&
                       input_query::id.op_index == id.op_index_.get() &&
                       input_query::id.scriptSig == id.scriptSig_ &&
                       input_query::id.sequence == id.sequence_.get()));

            if(inputs.empty()) {
                db->persist(input);
            } else {
                input = inputs.begin().load();
            }

            std::shared_ptr<TxHasInput> tx_has_input(new TxHasInput(tx, i, input));
            db->persist(tx_has_input);
            i++;
        }

        typedef odb::query<Output> output_query;

        //outputs
        i = 0;
        for(const Coin::TxOut &txout : coin_tx.outputs) {

            std::shared_ptr<Output> output(new Output(txout));
            Output_id id = output->id();
            odb::result<Output> outputs(db->query<Output>(
                                        output_query::id.scriptPubKey == id.scriptPubKey_ &&
                                        output_query::id.value == id.value_));

            if(outputs.empty()) {
                output->address(addressFind(db, output->script()));
                db->persist(output);
            } else {
                output = outputs.begin().load();
            }

            std::shared_ptr<TxHasOutput> tx_has_output(new TxHasOutput(tx, i, output));
            db->persist(tx_has_output);
            i++;
        }

        return tx;
    }

    std::vector<TxHasInput>
    transactionLoadInputs(std::unique_ptr<odb::database> &db, std::string txid) {
        typedef odb::query<TxHasInput> query;
        typedef odb::result<TxHasInput> result;
        std::vector<TxHasInput> inputs;
        result r(db->query<TxHasInput>((query::tx_ix.tx->txid == txid) + "ORDER BY tx_ix_index ASC"));
        for (auto &input : r) {
            inputs.push_back(input);
        }
        return inputs;
    }

    std::vector<TxHasOutput>
    transactionLoadOutputs(std::unique_ptr<odb::database> &db, std::string txid) {
        typedef odb::query<TxHasOutput> query;
        typedef odb::result<TxHasOutput> result;
        std::vector<TxHasOutput> outputs;
        result r(db->query<TxHasOutput>((query::tx_ix.tx->txid == txid) + "ORDER BY tx_ix_index ASC"));
        for (auto &output : r) {
            outputs.push_back(output);
        }
        return outputs;
    }

    bool transactionLoad(std::unique_ptr<odb::database> &db, std::string txid, Coin::Transaction &coin_tx) {

        std::shared_ptr<Transaction> tx(db->load<Transaction>(txid));

        if(tx == nullptr) {
            return false;
        }

        coin_tx.inputs.clear();
        for(TxHasInput &input : transactionLoadInputs(db, txid)) {
            coin_tx.addInput(input.input()->toCoinCore());
        }

        coin_tx.outputs.clear();
        for(TxHasOutput &output : transactionLoadOutputs(db, txid)) {
            coin_tx.addOutput(output.output()->toCoinCore());
        }

        coin_tx.lockTime = tx->locktime();
        coin_tx.version = tx->version();
        return true;
    }

    void transactionDelete(std::unique_ptr<odb::database> &db, std::string txid) {
        /*
        for(auto &input : transactionLoadInputs(db, txid)) {
            db->erase(input);
        }

        for(auto &output : transactionLoadOutputs(db, txid)) {
            db->erase(output);
        }
        */

        db->erase<Transaction>(txid);
    }

} // anonymous namespace (local helper functions)

}//bitcoin
}//joystream