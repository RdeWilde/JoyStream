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


    void blockHeaderRemove(std::unique_ptr<odb::database> & db, std::shared_ptr<detail::store::BlockHeader> block_header);
    void blockHeaderAdd(std::unique_ptr<odb::database> & db, const ChainHeader & header);
    void transactionMinedInBlockAdd(std::unique_ptr<odb::database> & db,
                                    const Coin::PartialMerkleTree & tree, const Coin::BlockId & blockId);
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

    //don't overwrite existing file
    if(boost::filesystem::exists(file)) {
        std::cerr << "Cannot create database over existing file.\n";
        return false;
    }

    try {
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
    if(_db) _db.reset();
    _seed.clear();
}

Coin::HDKeychain Store::getKeyChain(bool createReceiveAddress) {
    if(!_db) {
        throw std::runtime_error("database not connected");
    }

    odb::transaction t(_db->begin());
    Coin::HDKeychain hdKeyChain(getKeyChain_tx(createReceiveAddress));
    t.commit();

    return hdKeyChain;
}

Coin::PrivateKey Store::getKey(bool createReceiveAddress){
    Coin::HDKeychain keychain(getKeyChain(createReceiveAddress));
    return Coin::PrivateKey(keychain.privkey());
}

std::vector<Coin::HDKeychain> Store::getKeyChains(uint32_t numKeys, bool createReceiveAddress) {
    typedef odb::query<detail::store::key_view_t> query;
    typedef odb::result<detail::store::key_view_t> result;

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
        i.key->used(true);
        _db->update(i.key);
        Coin::PrivateKey sk(hdKeyChain.privkey());
        Coin::PublicKey pk = sk.toPublicKey();
        _publicKeyToIndex[pk] = i.key->id();
        n++;
        if(n == numKeys) break;
    }
    t.commit();

    // if we didn't find enough unused keys in the database, generate the rest
    if(n < numKeys) {
        t.reset(_db->begin());
        for(; n < numKeys; n++) {
            keychains.push_back(getKeyChain_tx(createReceiveAddress));
        }

        t.commit();
    }

    return keychains;
}

std::vector<Coin::PrivateKey> Store::getKeys(uint32_t numKeys, bool createReceiveAddress) {
    std::vector<Coin::HDKeychain> keychains(getKeyChains(numKeys, createReceiveAddress));

    std::vector<Coin::PrivateKey> keys;
    for(auto & keychain : keychains) {
        keys.push_back(Coin::PrivateKey(keychain.privkey()));
    }

    return keys;
}

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

    odb::transaction t(_db->begin());
    detail::store::Key key(index, false);
    _db->update(key);
    t.commit();
}

void Store::releaseKeys(const std::vector<Coin::HDKeychain> keychains) {
    if(!_db) {
        throw std::runtime_error("database not connected");
    }

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
    transactionSave(_db, cointx);
    t.commit();
}

void Store::addTransaction(const Coin::Transaction & cointx, const ChainMerkleBlock & chainmerkleblock) {
    odb::transaction t(_db->begin());

    //get header or create new one
    std::shared_ptr<detail::store::BlockHeader> header;
    try{
        header = _db->load<detail::store::BlockHeader>(chainmerkleblock.hash().getHex());
    } catch( const odb::object_not_persistent &e) {
        header = std::make_shared<detail::store::BlockHeader>(chainmerkleblock);
        _db->persist(header);
    }

    //create new tx or retreive existing one
    std::shared_ptr<detail::store::Transaction> tx = transactionSave(_db, cointx);
    tx->header(header);
    _db->update(tx);

    t.commit();
}

void Store::addBlockHeader(const ChainMerkleBlock & chainmerkleblock) {
    odb::transaction t(_db->begin());
    std::shared_ptr<detail::store::BlockHeader> block(new detail::store::BlockHeader(chainmerkleblock));
    try {
        _db->persist(block);
    } catch (const odb::object_already_persistent &e) {
        // block header already added!
    }
    t.commit();
}

void Store::confirmTransaction(std::string txhash, const ChainMerkleBlock &chainmerkleblock) {

    odb::transaction t(_db->begin());

    //get header or create new one
    std::shared_ptr<detail::store::BlockHeader> header;
    try{
        header = _db->load<detail::store::BlockHeader>(chainmerkleblock.hash().getHex());
    } catch( const odb::object_not_persistent &e) {
        header = std::make_shared<detail::store::BlockHeader>(chainmerkleblock);
        _db->persist(header);
    }

    //retreive existing transaction and update it
    std::shared_ptr<detail::store::Transaction> tx;
    try {
        tx = _db->load<detail::store::Transaction>(txhash);
        tx->header(header);
        _db->update(tx);
        t.commit();
    } catch(const odb::object_not_persistent & e) {}
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

uint64_t Store::getWalletBalance(int32_t confirmations, int32_t main_chain_height) const {
    if(confirmations > 0  && main_chain_height == 0) {
        throw std::runtime_error("getWalletBalance: must provide main_chain_height");
    }

    if(confirmations > 0 && confirmations > main_chain_height) {
        return 0;
    }

    uint64_t balance = 0;
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
        balance += output.value();
    }
    t.commit();
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


        std::shared_ptr<Transaction> tx(new Transaction(coin_tx));

        // Persist new transaction or return existing transaction
        try{
            db->persist(tx);
        } catch (const odb::object_already_persistent &e) {
            return db->load<Transaction>(tx->txid());
        }

        //inputs
        uint32_t i = 0;
        for(const Coin::TxIn &txin : coin_tx.inputs) {

            std::shared_ptr<Input> input(new Input(txin));
            try{
                db->persist(input);
            } catch (const odb::object_already_persistent &e) {
                input = db->load<Input>(input->id());
            }

            std::shared_ptr<TxHasInput> tx_has_input(new TxHasInput(tx, i, input));
            db->persist(tx_has_input);
            i++;
        }

        //outputs
        i = 0;
        for(const Coin::TxOut &txout : coin_tx.outputs) {

            std::shared_ptr<Output> output(new Output(txout));
            try {
                output->address(addressFind(db, output->script()));
                db->persist(output);
            } catch (const odb::object_already_persistent &e) {
                output = db->load<Output>(output->id());
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

} // anonymous namespace (local helper functions)

}//bitcoin
}//joystream
