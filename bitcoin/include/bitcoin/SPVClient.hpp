#pragma once

#include <bitcoin/Store.hpp>
#include <CoinQ/CoinQ_netsync.h>

namespace joystream {
namespace bitcoin {

class SPVClient : QObject
{
    Q_OBJECT

public:
    enum status_t
    {
        STOPPED,
        STARTING,
        SYNCHING_HEADERS,
        SYNCHING_BLOCKS,
        SYNCHED
    };

    static const std::string getStatusString(status_t status);

    SPVClient(const CoinQ::CoinParams& coinParams = CoinQ::getBitcoinParams());
    ~SPVClient();

    const CoinQ::CoinParams& getCoinParams() const { return m_networkSync.getCoinParams(); }

    void loadHeaders(const std::string& blockTreeFile, bool bCheckProofOfWork = false, CoinQBlockTreeMem::callback_t callback = nullptr);
    bool areHeadersLoaded() const { return m_bBlockTreeLoaded; }

    void startSync(const std::string& host, const std::string& port);
    void startSync(const std::string& host, int port);
    void stopSync();
    bool isConnected() const { return m_networkSync.connected(); }

    void suspendBlockUpdates();
    void syncBlocks();

    void setFilterParams(double falsePositiveRate, uint32_t nTweak, uint8_t nFlags);
    void updateBloomFilter();

    status_t getStatus() const { return m_status; }
    uint32_t getBestHeight() const { return m_bestHeight; }
    const bytes_t& getBestHash() const { return m_bestHash; }
    uint32_t getSyncHeight() const { return m_syncHeight; }
    const bytes_t& getSyncHash() const { return m_syncHash; }

    //std::shared_ptr<Tx> sendTx(const bytes_t& hash);
    //std::shared_ptr<Tx> sendTx(unsigned long tx_id);
    void sendTx(Coin::Transaction& coin_tx);

signals:
    void StoreError(const char *);
    void StoreClosed();
    void StoreOpened(Store *);
    void StatusChanged(status_t);
    void ProtocolError(const std::string& error, int code);
    void ConnectionError(const std::string& error, int code);
    void BlockTreeError(const std::string& error, int code);
    void PeerConnected();
    void PeerDisconnected();
//private signals?
    void HeadersSynched();
    void SynchingHeaders();
    void SynchingBlocks();
    void BlocksSynched();
    void NewTx(const Coin::Transaction & tx);
    void MerkleTx(const ChainMerkleBlock& chainmerkleblock, const Coin::Transaction& cointx, unsigned int txindex, unsigned int txcount);
    void TxConfirmed(const ChainMerkleBlock& chainmerkleblock, const bytes_t& txhash, unsigned int txindex, unsigned int txcount);
    void MerkleBlock(const ChainMerkleBlock& chainMerkleBlock);
    void BlockTreeChanged();

private:

    status_t                    m_status;
    void                        updateStatus(status_t newStatus);

    // Best known chain
    uint32_t                    m_bestHeight;
    bytes_t                     m_bestHash;
    void                        updateBestHeader(uint32_t bestHeight, const bytes_t& bestHash);

    // Most recent block stored in Store
    uint32_t                    m_syncHeight;
    bytes_t                     m_syncHash;
    void                        updateSyncHeader(uint32_t syncHeight, const bytes_t& syncHash);

    // Bloom filter parameters
    double                      m_filterFalsePositiveRate;
    uint32_t                    m_filterTweak;
    uint8_t                     m_filterFlags;

    CoinQ::Network::NetworkSync m_networkSync;
    std::string                 m_blockTreeFile;
    bool                        m_bBlockTreeLoaded;
    bool                        m_bConnected;
    bool                        m_bSynching;
    bool                        m_bBlockTreeSynched;
    bool                        m_bGotMempool;

    bool                        m_bInsertMerkleBlocks;
};

} // bitcoin
} // joystream
