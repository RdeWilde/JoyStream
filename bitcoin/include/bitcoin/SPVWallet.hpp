#ifndef BITCOIN_SPVWALLET_HPP
#define BITCOIN_SPVWALLET_HPP

#include <QObject>
#include <common/Network.hpp>
#include <common/UnspentP2PKHOutput.hpp>

#include <CoinQ/CoinQ_netsync.h>
#include <bitcoin/Store.hpp>

#include <mutex>

class Test;

namespace joystream {
namespace bitcoin {

class SPVWallet : public QObject
{
    Q_OBJECT

public:

    enum wallet_status_t
    {
        UNINITIALIZED,      // wallet database file not yet opened, netsync stopped
                            // this is the starting state of the wallet, it will never go back
                            // to this state after successful opening/creating of the wallet database

        OFFLINE,            // wallet database ready, netsync stopped

        CONNECTING,         // netsync connecting, starting background threads

        DISCONNECTED,       // netsync peer disconnected, stopping background threads
                            // when threads stop, status will transition to OFFLINE

        CONNECTED,          // netsync connected to peer

        SYNCHING_HEADERS,   // netsync is retrieving headers and updating blocktree

        SYNCHING_BLOCKS,    // netsync is retreiving filtered blocks and the wallet is
                            // updating the store block headers and transactions

        SYNCHED             // receiving mempool (unconfirmed transactions) and waiting for next block
    };

    explicit SPVWallet(std::string storePath, std::string blockTreeFile, Coin::Network network = Coin::Network::testnet3);

    // Create a new wallet with auto generated seed
    void create();

    // Create a new wallet with provided seed (useful for recovering a wallet from seed)
    void create(Coin::Seed seed, uint32_t timestamp = 0);

    // Open the wallet. Will throw exception on failure.
    void open();

    // Start Synching the wallet with peer at host:port
    void sync(std::string host, int port);
    void stopSync();

    wallet_status_t status() const { return _walletStatus; }

    bool isInitialized() const { return _walletStatus != UNINITIALIZED; }
    bool isConnected() const { return _walletStatus >= CONNECTED; }
    bool isSynchingHeaders() const { return _walletStatus == SYNCHING_HEADERS;}
    bool isSynchingBlocks() const { return _walletStatus == SYNCHING_BLOCKS;}
    bool isSynched() const { return _walletStatus == SYNCHED;}

    Coin::PrivateKey getKey(bool createReceiveAddress);
    std::vector<Coin::PrivateKey> getKeys(uint32_t numKeys, bool createReceiveAddress);
    std::vector<Coin::KeyPair> getKeyPairs(uint32_t num_pairs, bool createReceiveAddress);
    void releaseKey(const Coin::PrivateKey &sk);
    Coin::P2PKHAddress getReceiveAddress();

    std::list<Coin::UnspentP2PKHOutput> lockOutputs(uint64_t minValue, uint32_t minimalConfirmations = 0);
    void unlockOutputs(const std::list<Coin::UnspentP2PKHOutput> outputs);

    uint64_t balance() const;
    uint64_t unconfirmedBalance() const;

    Coin::Network network() const { return _network; }

    Q_INVOKABLE void broadcastTx(Coin::Transaction & cointx);

    int32_t bestHeight() const;

signals:

    void syncStarted();

    // A general status message to display in the UI
    void statusMessageUpdated(std::string);

    // Error signals    
    void storeUpdateFailed(std::string);
    void blockTreeUpdateFailed(std::string);

    void protocolError(std::string);
    void connectionError(std::string);
    void connectionTimedOut();

    // Wallet Status change
    void statusChanged(wallet_status_t);

    // Signal corresponding to state transition
    void offline();
    void connecting();
    void disconnected();
    void connected();
    void synchingHeaders();
    void synchingBlocks();
    void synched();

    // Signal emitted when wallet balance changes
    void balanceChanged(uint64_t confirmedBalance, uint64_t unconfirmedBalance);

    void txUpdated(Coin::TransactionId txid, int confirmations);

public slots:


private:
    friend class ::Test;

    std::string _storePath;

    Store _store;

    Coin::Network _network;

    CoinQ::Network::NetworkSync _networkSync;
    wallet_status_t _walletStatus;

    void updateStatus(wallet_status_t status);

    std::string _blockTreeFile;
    bool _blockTreeLoaded;
    bool _blockTreeError;

    uint64_t _confirmedBalance;
    uint64_t _unconfirmedBalance;

    // NetSync event handlers
    void onBlockTreeError(const std::string &error, int code);
    void onBlockTreeChanged();
    void onSynchingHeaders();
    void onHeadersSynched();
    void onSynchingBlocks();
    void onBlocksSynched();
    void onNewTx(const Coin::Transaction& cointx);
    void onTxConfirmed(const ChainMerkleBlock& chainmerkleblock, const bytes_t& txhash, unsigned int txindex, unsigned int txcount);
    void onMerkleTx(const ChainMerkleBlock& chainmerkleblock, const Coin::Transaction& cointx, unsigned int txindex, unsigned int txcount);
    void onMerkleBlock(const ChainMerkleBlock& chainmerkleblock);

    // Load the blocktree file
    void loadBlockTree();

    std::set<uchar_vector> _bloomFilterPubKeyHashes;
    std::set<uchar_vector> _bloomFilterCompressedPubKeys;

    Coin::BloomFilter makeBloomFilter(double falsePositiveRate, uint32_t nTweak, uint32_t nFlags);

    void updateBloomFilter();

    bool transactionShouldBeStored(const Coin::Transaction &) const;
    bool spendsWalletOutput(const Coin::TxIn &) const;
    bool createsWalletOutput(const Coin::TxOut &) const;

    void recalculateBalance();

    mutable std::mutex _utxoMutex;
    std::set<Coin::typesafeOutPoint> _lockedOutpoints;
    
    // Prefix methods only required from unit tests with test_
    void test_syncBlocksStaringAtHeight(int32_t height);
    int32_t test_netsyncBestHeight() const { return _networkSync.getBestHeight(); }
    Coin::Transaction test_sendToAddress(uint64_t value, const Coin::P2PKHAddress &addr, uint64_t fee);

};

}
}
#endif // BITCOIN_SPVWALLET_HPP
