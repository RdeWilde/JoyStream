#ifndef BITCOIN_STORE_HPP
#define BITCOIN_STORE_HPP

#include <common/Seed.hpp>
#include <common/Network.hpp>
#include <common/PrivateKey.hpp>
#include <common/PublicKeyCompression.hpp>
#include <common/KeyPair.hpp>
#include <common/TransactionId.hpp>
#include <common/UnspentP2PKHOutput.hpp>

#include <CoinCore/hdkeys.h>
#include <CoinCore/CoinNodeData.h>
#include <CoinQ/CoinQ_blocks.h>
#include <CoinCore/BloomFilter.h>

#include <odb/database.hxx>

#include <mutex>

namespace joystream {
namespace bitcoin {
class Store {

public:

    typedef std::function<void(std::string)> transactionDeconfirmedCallback;

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

    Coin::PrivateKey getKey(bool createReceiveAddress);
    std::vector<Coin::PrivateKey> getKeys(uint32_t numKeys, bool createReceiveAddress);

    // returns a vector of key pairs, with ordered ascendingly in terms
    // of index.
    // **NB: createReceiveAddress == false <=> These keys are have no corresponding addresses
    // generates fresh keys
    std::vector<Coin::KeyPair> getKeyPairs(uint32_t num_pairs, bool createReceiveAddress);

    // Generate p2pkh receive address corresponding to a fresh private key.
    // These addresses are monitored for incoming and outgoing spends.
    // generates a fresh key
    Coin::P2PKHAddress getReceiveAddress();

    //total number keys in wallet (including unused)
    uint32_t numberOfKeysInWallet();

    void releaseKey(const Coin::PrivateKey & sk);
    void releaseKey(const Coin::HDKeychain & chain);
    void releaseKeys(const std::vector<Coin::HDKeychain> keychains);
    void releaseAddress(const Coin::P2PKHAddress & p2pkhaddress);

    std::list<Coin::P2PKHAddress> listReceiveAddresses();
    std::list<Coin::PrivateKey> listPrivateKeys();
    std::list<Coin::Transaction> listTransactions();

    bool addressExists(const Coin::P2PKHAddress & addr);
    bool transactionExists(const Coin::TransactionId & txid);
    bool transactionExists(const Coin::Transaction & tx);

    bool loadKey(const Coin::P2PKHAddress &address, Coin::PrivateKey & sk);

    std::list<Coin::UnspentP2PKHOutput> getUnspentTransactionsOutputs(int32_t confirmations = 0, int32_t main_chain_height = 0) const;
    uint64_t getWalletBalance(int32_t confirmations = 0, int32_t main_chain_height = 0) const;

    std::vector<std::string> getLatestBlockHeaderHashes();

    // Methods used to update Store with block headers and transactions
    void addTransaction(const Coin::Transaction & tx);
    void addTransaction(const Coin::Transaction & tx, const ChainMerkleBlock & chainmerkleblock, bool createHeader, transactionDeconfirmedCallback callback);
    void confirmTransaction(std::string txhash, const ChainMerkleBlock &chainmerkleblock, bool createHeader, transactionDeconfirmedCallback callback);
    void addBlockHeader(const ChainMerkleBlock & chainmerkleblock, transactionDeconfirmedCallback callback);
    uint32_t getBestHeaderHeight() const;

private:
    // don't allow copying, store should be passed by reference only
    Store(const Store &){}

    Coin::Network _network;
    Coin::Seed _seed;
    Coin::HDKeychain _rootKeychain;
    uint32_t _timestamp;
    std::unique_ptr<odb::database> _db;
    mutable std::mutex _storeMutex;

    std::map<Coin::PublicKey, uint32_t> _publicKeyToIndex;

    // returns an HDKeychain which persists in wallet
    // **NB: createReceiveAddress == false <=> These keys are have no corresponding addresses
    // which are monitored for inbound/outbound spends.
    // generates fresh key
    Coin::HDKeychain getKeyChain(bool createReceiveAddress);

    // returns a vector of HDKeychain - tries to get any unused keys from the database
    // and supplements them with fresh keys if required
    std::vector<Coin::HDKeychain> getKeyChains(uint32_t numKeys, bool createReceiveAddress);

    //internal method used to persist a new key
    //should be wrapped in an odb::transaction
    Coin::HDKeychain getKeyChain_tx(bool createReceiveAddress);

    void releaseKey(uint32_t index);
};

}//bitcoin
}//joystream

#endif
