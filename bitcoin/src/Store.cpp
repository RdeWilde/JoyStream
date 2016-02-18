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
    std::string transactionSave(std::unique_ptr<odb::database> & db, const Coin::Transaction & coin_tx);
    std::vector<detail::store::TxHasInput> transactionLoadInputs(std::unique_ptr<odb::database> &db, std::string txid);
    std::vector<detail::store::TxHasOutput> transactionLoadOutputs(std::unique_ptr<odb::database> &db, std::string txid);
    bool transactionLoad(std::unique_ptr<odb::database> &db, std::string txid, Coin::Transaction &coin_tx);
    void transactionDelete(std::unique_ptr<odb::database> &db, std::string txid);

#ifdef USE_STORE_ALPHA_CODE
    void blockHeaderRemove(std::unique_ptr<odb::database> & db, std::shared_ptr<detail::store::BlockHeader> block_header);
    void blockHeaderAdd(std::unique_ptr<odb::database> & db, const ChainHeader & header);
    void transactionMinedInBlockAdd(std::unique_ptr<odb::database> & db,
                                    const Coin::PartialMerkleTree & tree, const Coin::BlockId & blockId);
    uint64_t getWalletBalance(std::unique_ptr<odb::database> & db, uint32_t confirmations, uint32_t main_chain_height);
#endif
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
        std::cerr << "error trying to create database over existing file.\n";
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
        detail::store::Metadata metadata(seed.toHex().toStdString(), network, timestamp);
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

Coin::HDKeychain Store::getKey(bool createReceiveAddress) {
    if(!_db) {
        throw std::runtime_error("database not connected");
    }

    odb::transaction t(_db->begin());
    Coin::HDKeychain hdKeyChain(getKey_tx(createReceiveAddress));
    t.commit();

    return hdKeyChain;
}

std::vector<Coin::HDKeychain> Store::getKeys(uint32_t numKeys, bool createReceiveAddress) {
    typedef odb::query<detail::store::key_view_t> query;
    typedef odb::result<detail::store::key_view_t> result;

    std::vector<Coin::HDKeychain> keys;
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
        keys.push_back(_rootKeychain.getChild(i.key->id()));
        i.key->used(true);
        _db->update(i.key);
        n++;
        if(n == numKeys) break;
    }
    t.commit();

    // if we didn't find enough unused keys in the database, generate the rest
    if(n < numKeys) {
        t.reset(_db->begin());
        for(; n < numKeys; n++) {
            keys.push_back(getKey_tx(createReceiveAddress));
        }

        t.commit();
    }

    return keys;
}

// since we are not returning an HDKeychain only way to release keys
// is to release the p2pkh address, try to avoid using this method
std::vector<Coin::KeyPair> Store::getKeyPairs(uint32_t num_pairs, bool createReceiveAddress) {
    if(!_db) {
        throw std::runtime_error("database not connected");
    }

    std::vector<Coin::KeyPair> key_pairs;

    odb::transaction t(_db->begin());
    for(uint32_t i = 0; i < num_pairs; i++){

        Coin::PrivateKey sk(getKey_tx(createReceiveAddress).privkey());
        Coin::PublicKey pk = sk.toPublicKey();
        Coin::KeyPair pair(pk, sk);
        key_pairs.push_back(pair);
    }
    t.commit();
    return key_pairs;
}

Coin::P2PKHAddress Store::getReceiveAddress() {
    Coin::PrivateKey sk(getKey(true).privkey());
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
    typedef odb::query<detail::store::key_view_t> query;
    typedef odb::result<detail::store::key_view_t> result;

    odb::transaction t(_db->begin());
    result r(_db->query<detail::store::key_view_t>(query::address::id.is_not_null() && query::key::used == true));
    for(auto &record : r) {
        Coin::PrivateKey sk(_rootKeychain.getChild(record.key->id()).privkey());
        addresses.insert(addresses.begin(), sk.toPublicKey().toP2PKHAddress(_network));
    }
    t.commit();
    return addresses;
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
    if(_db->load<detail::store::Transaction>(txid.toHex().toStdString())) return true;
    return false;
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

void Store::addTransaction(const Coin::Transaction & tx) {
    odb::transaction t(_db->begin());
    transactionSave(_db, tx);
    t.commit();
}

bool Store::loadKey(const std::string & address, Coin::PrivateKey & sk) {
    bool found = false;
    typedef odb::query<detail::store::Address> query;
    odb::transaction t(_db->begin());
    std::shared_ptr<detail::store::Address> addr(_db->query_one<detail::store::Address>(query::address == address));
    if(addr) {
        sk = Coin::PrivateKey(_rootKeychain.getChild(addr->key()->id()).privkey());
        found = true;
    }
    t.commit();
    return found;
}

Coin::HDKeychain Store::getKey_tx(bool createReceiveAddress) {
    //persist a new key
    std::shared_ptr<detail::store::Key> key(new detail::store::Key());
    uint32_t index = _db->persist(key);

    Coin::HDKeychain hdKeyChain = _rootKeychain.getChild(index);

    if(createReceiveAddress) {
        Coin::PrivateKey sk(hdKeyChain.privkey());
        Coin::P2PKHAddress p2pkh_addr = sk.toPublicKey().toP2PKHAddress(_network);
        detail::store::Address addr(key, p2pkh_addr);
        _db->persist(addr);
    }

    return hdKeyChain;
}

namespace {
    using namespace detail::store;

    std::shared_ptr<Address> addressFind(std::unique_ptr<odb::database> & db, const std::string & script) {
        typedef odb::query<Address> query;
        std::shared_ptr<Address> addr(db->query_one<Address>(query::scriptPubKey == script));
        return addr;
    }

    //if any input, output scripts or hashes are null transactionSave will throw an exception
    std::string transactionSave(std::unique_ptr<odb::database> & db,
                                const Coin::Transaction & coin_tx) {
        //TODO: multiple checks before adding transaction
        //sanity check (valid transaction) - do not trust network
        //look for duplicate txid (or duplicate outpoints [as better way to track transaction due to malleability)
        //ref: https://bitcoin.org/en/developer-guide#transaction-malleability
        //double spend..

        std::shared_ptr<Transaction> tx(new Transaction(coin_tx));
        db->persist(tx);

        //inputs
        uint32_t i = 0;
        for(const Coin::TxIn &txin : coin_tx.inputs) {

            std::shared_ptr<Input> input(new Input(txin));
            try{
                db->persist(input);
            } catch (const odb::object_already_persistent &e) {
                std::cout << "object already persistent\n";
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

        return tx->txid();
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

#ifdef USE_STORE_ALPHA_CODE
    uint64_t getWalletBalance(std::unique_ptr<odb::database> & db, uint32_t confirmations, uint32_t main_chain_height) {
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
        odb::transaction t(db->begin());
        outputs_r outputs;
        if(confirmations > 0) {
            outputs = db->query<detail::store::outputs_view_t>(
                              // output exists
                              outputs_q::output_tx::txid.is_not_null() &&
                              // no tx spends it
                              outputs_q::spending_tx::txid.is_null() && //not considering mined status
                              // output transaction is mined
                              outputs_q::output_block::id.is_not_null() &&
                              // and is on the main chain
                              outputs_q::output_block::isOnMainChain == true && //assuming only one block = on main chain with tx
                               // meets minimum confirmations requirement
                              outputs_q::output_block::height <= maxHeight
                              );
        } else {
            //Zero Confirmations - by defenition output doesn't have to be mined
            outputs = db->query<detail::store::outputs_view_t>(outputs_q::output_tx::txid.is_not_null() &&
                                                               outputs_q::spending_tx::txid.is_null());
            //make sure to remove tx that never get mined after certain period of time
        }

        for(auto & output : outputs) {
            std::cout << "wallet utxo: " << output.value() << std::endl;
            balance += output.value();
        }
        t.commit();
        return balance;
    }

    void blockHeaderRemove(std::unique_ptr<odb::database> & db, std::shared_ptr<BlockHeader> block_header) {
        odb::result<TransactionMinedInBlock> mined(
            db->query<TransactionMinedInBlock>(odb::query<TransactionMinedInBlock>::tx_block.block == block_header->id())
        );

        for(auto &i : mined) {
            db->erase<TransactionMinedInBlock>(i);
        }

        db->erase(block_header);
    }

    void blockHeaderAdd(std::unique_ptr<odb::database> & db, const ChainHeader & header) {
        std::shared_ptr<BlockHeader> block_header(db->load<BlockHeader>(header.hash().getHex()));
        if(block_header) {
            //found existing
            if(!header.inBestChain) {
                //WILL SPV client get us here ?
                //block no longer on main chain.. delete it?
                blockHeaderRemove(db, block_header);
             }else {
                block_header->onMainChain(true);
                db->update(block_header);

                //THIS MAY NOT BE NECESSARY IF SPV client will call blockHeaderAdd() with header.inBestChain == false
                //get transactions mined in this block
                odb::result<TransactionMinedInBlock> transactions(db->query<TransactionMinedInBlock>(
                                                                  odb::query<TransactionMinedInBlock>::tx_block.block == block_header->id()));
                typedef odb::query<block_headers_view_t> query;
                typedef odb::result<block_headers_view_t> result;

                for(auto &tx : transactions) {
                    //find all other blocks this tx is mined in
                    result blocks(db->query<block_headers_view_t>(query::tx_in_block::tx_block.tx == tx.txid()));

                    //for each block mark it not on main chain
                    for(auto &block : blocks) {
                        if(block.header == block_header) continue;//except the block header being updated
                        block.header->onMainChain(false);
                        db->update(block.header);
                    }
                }
             }
        } else {
            //new header
            block_header = std::shared_ptr<BlockHeader>(new BlockHeader(header));
            db->persist(block_header);
        }
    }

    void transactionMinedInBlockAdd(std::unique_ptr<odb::database> & db,
                                    const Coin::PartialMerkleTree & tree, const Coin::BlockId & blockId) {

        //TODO double check if the block id is in same byte order as persisted value
        std::shared_ptr<BlockHeader> header(db->load<BlockHeader>(blockId.toHex().toStdString()));
        if(!header) return;

        std::vector<uchar_vector> txhashes = tree.getTxHashesVector();

        for (uchar_vector & txhash: txhashes) {

            std::shared_ptr<Transaction> tx(db->load<Transaction>(txhash.getHex()));

            if(!tx) continue;

            TransactionMinedInBlock entry(tx, header);//TODO pass in merkelbranch and index in block?

            try{
                db->persist(entry);
            } catch(odb::object_already_persistent &e) {
                continue;
            }
        }

    }
#endif //store alpha code
} // anonymous namespace (local helper functions)

}//bitcoin
}//joystream
