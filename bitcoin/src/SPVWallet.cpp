#include <common/UnspentP2PKHOutput.hpp>
#include <common/P2PKHAddress.hpp>
#include <bitcoin/SPVWallet.hpp>

namespace joystream {
namespace bitcoin {

const CoinQ::CoinParams regtestParams(
        0xDAB5BFFA, 70001, "18444", 0x6f, 0xc4, "regtest", "regtest", 100000000, "testBTC", 21000000, 0,
        &sha256_2, &sha256_2,
        Coin::CoinBlockHeader(1, 1296688602, 0x207fffff, 2, uchar_vector(32, 0), uchar_vector("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")));

const CoinQ::CoinParams getCoinParamsForNetwork(Coin::Network network) {

    if(network == Coin::Network::mainnet) {
        return CoinQ::getBitcoinParams();

    } else if(network == Coin::Network::testnet3) {
        return CoinQ::getTestnet3Params();

    }else if(network == Coin::Network::regtest){
        return regtestParams;
    }

    throw std::runtime_error("network not supported");
}

SPVWallet::SPVWallet(std::string storePath, std::string blockTreeFile, Coin::Network network) :
  _storePath(storePath),
  _network(network),
  _networkSync(getCoinParamsForNetwork(network)),
  _blockTreeLoaded(false),
  _unconfirmedBalance(0),
  _confirmedBalance(0)
{
    _networkSync.subscribeStatus([this](const std::string& message)
    {

    });

    _networkSync.subscribeProtocolError([this](const std::string& error, int code)
    {

    });

    _networkSync.subscribeConnectionError([this](const std::string& error, int code)
    {

    });

    _networkSync.subscribeBlockTreeError([this](const std::string& error, int code) { onBlockTreeError(error, code); });

    _networkSync.subscribeOpen([this]()
    {

    });

    _networkSync.subscribeClose([this]()
    {

    });

    _networkSync.subscribeStarted([this]()
    {

    });

    _networkSync.subscribeStopped([this]()
    {

    });

    _networkSync.subscribeTimeout([this]()
    {

    });

    _networkSync.subscribeSynchingHeaders([this](){ onSynchingHeaders(); });
    _networkSync.subscribeHeadersSynched([this](){ onHeadersSynched(); });
    _networkSync.subscribeSynchingBlocks([this](){ onSynchingBlocks(); });
    _networkSync.subscribeBlocksSynched([this](){ onBlocksSynched(); });

    _networkSync.subscribeNewTx([this](const Coin::Transaction& cointx)
    {
        onNewTx(cointx);
    });

    _networkSync.subscribeMerkleTx([this](const ChainMerkleBlock& chainmerkleblock, const Coin::Transaction& cointx, unsigned int txindex, unsigned int txcount)
    {
        onMerkleTx(chainmerkleblock, cointx, txindex, txcount);
    });

    _networkSync.subscribeTxConfirmed([this](const ChainMerkleBlock& chainmerkleblock, const bytes_t& txhash, unsigned int txindex, unsigned int txcount)
    {
        onTxConfirmed(chainmerkleblock, txhash, txindex, txcount);
    });

    _networkSync.subscribeMerkleBlock([this](const ChainMerkleBlock& chainmerkleblock)
    {
        onMerkleBlock(chainmerkleblock);
    });

    _networkSync.subscribeBlockTreeChanged([this]()
    {
        emit BlockTreeChanged();
    });

    _blockTreeFile = blockTreeFile;

    // NOTE: Loading the blocktree could have gone here but we would miss the signals
    // because it is a synchronous call.
}

void SPVWallet::Create() {
    if(!_store.create(_storePath, _network)){
        throw std::runtime_error("unable to create store");
    }
}

void SPVWallet::Create(Coin::Seed seed) {
    if(!_store.create(_storePath, _network, seed, std::time(nullptr))){
        throw std::runtime_error("unable to create store");
    }
}

void SPVWallet::Open() {

    // Only open the store once
    if(!_store.connected()) {
        if(!_store.open(_storePath)) {
            throw std::runtime_error("failed to open wallet");
        }

        if(_network != _store.network()){
            throw std::runtime_error("store network type mistmatch");
        }
    }

    //TODO: get best synched height

    recalculateBalance();
}

void SPVWallet::LoadBlockTree() {
    // Create block tree file or load if it exists
    // If there is a problem reading/creating the file netsync will emit the BlockTreeError signal
    // but netsync can continue to function without a persisted blocktree file,
    // however all headers from genesis block will be resynched!

    // Only load the blocktree once
    if(_blockTreeLoaded) return;

    _networkSync.loadHeaders(_blockTreeFile);
    _blockTreeLoaded = true;
}

void SPVWallet::Sync(std::string host, int port) {
    _networkSync.start(host, port);
}

void SPVWallet::StopSync() {
    _networkSync.stopSynchingBlocks();
    _networkSync.stop();
}

Coin::PrivateKey SPVWallet::GetKey(bool createReceiveAddress) {
    Coin::PrivateKey sk = _store.getKey(createReceiveAddress);

    if(createReceiveAddress) {
        updateBloomFilter();
    }

    return sk;
}

std::vector<Coin::PrivateKey> SPVWallet::GetKeys(uint32_t numKeys, bool createReceiveAddress) {
    std::vector<Coin::PrivateKey> keys = _store.getKeys(numKeys, createReceiveAddress);
    if(createReceiveAddress) {
        updateBloomFilter();
    }
    return keys;
}

std::vector<Coin::KeyPair>
SPVWallet::GetKeyPairs(uint32_t num_pairs, bool createReceiveAddress) {
    std::vector<Coin::KeyPair> keyPairs = _store.getKeyPairs(num_pairs, createReceiveAddress);
    if (createReceiveAddress) {
        updateBloomFilter();
    }
    return keyPairs;
}

void SPVWallet::ReleaseKey(const Coin::PrivateKey &sk) {
    _store.releaseKey(sk);
}

Coin::P2PKHAddress
SPVWallet::GetReceiveAddress()
{
    Coin::P2PKHAddress addr = _store.getReceiveAddress();
    updateBloomFilter();
    return addr;
}


void SPVWallet::BroadcastTx(Coin::Transaction & tx) {
    _store.addTransaction(tx);
    _networkSync.sendTx(tx);
}

uint64_t SPVWallet::Balance() const {
    return _confirmedBalance;
}

uint64_t SPVWallet::UnconfirmedBalance() const {
    return _unconfirmedBalance;
}


void SPVWallet::onBlockTreeError(const std::string& error, int code) {
    // Ignore file not found error - not critical
    if(error == "Blocktree file not found.") return;

    emit BlockTreeError();
}

void SPVWallet::onSynchingHeaders() {
    emit SynchingHeaders();
}

void SPVWallet::onHeadersSynched() {
    emit HeadersSynched();

    uint32_t startTime = _store.created();

    std::vector<std::string> hashes = _store.getLatestBlockHeaderHashes();
    std::vector<bytes_t> locatorHashes;

    for(const std::string hex : hashes){
        locatorHashes.push_back(uchar_vector(hex));
    }

    updateBloomFilter();
    _networkSync.syncBlocks(locatorHashes, startTime);
}

void SPVWallet::onSynchingBlocks() {
    emit SynchingBlocks();
}

void SPVWallet::onBlocksSynched() {
    _networkSync.getMempool();
    // TODO: add unconfirmed Tx from store to netsync mempool
    recalculateBalance();
    emit BlocksSynched();
}

void SPVWallet::onNewTx(const Coin::Transaction& cointx) {
    // TODO: match outputs we control and inputs that spend our outputs
    _store.addTransaction(cointx);
    recalculateBalance();
}

void SPVWallet::onTxConfirmed(const ChainMerkleBlock& chainmerkleblock, const bytes_t& txhash, unsigned int txindex, unsigned int txcount){
    _store.confirmTransaction(uchar_vector(txhash).getHex(), chainmerkleblock, txindex == 0);

    //if(txindex + 1 == txcount) // when last tx in the block is processed
        recalculateBalance();
}

void SPVWallet::onMerkleTx(const ChainMerkleBlock& chainmerkleblock, const Coin::Transaction& cointx, unsigned int txindex, unsigned int txcount){
    //std::cout << "onMerkleTx() - " << cointx.hash().getHex() << std::endl;

    // Mined Block containing transactions we might care about
    _store.addTransaction(cointx, chainmerkleblock, txindex == 0);

    // TODO: if netsync status is synched && if(txindex + 1 == txcount)
    recalculateBalance();
}

void SPVWallet::onMerkleBlock(const ChainMerkleBlock& chainmerkleblock) {
    //std::cout << "onMerkleBlock() - " << chainmerkleblock.hash().getHex() << std::endl;
    // Block without tx we care about
    _store.addBlockHeader(chainmerkleblock);

    // TODO: if netsync status is synched
    recalculateBalance();
}

void SPVWallet::updateBloomFilter() {
    // TODO: Only update the bloom filter if elements have changed from last call
    _networkSync.setBloomFilter(makeBloomFilter(0.001, 0, 0));
}

Coin::BloomFilter SPVWallet::makeBloomFilter(double falsePositiveRate, uint32_t nTweak, uint32_t nFlags) {
    // TODO: Keep track of elements being added - to acheive two purposes
    // 1. Can be used to filter incoming false positive transactions
    // 2. Only update the bloom filter if we are adding new elements

    // Side Note: There is a limit on the maximum size of the bloom filter
    // We can keep out keys of addresses we know have been spent and are certain
    // would never be used again by a human (keys associated with joystream p2p protocol)
    std::vector<uchar_vector> elements;

    std::list<Coin::PrivateKey> privateKeys = _store.listPrivateKeys();

    for(auto &sk : privateKeys) {
        //public key - to capture inputs that spend outputs we control
        elements.push_back(sk.toPublicKey().toUCharVector());

        //pubkeyhash - to capture outputs we control
        elements.push_back(sk.toPublicKey().toPubKeyHash().toUCharVector());
    }

    if(elements.size() == 0) return Coin::BloomFilter();

    Coin::BloomFilter filter(elements.size(), falsePositiveRate, nTweak, nFlags);

    for(auto elm : elements) filter.insert(elm);

    return filter;
}

void SPVWallet::recalculateBalance() {
    if(!_store.connected()) return;

    uint64_t confirmed = _store.getWalletBalance(1,_networkSync.getBestHeight());
    uint64_t unconfirmed = _store.getWalletBalance();

    if(_confirmedBalance != confirmed || _unconfirmedBalance != unconfirmed) {
        _confirmedBalance = confirmed;
        _unconfirmedBalance = unconfirmed;
        emit BalanceChanged(confirmed, unconfirmed);
    }
}

}
}
