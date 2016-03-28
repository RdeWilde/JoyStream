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
    std::shared_ptr<detail::store::BlockHeader> insertBlockHeader(std::unique_ptr<odb::database> &db, const ChainMerkleBlock & chainmerkleblock, Store::transactionDeconfirmedCallback callback);
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
            std::cerr << "Error. Database no longer supported\n";
            close();
            return false;
        }

        // database schema is newer than application supports
        // updating version of joystream should work
        if (v > cv) {
            std::cerr << "Error. Database is from newer version of joystream.\n";
            close();
            return false;
        }

        //migrate the database
        if (v < cv) {
            std::cout << "Migrating database...\n";
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
        std::cerr << "error reading metadata. " << e.what() << std::endl;
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
Coin::HDKeychain Store::getKeyChain(bool createReceiveAddress) {
    if(!_db) {
        throw std::runtime_error("database not connected");
    }

    odb::transaction t(_db->begin());
    Coin::HDKeychain hdKeyChain(getKeyChain_tx(createReceiveAddress));
    t.commit();

    return hdKeyChain;
}

// Generates a new key (NOT from key pool) so doesn't require synchronization
Coin::PrivateKey Store::getKey(bool createReceiveAddress){
    Coin::HDKeychain keychain(getKeyChain(createReceiveAddress));
    return Coin::PrivateKey(keychain.privkey());
}


std::vector<Coin::HDKeychain> Store::getKeyChains(uint32_t numKeys, bool createReceiveAddress) {
    typedef odb::query<detail::store::key_view_t> query;
    typedef odb::result<detail::store::key_view_t> result;

    // This routine retrieves unused keys (from the key pool) so requires synchronization
    std::lock_guard<std::mutex> lock(_storeMutex);

    std::vector<Coin::HDKeychain> keychains;
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
        Coin::HDKeychain hdKeyChain(_rootKeychain.getChild(i.key->id()));
        keychains.push_back(hdKeyChain);
        // removing key from the pool
        i.key->used(true);
        _db->update(i.key);
        Coin::PrivateKey sk(hdKeyChain.privkey());
        Coin::PublicKey pk = sk.toPublicKey();
        _publicKeyToIndex[pk] = i.key->id();
        n++;
        if(n == numKeys) break;
    }

    // if we didn't find enough unused keys in the database, generate the rest
    if(n < numKeys) {
        for(; n < numKeys; n++) {
            keychains.push_back(getKeyChain_tx(createReceiveAddress));
        }
    }

    t.commit();

    return keychains;
}

std::vector<Coin::PrivateKey> Store::getKeys(uint32_t numKeys, bool createReceiveAddress) {
    // store access synchronization is done in getKeyChains()
    std::vector<Coin::HDKeychain> keychains(getKeyChains(numKeys, createReceiveAddress));

    std::vector<Coin::PrivateKey> keys;
    for(auto & keychain : keychains) {
        keys.push_back(Coin::PrivateKey(keychain.privkey()));
    }

    return keys;
}

// Generates new keys (NOT from key pool) so doesn't require synchronization
// Not this routine could be refactored to use getKeyChains() to retreive keys from the 'key pool'
std::vector<Coin::KeyPair> Store::getKeyPairs(uint32_t num_pairs, bool createReceiveAddress) {
    if(!_db) {
        throw std::runtime_error("database not connected");
    }

    std::vector<Coin::KeyPair> key_pairs;

    odb::transaction t(_db->begin());
    for(uint32_t i = 0; i < num_pairs; i++){

        Coin::PrivateKey sk(getKeyChain_tx(createReceiveAddress).privkey());
        Coin::PublicKey pk = sk.toPublicKey();
        Coin::KeyPair pair(pk, sk);
        key_pairs.push_back(pair);
    }
    t.commit();
    return key_pairs;
}

// Generates a new key so doesn't require synchronization
Coin::P2PKHAddress Store::getReceiveAddress() {
    Coin::PrivateKey sk(getKeyChain(true).privkey());
    return sk.toPublicKey().toP2PKHAddress(_network);
}

uint32_t Store::numberOfKeysInWallet() {
    odb::transaction t(_db->begin());
    detail::store::key_stat_t stat(_db->query_value<detail::store::key_stat_t>());
    uint32_t count = stat.count;
    t.commit();
    return count;
}

void Store::releaseKey(const Coin::HDKeychain & hdKeyChain) {
    if(!_db) {
        throw std::runtime_error("database not connected");
    }

    //verify this key is a child of the rootKeychain
    if(hdKeyChain.parent_fp() != _rootKeychain.fp()) {
        throw std::runtime_error("attemp to release key not dervied from root keychain");
    }

    // Releasing a key requires synchronizing access to the key pool
    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());
    detail::store::Key key(hdKeyChain.child_num(), false);
    _db->update(key);
    t.commit();
}

void Store::releaseKey(const Coin::PrivateKey &sk) {
    Coin::PublicKey pk = sk.toPublicKey();

    if(_publicKeyToIndex.find(pk) != _publicKeyToIndex.end()) {
        releaseKey(_publicKeyToIndex[pk]);
        _publicKeyToIndex.erase(pk);
    }else {
        throw std::runtime_error("attempting to release a private key not in the pool");
    }
}

void Store::releaseKey(uint32_t index) {
    if(!_db) {
        throw std::runtime_error("database not connected");
    }

    // Releasing a key requires synchronizing access to the key pool
    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());
    detail::store::Key key(index, false);
    _db->update(key);
    t.commit();
}

void Store::releaseKeys(const std::vector<Coin::HDKeychain> keychains) {
    if(!_db) {
        throw std::runtime_error("database not connected");
    }

    // Releasing a key requires synchronizing access to the key pool
    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());
    for(auto &hdKeyChain : keychains) {
        //verify this key is a child of the rootKeychain
        if(hdKeyChain.parent_fp() != _rootKeychain.fp()) {
            throw std::runtime_error("attemp to release key not dervied from root keychain");
        }
        detail::store::Key key(hdKeyChain.child_num(), false);
        _db->update(key);
    }
    t.commit();
}

// this method is slow, better to call releaseKey if possible, because it has to lookup
// the key via the address
void Store::releaseAddress(const Coin::P2PKHAddress & p2pkhaddress) {
    if(!_db) {
        throw std::runtime_error("database not connected");
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
    }
    t.commit();
}

std::list<Coin::P2PKHAddress> Store::listReceiveAddresses() {
    std::list<Coin::P2PKHAddress> addresses;

    for(auto &sk : listPrivateKeys()) {
        addresses.insert(addresses.begin(), sk.toPublicKey().toP2PKHAddress(_network));
    }

    return addresses;
}

std::list<Coin::PrivateKey> Store::listPrivateKeys() {
    typedef odb::query<detail::store::key_view_t> query;
    typedef odb::result<detail::store::key_view_t> result;

    std::list<Coin::PrivateKey> keys;

    odb::transaction t(_db->begin());
    result r(_db->query<detail::store::key_view_t>(query::address::id.is_not_null() && query::key::used == true));
    for(auto &record : r) {
        Coin::PrivateKey sk(_rootKeychain.getChild(record.key->id()).privkey());
        keys.insert(keys.begin(), sk);
    }
    t.commit();
    return keys;
}

std::list<Coin::Transaction> Store::listTransactions() {
    std::list<Coin::Transaction> transactions;
    odb::transaction t(_db->begin());
    //get all transactions
    odb::result<detail::store::Transaction> r(_db->query<detail::store::Transaction>());
    for(detail::store::Transaction &tx : r) {
        Coin::Transaction coin_tx;
        transactionLoad(_db, tx.txid(), coin_tx);
        transactions.insert(transactions.begin(), coin_tx);
    }
    t.commit();
    return transactions;
}

bool Store::addressExists(const Coin::P2PKHAddress & addr) {
    typedef odb::query<detail::store::Address> query;
    std::string base58 = addr.toBase58CheckEncoding().toStdString();
    if(_db->query_one<detail::store::Address>(query::address == base58)) return true;
    return false;
}

bool Store::transactionExists(const Coin::TransactionId & txid) {
    try{
        _db->load<detail::store::Transaction>(txid.toHex().toStdString());
        return true;
    } catch(const odb::object_not_persistent &e) {
        return false;
    }
}

bool Store::transactionExists(const Coin::Transaction & tx) {
    Coin::Transaction transaction;
    if(transactionLoad(_db, Coin::TransactionId::fromTx(tx).toHex().toStdString(), transaction)) {

        return (tx.inputs.size() == transaction.inputs.size()
                && tx.outputs.size() == transaction.outputs.size()
                && tx.version == transaction.version
                && tx.lockTime == transaction.lockTime);
    }
    return false;
}

void Store::addTransaction(const Coin::Transaction & cointx) {
    odb::transaction t(_db->begin());
    auto tx = transactionSave(_db, cointx);
    t.commit();
}

void Store::addTransaction(const Coin::Transaction & cointx, const ChainMerkleBlock & chainmerkleblock, bool createHeader, transactionDeconfirmedCallback callback) {
    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());

    //get header or create new one
    std::shared_ptr<detail::store::BlockHeader> header;
    if(createHeader) {
        // exception will be thrown if header is not inserted
        header = insertBlockHeader(_db, chainmerkleblock, callback);
    } else {
        // exception will be thrown if header not found in the store
        header = _db->load<detail::store::BlockHeader>(chainmerkleblock.hash().getHex());
    }

    //create new tx or retreive existing one
    std::shared_ptr<detail::store::Transaction> tx = transactionSave(_db, cointx);
    tx->header(header);
    _db->update(tx);

    t.commit();
}

void Store::addBlockHeader(const ChainMerkleBlock & chainmerkleblock, transactionDeconfirmedCallback callback) {
    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());

    // exception will be thrown if trying to insert a header out of order
    insertBlockHeader(_db, chainmerkleblock, callback);

    t.commit();
}

void Store::confirmTransaction(std::string txhash, const ChainMerkleBlock &chainmerkleblock, bool createHeader, transactionDeconfirmedCallback callback) {
    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());

    //get header or create new one
    std::shared_ptr<detail::store::BlockHeader> header;
    if(createHeader) {
        // exception will be thrown if header not inserted
        header = insertBlockHeader(_db, chainmerkleblock, callback);
    } else {
        // exception will be thrown if header not found in the store
        header = _db->load<detail::store::BlockHeader>(chainmerkleblock.hash().getHex());
    }

    //retreive existing transaction and update it
    std::shared_ptr<detail::store::Transaction> tx;
    odb::result<detail::store::Transaction> transactions(_db->query<detail::store::Transaction>(odb::query<detail::store::Transaction>::txid == txhash));
    if(!transactions.empty()) {
        tx = transactions.begin().load();
        tx->header(header);
        _db->update(tx);
    }

    t.commit();
}

bool Store::loadKey(const Coin::P2PKHAddress & address, Coin::PrivateKey & sk) {
    bool found = false;
    typedef odb::query<detail::store::Address> query;
    odb::transaction t(_db->begin());
    std::string base58addr = address.toBase58CheckEncoding().toStdString();
    std::shared_ptr<detail::store::Address> addr(_db->query_one<detail::store::Address>(query::address == base58addr));
    if(addr) {
        sk = Coin::PrivateKey(_rootKeychain.getChild(addr->key()->id()).privkey());
        found = true;
    }
    t.commit();
    return found;
}

std::list<Coin::UnspentP2PKHOutput>
Store::getUnspentTransactionsOutputs(int32_t confirmations, int32_t main_chain_height) const {
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
                          outputs_q::address::id.is_not_null() &&
                          // output exists
                          outputs_q::output_tx::txid.is_not_null() &&
                          // no tx spends it
                          outputs_q::spending_tx::txid.is_null() && //not considering mined status
                          // output transaction is mined
                          outputs_q::output_block::id.is_not_null() &&
                           // meets minimum confirmations requirement
                          outputs_q::output_block::height <= maxHeight
                          );
    } else {
        //Zero Confirmations - by defenition output doesn't have to be mined
        outputs = _db->query<detail::store::outputs_view_t>(outputs_q::address::id.is_not_null() &&
                                                            outputs_q::output_tx::txid.is_not_null() &&
                                                            outputs_q::spending_tx::txid.is_null());
    }

    for(auto & output : outputs) {

        Coin::PrivateKey sk(_rootKeychain.getChild(output.keyIndex()).privkey());
        Coin::KeyPair keypair(sk);
        Coin::TransactionId txid(Coin::TransactionId::fromRPCByteOrder(uchar_vector(output.txid())));
        Coin::typesafeOutPoint outpoint(txid, output.index());

        utxos.insert(utxos.begin(), Coin::UnspentP2PKHOutput(keypair, outpoint, output.value()));
    }

    t.commit();

    return utxos;
}

uint64_t Store::getWalletBalance(int32_t confirmations, int32_t main_chain_height) const {
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

Coin::HDKeychain Store::getKeyChain_tx(bool createReceiveAddress) {
    //persist a new key
    std::shared_ptr<detail::store::Key> key(new detail::store::Key());
    uint32_t index = _db->persist(key);

    Coin::HDKeychain hdKeyChain = _rootKeychain.getChild(index);

    Coin::PrivateKey sk(hdKeyChain.privkey());
    Coin::PublicKey pk = sk.toPublicKey();

    _publicKeyToIndex[pk] = index;

    if(createReceiveAddress) {
        Coin::P2PKHAddress p2pkh_addr = pk.toP2PKHAddress(_network);
        detail::store::Address addr(key, p2pkh_addr);
        _db->persist(addr);
    }

    return hdKeyChain;
}

std::vector<std::string> Store::getLatestBlockHeaderHashes() {
    std::vector<std::string> hashes;

    typedef odb::query<detail::store::BlockHeader> query;
    typedef odb::result<detail::store::BlockHeader> result;

    odb::transaction t(_db->begin());

    try {
        result headers(_db->query<detail::store::BlockHeader>("ORDER BY"+ query::height + "DESC LIMIT 10"));
        for(auto &header : headers) {
            hashes.push_back(header.id());
        }
        t.commit();
    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
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
                                        output_query::id.value == id.value_.get()));

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

        for(auto &input : transactionLoadInputs(db, txid)) {
            db->erase(input);
        }

        for(auto &output : transactionLoadOutputs(db, txid)) {
            db->erase(output);
        }

        db->erase<Transaction>(txid);
    }

    std::shared_ptr<BlockHeader> insertBlockHeader(std::unique_ptr<odb::database> &db, const ChainMerkleBlock & chainmerkleblock, Store::transactionDeconfirmedCallback callback) {
        typedef odb::query<BlockHeader> header_query;
        typedef odb::result<BlockHeader> header_result;
        typedef odb::query<Transaction> transaction_query;
        typedef odb::result<Transaction> transaction_result;

        // Only add block header if it attaches to previous block header in our store ...
        header_result prevHeaders(db->query<BlockHeader>(header_query::id == chainmerkleblock.prevBlockHash().getHex()));
        if(prevHeaders.empty()) {
            // .. unless its the first block header in the store
            prevHeaders = db->query<BlockHeader>((header_query::height < chainmerkleblock.height) + "LIMIT 1");
            if(!prevHeaders.empty()) {
                throw std::runtime_error("block header doesn't connect to stored block header chain");
            }
        } else {
            // Height of block being inserted should be one greater than the block it is connecting to
            std::shared_ptr<BlockHeader> prevBlockHeader(prevHeaders.begin().load());
            if(prevBlockHeader->height() + 1 != chainmerkleblock.height) {
                throw std::runtime_error("block header doesn't connect to stored block header chain");
            }
        }

        // find all transactions mined in blocks with height equal or greater than chainmerkleblock.height
        transaction_result transactions(db->query<Transaction>(transaction_query::header.is_not_null() &&
                                                               transaction_query::header->height >= chainmerkleblock.height));

        for(Transaction &tx : transactions) {            
            tx.header(nullptr);
            db->update(tx);
            // add deconfirmed transaction to mempool
            callback(tx.txid());
        }

        // delete all blocks with height equal or greater than chainmerkleblock.height
        header_result headers(db->query<BlockHeader>(header_query::height >= chainmerkleblock.height));
        for(const BlockHeader &header : headers) {
            db->erase(header);
        }

        std::shared_ptr<detail::store::BlockHeader> block(new detail::store::BlockHeader(chainmerkleblock));

        db->persist(block);

        return block;

    }
} // anonymous namespace (local helper functions)

}//bitcoin
}//joystream
