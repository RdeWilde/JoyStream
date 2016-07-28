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

#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

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
    odb::result<detail::store::outputs_view_t> getOutputs(const std::unique_ptr<odb::database> & db, int32_t confirmations, int32_t main_chain_height);

    uchar_vector aes_128_gcm_encrypt(uchar_vector plaintext, uchar_vector key)
    {
        //check key.size() == 128

        size_t enc_length = plaintext.size()*3;
        uchar_vector output;
        output.resize(enc_length,'\0');

        unsigned char tag[AES_BLOCK_SIZE];
        unsigned char iv[AES_BLOCK_SIZE];
        RAND_bytes(iv, sizeof(iv));
        std::copy( iv, iv+16, output.begin()+16);

        int actual_size=0, final_size=0;
        EVP_CIPHER_CTX* e_ctx = EVP_CIPHER_CTX_new();
        //EVP_CIPHER_CTX_ctrl(e_ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL);
        EVP_EncryptInit(e_ctx, EVP_aes_128_gcm(), &key[0], iv);
        EVP_EncryptUpdate(e_ctx, &output[32], &actual_size, &plaintext[0], plaintext.size() );

        if(EVP_EncryptFinal(e_ctx, &output[32+actual_size], &final_size) == 1) {
            EVP_CIPHER_CTX_ctrl(e_ctx, EVP_CTRL_GCM_GET_TAG, 16, tag);
            std::copy( tag, tag+16, output.begin() );
            std::copy( iv, iv+16, output.begin()+16);
            output.resize(32 + actual_size+final_size);
            EVP_CIPHER_CTX_free(e_ctx);
            return output;
        } else {
            EVP_CIPHER_CTX_free(e_ctx);
            throw std::runtime_error(ERR_error_string(ERR_get_error(), NULL));
        }

    }

    uchar_vector aes_128_gcm_decrypt(uchar_vector ciphertext, uchar_vector key)
    {
        // check key.size() == 128;

        unsigned char tag[AES_BLOCK_SIZE];
        unsigned char iv[AES_BLOCK_SIZE];
        std::copy( ciphertext.begin(),    ciphertext.begin()+16, tag);
        std::copy( ciphertext.begin()+16, ciphertext.begin()+32, iv);
        uchar_vector plaintext; plaintext.resize(ciphertext.size(), '\0');

        int actual_size=0, final_size=0;
        EVP_CIPHER_CTX *d_ctx = EVP_CIPHER_CTX_new();
        EVP_DecryptInit(d_ctx, EVP_aes_128_gcm(), &key[0], iv);
        EVP_DecryptUpdate(d_ctx, &plaintext[0], &actual_size, &ciphertext[32], ciphertext.size()-32 );
        EVP_CIPHER_CTX_ctrl(d_ctx, EVP_CTRL_GCM_SET_TAG, 16, tag);
        if(EVP_DecryptFinal(d_ctx, &plaintext[actual_size], &final_size) == 1) {
            EVP_CIPHER_CTX_free(d_ctx);
            plaintext.resize(actual_size + final_size, '\0');
            return plaintext;
        } else {
            EVP_CIPHER_CTX_free(d_ctx);
            throw std::runtime_error(ERR_error_string(ERR_get_error(), NULL));
        }
    }

}

// open existing store
// used Store::connected() to check if store was opened successfully
Store::Store(std::string file, Coin::Network network) {
    open(file, network);
}

Store::~Store(){
    close();
}

// open existing store, return true if opened successfully
bool Store::open(std::string file, Coin::Network network) {

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

        // Make sure the network the client is attempting to use matches the store's network
        if(network != metadata->network()){
            throw std::runtime_error("Network Mismatch");
        }

        _network = network;

        _coin_type = network == Coin::Network::mainnet ? BIP44_COIN_TYPE_BITCOIN : BIP44_COIN_TYPE_BITCOIN_TESTNET;

        _timestamp = metadata->created();

        _accountPubKeychain = Coin::HDKeychain(uchar_vector(metadata->xpublicKey()));

        if(metadata->encrypted()) {
            _locked = true;
            std::cerr << "Store opened in locked state\n";
        }else {
            _locked = false;
            _entropy = Coin::Entropy(metadata->entropy());
            _accountPrivKeychain = _entropy.seed().generateHDKeychain().getChild(BIP44_PURPOSE).getChild(_coin_type).getChild(BIP44_DEFAULT_ACCOUNT);
        }

        return true;

    } catch (odb::exception &e) {
        // failed to open the database file
        std::cerr << e.what() << std::endl;
        close();
    } catch (std::runtime_error &e) {
        std::cerr << "Store::open() error: " << e.what() << std::endl;
        close();
    }

    return false;
}

bool Store::create(std::string file, Coin::Network network) {
    return create(file, network, Coin::Entropy::generate(), std::time(nullptr));
}

bool Store::create(std::string file, Coin::Network network, const Coin::Entropy & entropy, uint32_t timestamp) {

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
        _coin_type = _network == Coin::Network::mainnet ? BIP44_COIN_TYPE_BITCOIN : BIP44_COIN_TYPE_BITCOIN_TESTNET;
        _entropy = entropy;
        _accountPrivKeychain = _entropy.seed().generateHDKeychain().getChild(BIP44_PURPOSE).getChild(_coin_type).getChild(BIP44_DEFAULT_ACCOUNT);
        _accountPubKeychain = _accountPrivKeychain.getPublic();
        _timestamp = timestamp;
        detail::store::Metadata metadata(_entropy.getHex(), uchar_vector(_accountPubKeychain.extkey()).getHex(), timestamp, network);
        _timestamp = timestamp;
        _locked = false;
        _db->persist(metadata);

        t.commit ();
        c->execute ("PRAGMA foreign_keys=ON");
        return true;

    } catch (odb::exception &e) {
        // failed to initialise the database
        std::cerr << "Store::create() error: " << e.what() << std::endl;
        close();

        // cleanup
        if(boost::filesystem::exists(file)) {
            boost::filesystem::remove(file);
        }
    }

    return false;
}

std::string Store::getSeedWords() const
{
    if(_locked) {
        throw OperationNotAllowed();
    }

    return _entropy.mnemonic();
}

bool Store::locked() const {
    return _locked;
}

bool Store::encrypted() const {
    std::lock_guard<std::mutex> lock(_storeMutex);
    //load metadata
    odb::transaction t(_db->begin());
    std::shared_ptr<detail::store::Metadata> metadata(_db->query_one<detail::store::Metadata>());
    return metadata->encrypted();
}

uchar_vector Store::keyFromPassphrase(std::string passphrase, uchar_vector random_salt) const {
    const char * password = passphrase.c_str();

    unsigned char salt[8];
    random_salt.copyToArray(salt);

    unsigned char digest[32]; //256 bits

    if(!PKCS5_PBKDF2_HMAC(password, strlen(password), salt, sizeof(salt), 2048, EVP_sha512(), sizeof(digest), digest)){
        throw std::runtime_error(ERR_error_string(ERR_get_error(), NULL));
    }

    Coin::UCharArray<32> key(QByteArray((char*)digest, 32));

    return key.toUCharVector();
}

void Store::encrypt(std::string passphrase) {
    std::lock_guard<std::mutex> lock(_storeMutex);
    //load metadata
    odb::transaction t(_db->begin());
    std::shared_ptr<detail::store::Metadata> metadata(_db->query_one<detail::store::Metadata>());
    if(metadata->encrypted()) {
        throw std::runtime_error("Store::encrypt() store is already encrypted");
    }

    unsigned char random_salt[8];

    if (!RAND_bytes(random_salt, sizeof(random_salt))) {
        throw std::runtime_error(ERR_error_string(ERR_get_error(), NULL));
    }

    uchar_vector salt(random_salt, sizeof(random_salt));

    metadata->salt(salt.getHex());

    auto key = keyFromPassphrase(passphrase, salt);

    uchar_vector entropy(metadata->entropy());

    metadata->entropy(aes_128_gcm_encrypt(entropy, key).getHex());

    metadata->encrypted(true);

    _db->update(metadata);
    t.commit();

    //encrypting the wallet doesn't change lock status..
}

void Store::decrypt(std::string passphrase) {
    std::lock_guard<std::mutex> lock(_storeMutex);
    //load metadata
    odb::transaction t(_db->begin());
    std::shared_ptr<detail::store::Metadata> metadata(_db->query_one<detail::store::Metadata>());
    if(!metadata->encrypted()) {
        throw std::runtime_error("Store::decrypt() store is not enrypted");
    }

    auto key = keyFromPassphrase(passphrase, uchar_vector(metadata->salt()));

    uchar_vector entropy(metadata->entropy());

    metadata->entropy(aes_128_gcm_decrypt(entropy, key).getHex());

    metadata->encrypted(false);

    _db->update(metadata);
    t.commit();

    // decrypting wallet also unlocks it
    _entropy = Coin::Entropy(metadata->entropy());
    _accountPrivKeychain = _entropy.seed().generateHDKeychain().getChild(BIP44_PURPOSE).getChild(_coin_type).getChild(BIP44_DEFAULT_ACCOUNT);
    _locked = false;
}

void Store::lock() {
    _locked = true;
}

void Store::unlock(std::string passphrase) {
    std::lock_guard<std::mutex> lock(_storeMutex);
    //load metadata
    odb::transaction t(_db->begin());
    std::shared_ptr<detail::store::Metadata> metadata(_db->query_one<detail::store::Metadata>());
    if(!metadata->encrypted()) {
        throw std::runtime_error("Store::unlock() store is not enrypted");
    }

    auto key = keyFromPassphrase(passphrase, uchar_vector(metadata->salt()));

    uchar_vector entropy(metadata->entropy());

    auto plaintext_entropy = aes_128_gcm_decrypt(entropy, key);

    _entropy = Coin::Entropy(plaintext_entropy);
    _accountPrivKeychain = _entropy.seed().generateHDKeychain().getChild(BIP44_PURPOSE).getChild(_coin_type).getChild(BIP44_DEFAULT_ACCOUNT);
    _locked = false;
}

bool Store::connected() const {
    return _db != nullptr;
}

void Store::close() {
    std::lock_guard<std::mutex> lock(_storeMutex);

    if(_db) _db.reset();
    _entropy.clear();
}

// Generates a new key - P2SH
Coin::PrivateKey Store::generatePrivateKey(const RedeemScriptGenerator & scriptGenerator) {
    if(!connected()) {
        throw NotConnected();
    }

    if(_locked) {
        throw OperationNotAllowed();
    }

    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());

    uint32_t index = getNextKeyIndex(KeychainType::Other);

    Coin::PrivateKey sk(createNewPrivateKey(scriptGenerator, index));
    t.commit();

    return sk;
}

std::vector<Coin::PrivateKey> Store::generatePrivateKeys(uint32_t numKeys, const Store::MultiRedeemScriptGenerator & multiScriptGenerator) {
    if(!connected()) {
        throw NotConnected();
    }

    if(_locked) {
        throw OperationNotAllowed();
    }

    std::lock_guard<std::mutex> lock(_storeMutex);

    std::vector<Coin::PrivateKey> privKeys;

    odb::transaction t(_db->begin());

    uint32_t index = getNextKeyIndex(KeychainType::Other);

    for(uint32_t n = 0; n < numKeys; n++, index++) {
        privKeys.push_back(createNewPrivateKey([&n, &multiScriptGenerator](const Coin::PublicKey & pubKey){
            return multiScriptGenerator(pubKey, n);
        }, index));
    }

    t.commit();

    return privKeys;
}

std::vector<Coin::KeyPair> Store::generateKeyPairs(uint32_t numKeys, const Store::MultiRedeemScriptGenerator & multiScriptGenerator) {

    std::vector<Coin::KeyPair> keyPairs;

    for(auto sk : generatePrivateKeys(numKeys, multiScriptGenerator)) {
        keyPairs.push_back(Coin::KeyPair(sk));
    }

    return keyPairs;
}

// Generates a new key - P2PKH
Coin::PrivateKey Store::generatePrivateKey(KeychainType type) {
    if(!connected()) {
        throw NotConnected();
    }

    if(_locked) {
        throw OperationNotAllowed();
    }

    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());

    uint32_t index = getNextKeyIndex(type);

    Coin::PrivateKey sk(createNewPrivateKey(type, index));
    t.commit();

    return sk;
}

// Generates a new key - P2PKH
Coin::PublicKey Store::generatePublicKey(KeychainType type) {
    if(!connected()) {
        throw NotConnected();
    }

    std::lock_guard<std::mutex> lock(_storeMutex);

    odb::transaction t(_db->begin());

    uint32_t index = getNextKeyIndex(type);

    Coin::PublicKey pk(createNewPublicKey(type, index));
    t.commit();

    return pk;
}

std::vector<Coin::PrivateKey> Store::generatePrivateKeys(uint32_t numKeys, KeychainType chainType) {
    if(!connected()) {
        throw NotConnected();
    }

    if(_locked) {
        throw OperationNotAllowed();
    }

    std::lock_guard<std::mutex> lock(_storeMutex);

    std::vector<Coin::PrivateKey> privKeys;

    odb::transaction t(_db->begin());

    uint32_t index = getNextKeyIndex(chainType);

    for(uint32_t n = 0; n < numKeys; n++, index++) {
        privKeys.push_back(createNewPrivateKey(chainType, index));
    }

    t.commit();

    return privKeys;
}

std::vector<Coin::KeyPair> Store::generateKeyPairs(uint32_t numKeys, KeychainType chainType) {

    std::vector<Coin::KeyPair> keyPairs;

    for(auto sk : generatePrivateKeys(numKeys, chainType)) {
        keyPairs.push_back(Coin::KeyPair(sk));
    }

    return keyPairs;
}

Coin::PrivateKey Store::generateReceivePrivateKey() {
    return generatePrivateKey(KeychainType::External);
}


Coin::PrivateKey Store::generateChangePrivateKey() {
    return generatePrivateKey(KeychainType::Internal);
}

Coin::PublicKey Store::generateReceivePublicKey() {
    return generatePublicKey(KeychainType::External);
}


Coin::PublicKey Store::generateChangePublicKey() {
    return generatePublicKey(KeychainType::Internal);
}

Coin::PrivateKey Store::derivePrivateKey(KeychainType chainType, uint32_t index) const {
    if(_locked) {
        throw OperationNotAllowed();
    }

    Coin::HDKeychain hdKeyChain = _accountPrivKeychain.getChild((uint32_t)chainType).getChild(index);

    Coin::PrivateKey sk(hdKeyChain.privkey());

    return sk;
}

Coin::PublicKey Store::derivePublicKey(KeychainType chainType, uint32_t index) const {

    Coin::HDKeychain hdKeyChain = _accountPubKeychain.getChild((uint32_t)chainType).getChild(index);

    Coin::PublicKey pk(hdKeyChain.pubkey());

    return pk;
}

std::vector<Coin::PublicKey> Store::listPublicKeys(KeychainType chainType) const {
    if(!connected()) {
        throw NotConnected();
    }

    typedef odb::query<detail::store::key_view_t> query;
    typedef odb::result<detail::store::key_view_t> result;

    std::vector<Coin::PublicKey> keys;
    odb::session s;
    odb::transaction t(_db->begin());
    result r(_db->query<detail::store::key_view_t>(query::address::id.is_not_null() && query::key::path.coin_type == _coin_type && query::key::path.change == (uint32_t)chainType));
    for(auto &entry : r) {
        keys.push_back(derivePublicKey(chainType, entry.key->index()));
    }

    return keys;
}

// Needed to update bloom filter to detect P2SH outputs and spends
// We can mark p2sh addresses spent when joystream paychan is settled (keeping bloom filter to minimal size)
std::vector<uchar_vector> Store::listRedeemScripts() const {
    if(!connected()) {
        throw NotConnected();
    }

    typedef odb::query<detail::store::key_view_t> query;
    typedef odb::result<detail::store::key_view_t> result;

    std::vector<uchar_vector> scripts;

    odb::transaction t(_db->begin());
    result r(_db->query<detail::store::key_view_t>(query::address::id.is_not_null() && query::key::path.coin_type == _coin_type && query::key::path.change == (uint32_t)KeychainType::Other));
    for(auto &entry : r) {
        scripts.push_back(uchar_vector(entry.address->redeemScript()));
    }

    return scripts;
}

/*
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

std::list<Coin::P2SHAddress> Store::listAddresses() {

    std::list<Coin::P2SHAddress> p2shAddresses;

    for(uchar_vector script : listRedeemScripts()) {
        p2shAddresses.insert(p2shAddresses.end(), Coin::P2SHAddress(_network, Coin::RedeemScriptHash::fromRawScript(script)));
    }

    return p2shAddresses;
}

bool Store::addressExists(const Coin::P2SHAddress & p2shaddress) {
    if(!connected()) {
        throw NotConnected();
    }

    odb::transaction t(_db->begin());
    typedef odb::query<detail::store::Address> query;
    std::string scriptPubKey = p2shaddress.toP2SHScriptPubKey().serialize().getHex();

    // schema allows only one unique address to be stored so this should never throw an exception
    if(_db->query_one<detail::store::Address>(query::scriptPubKey == scriptPubKey)) {
        return true;
    }

    return false;
}
*/

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

/*
bool Store::loadKey(const Coin::P2SHAddress & p2shaddress, Coin::PrivateKey & sk) {
    if(!connected()) {
        throw NotConnected();
    }

    bool found = false;
    typedef odb::query<detail::store::Address> query;
    odb::transaction t(_db->begin());
    std::string scriptPubKey = p2shaddress.toP2SHScriptPubKey().serialize().getHex();
    std::shared_ptr<detail::store::Address> addr(_db->query_one<detail::store::Address>(query::scriptPubKey == scriptPubKey));
    if(addr) {
        sk = addr->key()->getPrivateKey();
        found = true;
    }

    return found;
}
*/

std::list<std::shared_ptr<Coin::UnspentOutput>>
Store::getUnspentTransactionsOutputs(int32_t confirmations, int32_t main_chain_height, const RedeemScriptFilter &scriptFilter) const {
    if(!connected()) {
        throw NotConnected();
    }

    if(_locked) {
        throw OperationNotAllowed();
    }

    if(confirmations > 0  && main_chain_height == 0) {
        throw std::runtime_error("getUnspentTransactionOutputs: must provide main_chain_height");
    }

    if(confirmations > 0 && confirmations > main_chain_height) {
        return std::list<std::shared_ptr<Coin::UnspentOutput>>();
    }

    typedef odb::result<detail::store::outputs_view_t> outputs_r;
    odb::session s;
    odb::transaction t(_db->begin());
    auto outputs = getOutputs(_db, confirmations, main_chain_height);

    std::list<std::shared_ptr<Coin::UnspentOutput>> utxos;

    for(auto & output : outputs) {

        // Filter addresses with a script filter
        if(scriptFilter && !scriptFilter(uchar_vector(output.address->redeemScript()))){
            continue;
        }

        Coin::KeyPair keypair(derivePrivateKey((KeychainType)output.address->key()->change(), output.address->key()->index()));
        Coin::TransactionId txid(Coin::TransactionId::fromRPCByteOrder(uchar_vector(output.txid())));
        Coin::typesafeOutPoint outpoint(txid, output.index());

        if(output.address->redeemScript() != "") {
            std::shared_ptr<Coin::UnspentOutput> utxo(new Coin::UnspentP2SHOutput(keypair, uchar_vector(output.address->redeemScript()), uchar_vector(output.address->optionalData()), outpoint, output.value()));
            utxos.insert(utxos.end(), utxo);
        } else {
            std::shared_ptr<Coin::UnspentOutput> utxo(new Coin::UnspentP2PKHOutput(keypair, outpoint, output.value()));
            utxos.insert(utxos.end(), utxo);
        }
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

    odb::session s;
    odb::transaction t(_db->begin());

    for(auto & output : getOutputs(_db, confirmations, main_chain_height)) {
        balance += output.value();
    }

    return balance;
}

Coin::PrivateKey Store::createNewPrivateKey(RedeemScriptGenerator scriptGenerator, uint32_t index) {
    if(!connected()) {
        throw NotConnected();
    }

    if(_locked) {
        throw OperationNotAllowed();
    }

    if(!scriptGenerator) {
        throw std::runtime_error("redeem script generator function not provided");
    }

    Coin::PrivateKey sk(derivePrivateKey(KeychainType::Other, index));

    // persist a new key
    std::shared_ptr<detail::store::Key> key(new detail::store::Key(_coin_type, (uint32_t)KeychainType::Other, index));
    _db->persist(key);

    // persist new p2sh address
    std::shared_ptr<detail::store::Address> address(new detail::store::Address(key, scriptGenerator(sk.toPublicKey())));
    _db->persist(address);
    return sk;
}

Coin::PrivateKey Store::createNewPrivateKey(KeychainType chainType, uint32_t index) {
    if(!connected()) {
        throw NotConnected();
    }

    if(_locked) {
        throw OperationNotAllowed();
    }

    Coin::PrivateKey sk(derivePrivateKey(chainType, index));

    // persist a new key
    std::shared_ptr<detail::store::Key> key(new detail::store::Key(_coin_type, (uint32_t)chainType, index));
    _db->persist(key);

    // persist new p2pkh address
    auto scriptPubKey = Coin::P2PKHScriptPubKey(sk.toPublicKey());
    std::shared_ptr<detail::store::Address> address(new detail::store::Address(key, scriptPubKey));
    _db->persist(address);
    return sk;
}

Coin::PublicKey Store::createNewPublicKey(KeychainType chainType, uint32_t index) {
    if(!connected()) {
        throw NotConnected();
    }

    Coin::PublicKey pk(derivePublicKey(chainType, index));

    // persist a new key
    std::shared_ptr<detail::store::Key> key(new detail::store::Key(_coin_type, (uint32_t)chainType, index));
    _db->persist(key);

    // persist new p2pkh address
    auto scriptPubKey = Coin::P2PKHScriptPubKey(pk);
    std::shared_ptr<detail::store::Address> address(new detail::store::Address(key, scriptPubKey));
    _db->persist(address);
    return pk;
}

uint32_t Store::getNextKeyIndex(KeychainType chainType) {

    if(!connected()) {
        throw NotConnected();
    }

    typedef odb::query<detail::store::Key> query;

    auto stat(_db->query_value<detail::store::key_stat_t>(query::path.coin_type == _coin_type && query::path.change == (uint32_t)chainType));

    if(stat.count == 0) return 0;

    return stat.max_index + 1;
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

    odb::result<outputs_view_t>
    getOutputs(const std::unique_ptr<odb::database> &db, int32_t confirmations, int32_t main_chain_height) {

        uint32_t maxHeight = main_chain_height - confirmations + 1;
        // a transaction has  main_chain_height - block_height  = confirmations
        //                    main_chain_heigth - confirmations = block_height
        //                                          ^ goes up   -->  ^ goes down
        // for a transaction to have at least N confirmations
        // the block it is mined in must have a height no larger than:  main_chain_height - N = maxHeight
        typedef odb::result<outputs_view_t> outputs_r;
        typedef odb::query<outputs_view_t> outputs_q;
        outputs_r results;
        if(confirmations > 0) {
            results = db->query<outputs_view_t>(
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
            results = db->query<detail::store::outputs_view_t>((outputs_q::address::id.is_not_null() &&
                                                                outputs_q::output_tx::txid.is_not_null() &&
                                                                outputs_q::spending_tx::txid.is_null()) +
                                                                "ORDER BY" + outputs_q::Output::id.value + "DESC");
        }

        return results;
    }

} // anonymous namespace (local helper functions)

}//bitcoin
}//joystream
