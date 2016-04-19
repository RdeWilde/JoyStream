#include <common/UnspentP2PKHOutput.hpp>
#include <common/P2PKHAddress.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <common/P2PKHScriptSig.hpp>
#include <common/TransactionSignature.hpp>
#include <common/Utilities.hpp>

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
  _networkSync(getCoinParamsForNetwork(network), true),
  _walletStatus(wallet_status_t::UNINITIALIZED),
  _blockTreeLoaded(false),
  _blockTreeFile(blockTreeFile),
  _unconfirmedBalance(0),
  _confirmedBalance(0)
{
    // == Important == Make sure to catch all exceptions in callbacks
    // failing to do so might cause undefined behaviour in netsync

    _networkSync.subscribeStatus([this](const std::string& message)
    {
        emit statusMessageUpdated(message);
    });

    _networkSync.subscribeStarted([this]()
    {
        updateStatus(wallet_status_t::CONNECTING);
    });

    _networkSync.subscribeOpen([this]()
    {
        emit connected();
    });

    _networkSync.subscribeProtocolError([this](const std::string& error, int code)
    {
        emit protocolError(error);
    });

    _networkSync.subscribeConnectionError([this](const std::string& error, int code)
    {
        emit connectionError(error);
    });

    _networkSync.subscribeClose([this]()
    {
        emit disconnected();
    });

    _networkSync.subscribeTimeout([this]()
    {
        emit connectionTimedOut();
    });

    _networkSync.subscribeStopped([this]()
    {
        updateStatus(wallet_status_t::OFFLINE);
    });

    _networkSync.subscribeBlockTreeError([this](const std::string& error, int code) {
        onBlockTreeError(error, code);
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
        onBlockTreeChanged();
    });

    _store.setTxUpdatedCallback([this](Coin::TransactionId txid, int confirmations) {
       emit txUpdated(txid, confirmations);
    });
}

void SPVWallet::create() {

    if(isInitialized()) {
        throw std::runtime_error("wallet already opened");
    }

    if(!_store.create(_storePath, _network)){
        throw std::runtime_error("unable to create store");
    }

    updateStatus(wallet_status_t::OFFLINE);
}

void SPVWallet::create(Coin::Seed seed, uint32_t timestamp) {

    if(isInitialized()) {
        throw std::runtime_error("wallet already opened");
    }

    if(!_store.create(_storePath, _network, seed, timestamp == 0 ? std::time(nullptr) : timestamp)){
        throw std::runtime_error("unable to create store");
    }

    updateStatus(wallet_status_t::OFFLINE);
}

void SPVWallet::open() {

    // Only open the store once
    if(!isInitialized()) {
        if(!_store.open(_storePath)) {
            throw std::runtime_error("failed to open wallet");
        }

        if(_network != _store.network()){
            throw std::runtime_error("store network type mistmatch");
        }

        updateStatus(wallet_status_t::OFFLINE);

        recalculateBalance();

        updateBloomFilter(_store.listPrivateKeys());

    } else {
        throw std::runtime_error("wallet already opened");
    }
}

void SPVWallet::loadBlockTree(std::function<void(std::string)> feedback) {
    if(_blockTreeLoaded) return;

    if (boost::filesystem::exists(_blockTreeFile+".swp")) {
        boost::filesystem::remove(_blockTreeFile+".swp") ;
    }

    _networkSync.loadHeaders(_blockTreeFile, true, [this, &feedback](const CoinQBlockTreeMem& blockTree) {
        if(feedback) {
            std::stringstream progress;
            progress << "Loading Headers: " << blockTree.getBestHeight();
            feedback(progress.str());
        }
        return true;
    });

    _blockTreeLoaded = true;
}

void SPVWallet::sync(std::string host, int port) {

    // Wallet must be opened or created before synching
    if(!isInitialized()) {
        throw std::runtime_error("wallet not initialized");
    }

    // Only start synching from offline state
    if(!isOffline()) {
        return;
    }

    Q_ASSERT(_store.connected());

    if(!_blockTreeLoaded) {
        loadBlockTree();
    }

    _networkSync.start(host, port);
}

void SPVWallet::stopSync() {
    _networkSync.stop();
}

Coin::PrivateKey SPVWallet::getKey(bool createReceiveAddress) {
    if(!isInitialized()) {
        throw std::runtime_error("wallet not initialized");
    }

    Coin::PrivateKey sk = _store.getKey(createReceiveAddress);

    if(createReceiveAddress) {
        updateBloomFilter({sk});
    }

    return sk;
}

std::vector<Coin::PrivateKey> SPVWallet::getKeys(uint32_t numKeys, bool createReceiveAddress) {
    if(!isInitialized()) {
        throw std::runtime_error("wallet not initialized");
    }

    std::vector<Coin::PrivateKey> keys = _store.getKeys(numKeys, createReceiveAddress);
    if(createReceiveAddress) {
        updateBloomFilter(keys);
    }
    return keys;
}

std::vector<Coin::KeyPair>
SPVWallet::getKeyPairs(uint32_t num_pairs, bool createReceiveAddress) {
    if(!isInitialized()) {
        throw std::runtime_error("wallet not initialized");
    }

    std::vector<Coin::KeyPair> keyPairs = _store.getKeyPairs(num_pairs, createReceiveAddress);
    if (createReceiveAddress) {
        std::vector<Coin::PrivateKey> keys;

        for(auto &pair : keyPairs) {
            keys.push_back(pair.sk());
        }

        updateBloomFilter(keys);
    }
    return keyPairs;
}

void SPVWallet::releaseKey(const Coin::PrivateKey &sk) {
    if(!isInitialized()) {
        throw std::runtime_error("wallet not initialized");
    }

    _store.releaseKey(sk);
}

std::list<Coin::P2PKHAddress> SPVWallet::listAddresses() {
    return _store.listReceiveAddresses();
}

Coin::P2PKHAddress
SPVWallet::getReceiveAddress()
{
    if(!isInitialized()) {
        throw std::runtime_error("wallet not initialized");
    }

    Coin::P2PKHAddress addr = getKey(true).toPublicKey().toP2PKHAddress(_network);

    return addr;
}


void SPVWallet::broadcastTx(Coin::Transaction cointx) {
    if(!isConnected()) {
        throw std::runtime_error("cannot broadcast tx, wallet offline");
    }

    //_store.addTransaction(tx);
    _networkSync.sendTx(cointx);

    // add to the store when received from the network
    _networkSync.getTx(cointx.hash());
}

int32_t SPVWallet::bestHeight() const {
    if(!isInitialized()) {
        throw std::runtime_error("wallet not initialized");
    }

    return _store.getBestHeaderHeight();
}

std::list<Coin::UnspentP2PKHOutput> SPVWallet::lockOutputs(uint64_t minValue, uint32_t minimalConfirmations) {
    if(!isInitialized()) {
        throw std::runtime_error("wallet not initialized");
    }
    
    std::lock_guard<std::mutex> lock(_utxoMutex);

    uint32_t totalValue = 0;

    std::list<Coin::UnspentP2PKHOutput> selectedOutputs;

    // Assume outputs are sorted in descending value
    std::list<Coin::UnspentP2PKHOutput> unspentOutputs(_store.getUnspentTransactionsOutputs(minimalConfirmations, bestHeight()));

    for(Coin::UnspentP2PKHOutput & utxo : unspentOutputs) {
        if(_lockedOutpoints.find(utxo.outPoint()) != _lockedOutpoints.end()) continue;

        selectedOutputs.push_back(utxo);
        totalValue += utxo.value();

        if(totalValue >= minValue) {
            break;
        }
    }

    if(totalValue < minValue) {
        // not enough utxo
        return std::list<Coin::UnspentP2PKHOutput>();
    }

    // Lock and return the selected utxos
    for(auto utxo : selectedOutputs) {
        _lockedOutpoints.insert(utxo.outPoint());
    }

    return selectedOutputs;
}

void SPVWallet::unlockOutputs(const std::list<Coin::UnspentP2PKHOutput> outputs) {
    std::lock_guard<std::mutex> lock(_utxoMutex);

    for(auto utxo : outputs) {
        _lockedOutpoints.erase(utxo.outPoint());
    }
}

uint64_t SPVWallet::balance() const {
    return _confirmedBalance;
}

uint64_t SPVWallet::unconfirmedBalance() const {
    return _unconfirmedBalance;
}

void SPVWallet::updateStatus(wallet_status_t status) {
    if(_walletStatus != status) {
        _walletStatus = status;

        emit statusChanged(status);
    }

    switch(status) {
        case wallet_status_t::OFFLINE: emit offline(); break;
        case wallet_status_t::CONNECTING: emit connecting(); break;
        case wallet_status_t::SYNCHING_HEADERS: emit synchingHeaders(); break;
        case wallet_status_t::SYNCHING_BLOCKS: emit synchingBlocks(); break;
        case wallet_status_t::SYNCHED: emit synched(); break;
        default:
            Q_ASSERT(false);
    }

}

void SPVWallet::onBlockTreeError(const std::string& error, int code) {
    if(_walletStatus > wallet_status_t::UNINITIALIZED) {

        if(code == CoinQ::ErrorCodes::BLOCKTREE_FILE_WRITE_FAILURE) {
            emit blockTreeWriteFailed(error);
        } else {
            // Block Tree error while connected to a peer and updating our block tree
            // also as a result of failure to flush blocktree
            emit blockTreeUpdateFailed(error);
        }
    } else {

        // NetSync was not able to load the blocktree, and will just use a new one
        // If the blocktree was corrupt (most likely due to excessive flushing)
        // we are also able to recover and build a new one
    }
}

void SPVWallet::onBlockTreeChanged() {

}

void SPVWallet::onSynchingHeaders() {
    updateStatus(wallet_status_t::SYNCHING_HEADERS);
}

void SPVWallet::onHeadersSynched() {

    uint32_t startTime = _store.created();

    std::vector<bytes_t> locatorHashes;

    std::vector<std::string> hashes = _store.getLatestBlockHeaderHashes();

    if(hashes.size() > 1) {
        std::vector<std::string>::iterator i = hashes.begin();
        i++; // skip first so we can resync last block
        for(;i != hashes.end(); i++){
            locatorHashes.push_back(uchar_vector(*i));
        }
    }

    _networkSync.syncBlocks(locatorHashes, startTime);
}

void SPVWallet::onSynchingBlocks() {
    updateStatus(wallet_status_t::SYNCHING_BLOCKS);
}

void SPVWallet::onBlocksSynched() {

    recalculateBalance();

    updateStatus(wallet_status_t::SYNCHED);

    _networkSync.getMempool();
}

// This occurs when a new unconfirmed transaction arrives from the peer.
// We may very well already have it in our store from a previous session
void SPVWallet::onNewTx(const Coin::Transaction& cointx) {
    if(!transactionShouldBeStored(cointx)) return;

    try {

        _store.addTransaction(cointx);

    } catch(const std::exception & e) {
        emit storeUpdateFailed(e.what());
        _networkSync.stop();
    }

    recalculateBalance();
}

// This will be called when the blocks which mines a transaction in the mempool is received
// After a reorg if we do not return the transaction hash to netsync mempool, and the transaction
// is mined into another block this method will not be called (onMerkleTx will be called instead)
void SPVWallet::onTxConfirmed(const ChainMerkleBlock& chainmerkleblock, const bytes_t& txhash, unsigned int txindex, unsigned int txcount){
    try {

        if(txindex == 0) {
            _store.addBlockHeader(chainmerkleblock);
        }

        Coin::TransactionId txid(Coin::TransactionId::fromRPCByteOrder(txhash));

        if(!_store.transactionExists(txid)) return;

        _store.confirmTransaction(txid, chainmerkleblock);

    } catch(const std::exception & e) {
        emit storeUpdateFailed(e.what());
        _networkSync.stop();
    }
}

// This method will get called for each transaction when a merkleblock is received
// except for transactions which were in the mempool
void SPVWallet::onMerkleTx(const ChainMerkleBlock& chainmerkleblock, const Coin::Transaction& cointx, unsigned int txindex, unsigned int txcount){
    try {

        if( txindex == 0 ) {
            _store.addBlockHeader(chainmerkleblock);
        }

        if(transactionShouldBeStored(cointx)) {
            _store.addTransaction(cointx, chainmerkleblock);
        }

    } catch(const std::exception & e) {
        emit storeUpdateFailed(e.what());
        _networkSync.stop();
    }
}

// On receiving a merkle block without any transactions that match the bloom filter
void SPVWallet::onMerkleBlock(const ChainMerkleBlock& chainmerkleblock) {
    try {

        _store.addBlockHeader(chainmerkleblock);

    } catch(const std::exception & e) {
        emit storeUpdateFailed(e.what());
        _networkSync.stop();
    }
}

// call this first time we open the store with argument
// std::list<Coin::PrivateKey> privateKeys = _store.listPrivateKeys();

void SPVWallet::updateBloomFilter(const std::vector<Coin::PrivateKey> & privateKeys) {

    int currentElementsCount = _bloomFilterCompressedPubKeys.size() + _bloomFilterPubKeyHashes.size();

    for(auto &sk : privateKeys) {
        //public key - to capture inputs that spend outputs we control
        _bloomFilterCompressedPubKeys.insert(_bloomFilterCompressedPubKeys.begin(),
                                             sk.toPublicKey().toUCharVector());

        //pubkeyhash - to capture outputs we control
        _bloomFilterPubKeyHashes.insert(_bloomFilterPubKeyHashes.begin(),
                                        sk.toPublicKey().toPubKeyHash().toUCharVector());
    }

    int newElementsCount = _bloomFilterCompressedPubKeys.size() + _bloomFilterPubKeyHashes.size();

    if(newElementsCount > currentElementsCount) {
        Coin::BloomFilter filter(newElementsCount, 0.0001, 0,0);

        for(auto elm : _bloomFilterCompressedPubKeys) {
            filter.insert(elm);
        }

        for(auto elm : _bloomFilterPubKeyHashes) {
            filter.insert(elm);
        }

        _networkSync.setBloomFilter(filter);
    }

}

bool SPVWallet::transactionShouldBeStored(const Coin::Transaction & cointx) const {
    for(const Coin::TxIn & txin : cointx.inputs) {
        if(spendsWalletOutput(txin)) return true;
    }

    for(const Coin::TxOut & txout : cointx.outputs) {
        if(createsWalletOutput(txout)) return true;
    }

    return false;
}

bool SPVWallet::spendsWalletOutput(const Coin::TxIn & txin) const {
    try{
        uchar_vector pubkey = Coin::P2PKHScriptSig::deserialize(txin.scriptSig).pk().toUCharVector();

        if(_bloomFilterCompressedPubKeys.find(pubkey) != _bloomFilterCompressedPubKeys.end()) {
            return true;
        }

    } catch(std::runtime_error &e) {
        // not a p2pkh script sig
    }

    return false;
}

bool SPVWallet::createsWalletOutput(const Coin::TxOut & txout) const {
    try{
        // get the pubkeyhash
        Coin::P2PKHScriptPubKey script = Coin::P2PKHScriptPubKey::deserialize(txout.scriptPubKey);

        // if it matches bloom filter element we have a matching transaction
        if(_bloomFilterPubKeyHashes.find(script.pubKeyHash().toUCharVector()) != _bloomFilterPubKeyHashes.end()) {
            return true;
        }

    } catch(std::runtime_error &e) {
        // not a p2pkh pubkey script
    }

    return false;
}

void SPVWallet::recalculateBalance() {
    if(!isInitialized()) {
        return;
    }

    uint64_t confirmed = 0;

    if(_store.getBestHeaderHeight() != 0) {
        confirmed = _store.getWalletBalance(1, _store.getBestHeaderHeight());
    }

    uint64_t unconfirmed = _store.getWalletBalance();

    if(_confirmedBalance != confirmed || _unconfirmedBalance != unconfirmed) {
        _confirmedBalance = confirmed;
        _unconfirmedBalance = unconfirmed;
        emit balanceChanged(confirmed, unconfirmed);
    }
}

void SPVWallet::test_syncBlocksStaringAtHeight(int32_t height) {
    _networkSync.syncBlocks(height);
}

Coin::Transaction SPVWallet::test_sendToAddress(uint64_t value, const Coin::P2PKHAddress &addr, uint64_t fee) {

    // Get UnspentUTXO
    std::list<Coin::UnspentP2PKHOutput> utxos(lockOutputs(value + fee, 0));

    if(utxos.empty()) {
        throw std::runtime_error("Not enough funds");
    }

    Coin::UnspentP2PKHOutput utxo = utxos.front();

    if(utxo.value() < value) {
        throw std::runtime_error("Failed to get one UTXO with required value");
    }

    // Create Destination output
    Coin::P2PKHScriptPubKey destinationScript(addr.pubKeyHash());

    // Create Change output
    Coin::P2PKHAddress changeAddr = getReceiveAddress();
    Coin::P2PKHScriptPubKey changeScript(changeAddr.pubKeyHash());

    // Create an unsigned Transaction
    Coin::Transaction cointx;
    cointx.addOutput(Coin::TxOut(value, destinationScript.serialize()));

    uint64_t change = utxo.value() - (value + fee);
    if(change > 0) {
        cointx.addOutput(Coin::TxOut(change, changeScript.serialize()));
    }

    // Set Input
    cointx.addInput(Coin::TxIn(utxo.outPoint().getClassicOutPoint(), uchar_vector(), 0xFFFFFFFF));

    // Sign the input
    Coin::setScriptSigToSpendP2PKH(cointx, 0, utxo.keyPair().sk());

    broadcastTx(cointx);

    return cointx;
}

}
}
