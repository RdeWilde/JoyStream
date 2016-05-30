#ifndef BITCOIN_STORE_HPP
#define BITCOIN_STORE_HPP

#include <common/Seed.hpp>
#include <common/Network.hpp>
#include <common/PrivateKey.hpp>
#include <common/PublicKeyCompression.hpp>
#include <common/KeyPair.hpp>
#include <common/TransactionId.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <common/P2SHAddress.hpp>
#include <common/P2PKScriptPubKey.hpp>

#include <CoinCore/hdkeys.h>
#include <CoinCore/CoinNodeData.h>
#include <CoinQ/CoinQ_blocks.h>
#include <CoinCore/BloomFilter.h>

#include <bitcoin/Common.hpp>

#include <odb/database.hxx>

#include <mutex>

namespace joystream {
namespace bitcoin {
class Store {

public:

    typedef std::function<void(Coin::TransactionId, int confirmations)> transactionUpdatedCallback;

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

    Store(){}
    Store(std::string file);
    ~Store();

    bool open(std::string file);
    bool create(std::string file, Coin::Network network);
    bool create(std::string file, Coin::Network network, Coin::Seed seed, uint32_t timestamp);
    bool connected() const;
    void close();

    Coin::Network network() const { return _network; }
    uint32_t created() const { return _timestamp; }
    Coin::Seed seed() const { return _seed; }

    // Return a new private key
    Coin::PrivateKey generateKey(const RedeemScriptGenerator & scriptGenerator);

    // Returns a vector of new keys
    std::vector<Coin::PrivateKey> generateKeys(uint32_t numKeys, const MultiRedeemScriptGenerator & multiScriptGenerator);

    // Returns a vector of new key pairs
    std::vector<Coin::KeyPair> generateKeyPairs(uint32_t numKeys, const MultiRedeemScriptGenerator & multiScriptGenerator);

    // Total number keys in wallet
    uint32_t numberOfKeysInWallet();

    std::vector<Coin::PrivateKey> listPrivateKeys();
    std::vector<uchar_vector> listRedeemScripts();
    std::list<Coin::Transaction> listTransactions();
    std::list<Coin::P2SHAddress> listAddresses();

    bool addressExists(const Coin::P2SHAddress & p2shaddress);
    bool transactionExists(const Coin::TransactionId & txid);

    bool loadKey(const Coin::P2SHAddress &p2shaddress, Coin::PrivateKey & sk);

    std::list<Coin::UnspentP2PKHOutput> getUnspentTransactionsOutputs(int32_t confirmations = 0, int32_t main_chain_height = 0) const;
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
    Coin::Seed _seed;
    Coin::HDKeychain _rootKeychain;
    uint32_t _timestamp;
    std::unique_ptr<odb::database> _db;
    mutable std::mutex _storeMutex;

    //internal method used to persist a new key
    //should be wrapped in an odb::transaction
    Coin::PrivateKey createNewPrivateKey(RedeemScriptGenerator scriptGenerator);

    transactionUpdatedCallback notifyTxUpdated;
};

}//bitcoin
}//joystream

#endif
