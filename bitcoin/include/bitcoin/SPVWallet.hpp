#ifndef BITCOIN_SPVWALLET_HPP
#define BITCOIN_SPVWALLET_HPP

#include <QObject>
#include <common/Network.hpp>
#include <common/UnspentP2PKHOutput.hpp>

#include <CoinQ/CoinQ_netsync.h>
#include <bitcoin/Store.hpp>

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

        CONNECTED,          // netsync connected to peer

        SYNCHING_HEADERS,

        SYNCHING_BLOCKS,

        SYNCHED
    };

    explicit SPVWallet(std::string storePath, std::string blockTreeFile, Coin::Network network = Coin::Network::testnet3);

    // Create a new wallet with auto generated seed
    void Create();

    // Create a new wallet with provided seed (useful for recovering a wallet from seed)
    void Create(Coin::Seed seed, uint32_t timestamp = 0);

    // Open the wallet. Will throw exception on failure.
    void Open();

    // Start Synching the wallet with peer at host:port
    void Sync(std::string host, int port);
    void StopSync();

    bool IsOnline() const { return _walletStatus >= CONNECTED; }

    Coin::PrivateKey GetKey(bool createReceiveAddress);
    std::vector<Coin::PrivateKey> GetKeys(uint32_t numKeys, bool createReceiveAddress);
    std::vector<Coin::KeyPair> GetKeyPairs(uint32_t num_pairs, bool createReceiveAddress);
    void ReleaseKey(const Coin::PrivateKey &sk);
    Coin::P2PKHAddress GetReceiveAddress();

    std::list<Coin::UnspentP2PKHOutput> GetUnspentOutputs(uint64_t minValue, uint32_t minimalConfirmatinos = 0);
    Coin::UnspentP2PKHOutput GetOneUnspentOutput(uint64_t minValue);
    void ReleaseUnspentOutputs(const std::list<Coin::UnspentP2PKHOutput> outputs);

    uint64_t Balance() const;
    uint64_t UnconfirmedBalance() const;

    Coin::Network network() const { return _network; }

    Q_INVOKABLE void BroadcastTx(Coin::Transaction & cointx);

    int32_t bestHeight() const;

    Coin::Transaction SendToAddress(uint64_t value, const Coin::P2PKHAddress &addr, uint64_t fee);

signals:

    void NetSyncStarted();

    void NetSyncStatusMessage(std::string);

    // Error signals
    void StoreError(std::string);
    void BlockTreeError(std::string);
    void ProtocolError(std::string);
    void ConnectionError(std::string);
    void ConnectionTimeout();

    // Wallet Status change
    void StatusChanged(wallet_status_t);

    // Signal corresponding to state transition
    void Offline();
    void Connecting();
    void Disconnected();
    void Connected();
    void SynchingHeaders();
    void SynchingBlocks();
    void BlocksSynched();

    // Signal emitted when wallet balance changes
    void BalanceChanged(uint64_t confirmedBalance, uint64_t unconfirmedBalance);

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
    void LoadBlockTree();

    std::set<uchar_vector> _bloomFilterElements;

    Coin::BloomFilter makeBloomFilter(double falsePositiveRate, uint32_t nTweak, uint32_t nFlags);

    void updateBloomFilter();

    bool transactionShouldBeStored(const Coin::Transaction &) const;
    bool spendsWalletOutput(const Coin::TxIn &) const;
    bool createsWalletOutput(const Coin::TxOut &) const;

    void recalculateBalance();

    // Prefix methods only required from unit tests with test_
    void test_syncBlocksStaringAtHeight(int32_t height);
    int32_t test_netsyncBestHeight() const { return _networkSync.getBestHeight(); }

};

}
}
#endif // BITCOIN_SPVWALLET_HPP
