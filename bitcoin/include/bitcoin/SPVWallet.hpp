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

    enum netsync_status_t
    {
        STOPPED,
        STARTING,
        SYNCHING_HEADERS,
        SYNCHING_BLOCKS,
        SYNCHED
    };

    explicit SPVWallet(std::string storePath, std::string blockTreeFile, Coin::Network network = Coin::Network::testnet3);

    ~SPVWallet();

    // Create a new wallet with auto generated seed
    void Create();

    // Create a new wallet with provided seed (useful for recovering a wallet from seed)
    void Create(Coin::Seed seed);

    // Open the wallet. Will throw exception on failure.
    void Open();

    // Start Synching the wallet with peer at host:port
    void Sync(std::string host, int port);
    void StopSync();

    bool IsOnline() const { return _store.connected() && _networkSync.connected(); }

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

    Q_INVOKABLE void BroadcastTx(Coin::Transaction & tx);

    int bestHeight() const { return _networkSync.getBestHeight(); }

signals:

    void NetSyncConnected();
    void NetSyncDisconnected();
    void BlockTreeError();
    void BlockTreeChanged();
    void SynchingHeaders();
    void HeadersSynched();
    void SynchingBlocks();
    void BlocksSynched();
    void BalanceChanged(uint64_t confirmedBalance, uint64_t unconfirmedBalance);

    void MerkleBlock();
    void MerkleTx();
    void NewTx();
    void TxConfirmed();

public slots:


private:
    friend class ::Test;

    std::string _storePath;

    Store _store;

    Coin::Network _network;

    CoinQ::Network::NetworkSync _networkSync;
    netsync_status_t _networkSyncStatus;
    void updateStatus(netsync_status_t status) { _networkSyncStatus = status; }

    bool _networkSyncIsConnected;

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

    Coin::BloomFilter makeBloomFilter(double falsePositiveRate, uint32_t nTweak, uint32_t nFlags);

    void updateBloomFilter();

    void recalculateBalance();

    // Prefix methods only required from unit tests with test_
    void test_syncBlocksStaringAtHeight(int32_t height);

};

}
}
#endif // BITCOIN_SPVWALLET_HPP
