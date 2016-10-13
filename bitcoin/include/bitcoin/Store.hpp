#ifndef BITCOIN_STORE_HPP
#define BITCOIN_STORE_HPP

#include <common/Seed.hpp>
#include <common/Network.hpp>
#include <common/PrivateKey.hpp>
#include <common/PublicKeyCompression.hpp>
#include <common/KeyPair.hpp>
#include <common/TransactionId.hpp>
#include <common/UnspentP2SHOutput.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <common/P2SHAddress.hpp>
#include <common/P2PKScriptPubKey.hpp>
#include <common/Entropy.hpp>

#include <CoinCore/hdkeys.h>
#include <CoinCore/CoinNodeData.h>
#include <CoinQ/CoinQ_blocks.h>
#include <CoinCore/BloomFilter.h>
#include <CoinCore/bip39.h>

#include <bitcoin/Common.hpp>

#include <odb/database.hxx>

#include <mutex>

namespace joystream {
namespace bitcoin {
class Store {

public:

    enum class KeychainType : uint32_t {
        External = 0,
        Internal = 1,
        Other    = 2  // not part of BIP44 spec
    };

    const uint32_t BIP44_PURPOSE = 0x8000002C;
    const uint32_t BIP44_DEFAULT_ACCOUNT = 0x80000000;
    const uint32_t BIP44_COIN_TYPE_BITCOIN = 0x80000000;
    const uint32_t BIP44_COIN_TYPE_BITCOIN_TESTNET = 0x80000001;

    typedef std::function<void(Coin::TransactionId, int confirmations)> transactionUpdatedCallback;
    typedef std::function<RedeemScriptInfo(const Coin::PublicKey &, uint32_t n)> MultiRedeemScriptGenerator;
    typedef std::function<Coin::UnspentOutput*(const KeychainType &, const Coin::KeyPair &,
                                               const uchar_vector &script, const uchar_vector &optionalData,
                                               const Coin::typesafeOutPoint, const int64_t &value,
                                               bool &processNextSelector)> UnspentOutputSelector;

    // Custom Store Exceptions
    class BlockHeaderNotFound : public std::runtime_error {
    public:
        BlockHeaderNotFound() : std::runtime_error("BlockHeader missing") {}
    };

    class BlockHeaderDoesNotConnect : public std::runtime_error {
    public:
        BlockHeaderDoesNotConnect() : std::runtime_error("BlockHeader doesn't connect to stored chain") {}
    };

    class TransactionNotFound : public std::runtime_error {
    public:
        TransactionNotFound() : std::runtime_error("Transaction not found in store") {}
    };

    class NotConnected : public std::runtime_error {
    public:
        NotConnected() : std::runtime_error("Database not connected") {}
    };

    class OperationNotAllowed : public std::runtime_error {
    public:
        OperationNotAllowed() : std::runtime_error("Store operation not allowed") {}
    };

    Store(){}
    Store(std::string file, Coin::Network network);
    ~Store();

    // optional passphrase to decrypt entropy when loaded from db, only try it if metadata says the entropy is encrypted
    bool open(std::string file, Coin::Network network);
    bool create(std::string file, Coin::Network network);
    bool create(std::string file, Coin::Network network, const Coin::Entropy &entropy, uint32_t timestamp);

    bool encrypted() const;// is entropy in db encrypted?
    void encrypt(std::string passphrase);//encrypt the entropy in db (throw if already encrypted or something else goes wrong)
    void decrypt(std::string passphrase);//decrypt the entropy in db (throw if not encrypted, or wrong passphrase) and unlock

    bool locked() const; // is the store in locked state?
    void lock(); // set store to locked state
    void unlock(std::string passphrase);

    bool connected() const;
    void close();

    Coin::Network network() const { return _network; }
    uint32_t created() const { return _timestamp; }
    std::string getSeedWords() const;

    // Keys for use with P2SH addresses
    // Return a new private key
    Coin::PrivateKey generatePrivateKey(const RedeemScriptGenerator & scriptGenerator);

    // Returns a vector of new keys
    std::vector<Coin::PrivateKey> generatePrivateKeys(uint32_t numKeys, const MultiRedeemScriptGenerator & multiScriptGenerator);

    // Returns a vector of new key pairs
    std::vector<Coin::KeyPair> generateKeyPairs(uint32_t numKeys, const MultiRedeemScriptGenerator & multiScriptGenerator);

    // BIP44 - Keys for use with P2PKH Addresses, by default we generate a key for external use (receive address) change = 0
    // To generate a key for use with an internal address (for example a change output, pass argument change = 1)
    // Return a new private key
    Coin::PrivateKey generatePrivateKey(KeychainType type);
    Coin::PublicKey generatePublicKey(KeychainType type);

    // Returns a vector of new keys
    std::vector<Coin::PrivateKey> generatePrivateKeys(uint32_t numKeys, KeychainType chainType);

    // Returns a vector of new key pairs
    std::vector<Coin::KeyPair> generateKeyPairs(uint32_t numKeys, KeychainType chainType);

    Coin::PrivateKey generateReceivePrivateKey();

    Coin::PrivateKey generateChangePrivateKey();

    Coin::PrivateKey derivePrivateKey(KeychainType chainType, uint32_t index) const;

    Coin::PublicKey derivePublicKey(KeychainType chainType, uint32_t index) const;

    Coin::PublicKey generateReceivePublicKey();

    Coin::PublicKey generateChangePublicKey();

    std::vector<Coin::PublicKey> listPublicKeys(KeychainType chainType) const;
    std::vector<uchar_vector> listRedeemScripts() const;

    //std::list<Coin::Transaction> listTransactions();
    //std::list<Coin::P2SHAddress> listAddresses();
    //bool addressExists(const Coin::P2SHAddress & p2shaddress);
    bool transactionExists(const Coin::TransactionId & txid);
    //bool loadKey(const Coin::P2SHAddress &p2shaddress, Coin::PrivateKey & sk);

    std::list<std::shared_ptr<Coin::UnspentOutput> > getUnspentTransactionsOutputs(const std::vector<UnspentOutputSelector> & outputSelectors,
                                                                                   int32_t confirmations = 0,
                                                                                   int32_t main_chain_height = 0) const;
    uint64_t getWalletBalance(int32_t confirmations = 0, int32_t main_chain_height = 0) const;

    std::vector<std::string> getLatestBlockHeaderHashes();

    // Methods used to update Store with block headers and transactions
    void addTransaction(const Coin::Transaction & tx);
    void addTransaction(const Coin::Transaction & tx, const ChainMerkleBlock & chainmerkleblock);
    void confirmTransaction(Coin::TransactionId txid, const ChainMerkleBlock &chainmerkleblock);
    void addBlockHeader(const ChainMerkleBlock & chainmerkleblock);
    uint32_t getBestHeaderHeight() const;

    void setTxUpdatedCallback(transactionUpdatedCallback callback) { notifyTxUpdated = callback; }

private:
    // don't allow copying, store should be passed by reference only
    Store(const Store &){}

    Coin::Network _network;
    Coin::Entropy _entropy;
    bool _locked;
    uint32_t _coin_type;
    Coin::HDKeychain _accountPrivKeychain;
    Coin::HDKeychain _accountPubKeychain;
    uint32_t _timestamp;
    std::unique_ptr<odb::database> _db;
    mutable std::mutex _storeMutex;

    //internal methods used to persist new keys
    //should be wrapped in an odb::transaction
    Coin::PrivateKey createNewPrivateKey(RedeemScriptGenerator scriptGenerator, uint32_t index);
    Coin::PrivateKey createNewPrivateKey(KeychainType chainType, uint32_t index);
    Coin::PublicKey createNewPublicKey(KeychainType chainType, uint32_t index);

    // Total number of keys of change type
    uint32_t getNextKeyIndex(KeychainType chainType);

    transactionUpdatedCallback notifyTxUpdated;

    uchar_vector keyFromPassphrase(std::string passphrase, uchar_vector random_salt) const;
};

}//bitcoin
}//joystream

#endif
