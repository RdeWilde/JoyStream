#include <bitcoin/SPVClient.hpp>

#include <logger/logger.h>

using namespace CoinQ;
using namespace joystream::bitcoin;

const std::string SPVClient::getStatusString(status_t status)
{
    switch (status)
    {
    case STOPPED:
        return "STOPPED";
    case STARTING:
        return "STARTING";
    case SYNCHING_HEADERS:
        return "SYNCHING_HEADERS";
    case SYNCHING_BLOCKS:
        return "SYNCHING_BLOCKS";
    case SYNCHED:
        return "SYNCHED";
    default:
        return "UNKNOWN";
    } 
}

// Constructor
SPVClient::SPVClient(const CoinQ::CoinParams& coinParams) :
    m_store(nullptr),
    m_status(STOPPED),
    m_bestHeight(0),
    m_syncHeight(0),
    m_filterFalsePositiveRate(0.001),
    m_filterTweak(0),
    m_filterFlags(0),
    m_networkSync(coinParams),
    m_bBlockTreeLoaded(false),
    m_bConnected(false),
    m_bSynching(false),
    m_bBlockTreeSynched(false),
    m_bGotMempool(false),
    m_bInsertMerkleBlocks(false)
{
    LOGGER(trace) << "SPVClient::SPVClient()" << std::endl;

    m_networkSync.subscribeStatus([this](const std::string& message)
    {
        LOGGER(trace) << "SPVClient - Status: " << message << std::endl;
    });

    m_networkSync.subscribeProtocolError([this](const std::string& error, int code)
    {
        LOGGER(trace) << "SPVClient - Protocol error: " << error << std::endl;
        emit ProtocolError(error, code);
    });

    m_networkSync.subscribeConnectionError([this](const std::string& error, int code)
    {
        LOGGER(trace) << "SPVClient - Connection error: " << error << std::endl;
        emit ConnectionError(error, code);
    });

    m_networkSync.subscribeBlockTreeError([this](const std::string& error, int code)
    {
        LOGGER(trace) << "SPVClient - Blocktree error: " << error << std::endl;
        emit BlockTreeError(error, code);
    });

    m_networkSync.subscribeOpen([this]()
    {
        LOGGER(trace) << "SPVClient - connection opened." << std::endl;
        m_bConnected = true;
        emit PeerConnected();
    });

    m_networkSync.subscribeClose([this]()
    {
        LOGGER(trace) << "SPVClient - connection closed." << std::endl;
        m_bConnected = false;
        m_bSynching = false;
        emit PeerDisconnected();
    });

    m_networkSync.subscribeStarted([this]()
    {
        LOGGER(trace) << "SPVClient - Sync started." << std::endl;
    });

    m_networkSync.subscribeStopped([this]()
    {
        LOGGER(trace) << "SPVClient - Sync stopped." << std::endl;
        updateStatus(STOPPED);
    });

    m_networkSync.subscribeTimeout([this]()
    {
        LOGGER(trace) << "SPVClient - Sync timeout." << std::endl;
        emit ConnectionError("Network timed out.", -1);
    });

    m_networkSync.subscribeSynchingHeaders([this]()
    {
        LOGGER(trace) << "SPVClient - Synching headers." << std::endl;
        updateStatus(SYNCHING_HEADERS);
    });

    m_networkSync.subscribeHeadersSynched([this]()
    {
        LOGGER(trace) << "SPVClient - Headers sync complete." << std::endl;
        m_bBlockTreeSynched = true;
        updateBestHeader(m_networkSync.getBestHeight(), m_networkSync.getBestHash());

        if (!m_networkSync.connected())
        {
            updateStatus(STOPPED);
        }
        else if (!m_store)
        {
            updateStatus(SYNCHED);
        }
        else
        {
            emit HeadersSynched(); // start block sync
        }
    });

    m_networkSync.subscribeSynchingBlocks([this]()
    {
        LOGGER(trace) << "SPVClient - Synching blocks." << std::endl;
        
        if (m_store) { updateStatus(SYNCHING_BLOCKS); }

        emit SynchingBlocks();
    });

    m_networkSync.subscribeBlocksSynched([this]()
    {
        LOGGER(trace) << "SPVClient - Block sync complete." << std::endl;

        if (m_networkSync.connected())
        {
            updateStatus(SYNCHED);

            emit BlocksSynched();

            if (m_store && !m_bGotMempool)
            {
                LOGGER(info) << "SPVClient - Fetching mempool." << std::endl;
                m_networkSync.getMempool();
                m_bGotMempool = true;
            }
        }
    });

    /* These two following events callbacks will never get emitted
     * because the code in netsync which subscribes to these events on the blockTree is commented out
     * Not sure why...?
        m_blockTree.subscribeRemoveBestChain([&](const ChainHeader& header)
        m_blockTree.subscribeAddBestChain([&](const ChainHeader& header)
    */
    /*
    m_networkSync.subscribeAddBestChain([this](const chain_header_t& header)
    {
        LOGGER(trace) << "SPVClient - Added best chain. New best height: " << header.height << std::endl;

        updateBestHeader(header.height, header.hash());
    });

    m_networkSync.subscribeRemoveBestChain([this](const chain_header_t& header)
    {
        LOGGER(trace) << "SPVClient - removed best chain." << std::endl;
        int diff = m_bestHeight - m_networkSync.getBestHeight();
        if (diff >= 0)
        {
            LOGGER(trace) << "Reorganizing " << (diff + 1) << " blocks." << std::endl;
            updateBestHeader(m_networkSync.getBestHeight(), m_networkSync.getBestHash());
        }
    });
    */

    m_networkSync.subscribeNewTx([this](const Coin::Transaction& cointx)
    {
        LOGGER(trace) << "SPVClient - Received new transaction " << cointx.hash().getHex() << std::endl;

        emit NewTx(cointx);
    });

    m_networkSync.subscribeMerkleTx([this](const ChainMerkleBlock& chainmerkleblock, const Coin::Transaction& cointx, unsigned int txindex, unsigned int txcount)
    {
        LOGGER(trace) << "SPVClient - Received merkle transaction " << cointx.hash().getHex() << " in block " << chainmerkleblock.hash().getHex() << std::endl;

       emit MerkleTx(chainmerkleblock, cointx, txindex, txcount);
    });

    m_networkSync.subscribeTxConfirmed([this](const ChainMerkleBlock& chainmerkleblock, const bytes_t& txhash, unsigned int txindex, unsigned int txcount)
    {
        LOGGER(trace) << "SPVClient - Received transaction confirmation " << uchar_vector(txhash).getHex() << " in block " << chainmerkleblock.hash().getHex() << std::endl;

        emit TxConfirmed(chainmerkleblock, txhash, txindex, txcount);
    });

    m_networkSync.subscribeMerkleBlock([this](const ChainMerkleBlock& chainMerkleBlock)
    {
        LOGGER(trace) << "SPVClient - received merkle block " << chainMerkleBlock.hash().getHex() << " height: " << chainMerkleBlock.height << std::endl;

        if (!m_bInsertMerkleBlocks) return;

        emit MerkleBlock(chainMerkleBlock);
    });

    m_networkSync.subscribeBlockTreeChanged([this]()
    {
        LOGGER(trace) << "SPVClient - block tree changed." << std::endl;

        m_bBlockTreeSynched = false;
        updateBestHeader(m_networkSync.getBestHeight(), m_networkSync.getBestHash());
        emit BlockTreeChanged();
    });
}

// Destructor
SPVClient::~SPVClient()
{
    LOGGER(trace) << "SPVClient::~SPVClient()" << std::endl;
    stopSync();
}

// Block tree operations
void SPVClient::loadHeaders(const std::string& blockTreeFile, bool bCheckProofOfWork, CoinQBlockTreeMem::callback_t callback)
{
    LOGGER(trace) << "SPVClient::loadHeaders(" << blockTreeFile << ", " << (bCheckProofOfWork ? "true" : "false") << ")" << std::endl;

    m_blockTreeFile = blockTreeFile;
    m_bBlockTreeLoaded = false;
    m_networkSync.loadHeaders(blockTreeFile, bCheckProofOfWork, callback);
    m_bBlockTreeLoaded = true;
}

// Store operations
void SPVClient::useStore(Store * store)
{
    {

        if(store->connected()){
            m_store = store;
        } else {
            throw;
        }

        updateSyncHeader(m_store->getBestBlockHeaderHeight(),
                         m_store->getBestBlockHeaderHash());

        /*
         * TODO connect signal from Store to handle two events: TxUpdated and MerkleBlockInserted
         *
        m_store->subscribeTxUpdated([this](std::shared_ptr<Tx> tx)
        {
            if (tx->status() == Tx::PROPAGATED) { m_networkSync.addToMempool(tx->hash()); }
        });

        m_store->subscribeMerkleBlockInserted([this](std::shared_ptr<MerkleBlock> merkleblock)
        {
            updateSyncHeader(merkleblock->blockheader()->height(), merkleblock->blockheader()->hash());
        });
        */
    }

    emit StoreOpened(m_store);
    if (m_networkSync.connected() && m_networkSync.headersSynched()) { syncBlocks(); }
}

void SPVClient::closeStore()
{
    LOGGER(trace) << "SPVClient::closeStore()" << std::endl;

    {
        if (!m_store) return;

        m_bInsertMerkleBlocks = false;
        m_networkSync.stopSynchingBlocks();
        m_store = nullptr;
    }

    emit StoreClosed();
    updateSyncHeader(0, bytes_t());
    if (m_networkSync.connected() && m_networkSync.headersSynched()) { updateStatus(SYNCHED); }
}

// Peer to peer network operations
void SPVClient::startSync(const std::string& host, const std::string& port)
{
    LOGGER(trace) << "SPVClient::startSync(" << host << ", " << port << ")" << std::endl;
    m_bInsertMerkleBlocks = false;
    updateStatus(STARTING);
    m_networkSync.start(host, port); 
}

void SPVClient::startSync(const std::string& host, int port)
{
    std::stringstream ss;
    ss << port;
    startSync(host, ss.str());
}

void SPVClient::stopSync()
{
    LOGGER(trace) << "SPVClient::stopSync()" << std::endl;
    m_networkSync.stop();
}

//TODO: get rid of m_bInsertMerkleBlocks
void SPVClient::suspendBlockUpdates()
{
    LOGGER(trace) << "SPVClient::suspendBlockUpdates()" << std::endl;

    if (!m_store) return;
    if (!m_bInsertMerkleBlocks) return;
    m_bInsertMerkleBlocks = false;
}

void SPVClient::syncBlocks()
{
    LOGGER(trace) << "SPVClient::syncBlocks()" << std::endl;

    if (!m_bConnected) throw std::runtime_error("Not connected.");

    if (!m_store) throw std::runtime_error("No Store is open.");

    uint32_t startTime = m_store->created(); //m_vault->getMaxFirstBlockTimestamp()

    if (startTime == 0)
    {
        m_bInsertMerkleBlocks = false;
        return;
    }

    m_networkSync.setBloomFilter(m_store->getBloomFilter(m_filterFalsePositiveRate, m_filterTweak, m_filterFlags));

    std::vector<bytes_t> locatorHashes = m_store->getLocatorHashes();
    m_bGotMempool = false;
    m_bInsertMerkleBlocks = true;
    m_networkSync.syncBlocks(locatorHashes, startTime);
    //m_networkSync.syncBlocks(718000);
}

void SPVClient::setFilterParams(double falsePositiveRate, uint32_t nTweak, uint8_t nFlags)
{
    m_filterFalsePositiveRate = falsePositiveRate;
    m_filterTweak = nTweak;
    m_filterFlags = nFlags;
}

void SPVClient::updateBloomFilter()
{
    LOGGER(trace) << "SPVClient::updateBloomFilter()" << std::endl;

    if (!m_store) throw std::runtime_error("No Store is open.");
    std::lock_guard<std::mutex> lock(m_storeMutex);
    if (!m_store) throw std::runtime_error("No Store is open.");

    m_networkSync.setBloomFilter(m_store->getBloomFilter(m_filterFalsePositiveRate, m_filterTweak, m_filterFlags));
}

void SPVClient::sendTx(Coin::Transaction& coin_tx)
{
    uchar_vector hash = coin_tx.hash();
    LOGGER(trace) << "SPVClient::sendTx(" << hash.getHex() << ")" << std::endl;
    if (!m_bConnected) throw std::runtime_error("Not connected.");

    m_networkSync.sendTx(coin_tx);
    m_networkSync.getTx(hash);
}

void SPVClient::updateStatus(status_t newStatus)
{
    if (m_status != newStatus)
    {
        m_status = newStatus;
        emit StatusChanged(newStatus);
    }
}

void SPVClient::updateBestHeader(uint32_t bestHeight, const bytes_t& bestHash)
{
    if (m_bestHeight != bestHeight || m_bestHash != bestHash)
    {
        m_bestHeight = bestHeight;
        m_bestHash = bestHash;
    }
}

void SPVClient::updateSyncHeader(uint32_t syncHeight, const bytes_t& syncHash)
{
    if (m_syncHeight != syncHeight || m_syncHash != syncHash)
    {
        m_syncHeight = syncHeight;
        m_syncHash = syncHash;
    }
}
