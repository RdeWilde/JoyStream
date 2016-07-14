#include <common/P2PKHAddress.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <common/P2PKHScriptSig.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <common/TransactionSignature.hpp>
#include <common/Utilities.hpp>
#include <common/SigHashType.hpp>
#include <common/UnspentOutputSet.hpp>
#include <common/UnspentOutputSet.hpp>

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

void SPVWallet::create(uchar_vector entropy, uint32_t timestamp) {

    if(isInitialized()) {
        throw std::runtime_error("wallet already opened");
    }

    if(!_store.create(_storePath, _network, entropy, timestamp == 0 ? std::time(nullptr) : timestamp)){
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

        updateBloomFilter(_store.listRedeemScripts());

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

void SPVWallet::sync(std::string host, int port, unsigned int timeout) {

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

    _networkSync.start(host, port, timeout);
}

void SPVWallet::stopSync() {
    _networkSync.stop();
}

Coin::PrivateKey SPVWallet::generateKey(const RedeemScriptGenerator & scriptGenerator) {
    if(!isInitialized()) {
        throw std::runtime_error("wallet not initialized");
    }

    uchar_vector script;

    Coin::PrivateKey sk = _store.generateKey([&scriptGenerator, &script](const Coin::PublicKey & pubKey){
        RedeemScriptInfo info = scriptGenerator(pubKey);
        script = info.redeemScript;
        return info;
    });

    updateBloomFilter({script});

    return sk;
}

std::vector<Coin::PrivateKey> SPVWallet::generateKeys(const std::vector<RedeemScriptGenerator> & scriptGenerators) {
    if(!isInitialized()) {
        throw std::runtime_error("wallet not initialized");
    }

    std::vector<uchar_vector> scripts;
    uint32_t numKeys = scriptGenerators.size();

    std::vector<Coin::PrivateKey> keys = _store.generateKeys(numKeys, [&scriptGenerators, &scripts](const Coin::PublicKey & pubKey, uint32_t n){
        RedeemScriptInfo info = scriptGenerators[n](pubKey);
        scripts.push_back(info.redeemScript);
        return info;
    });

    updateBloomFilter(scripts);

    return keys;
}

std::vector<Coin::KeyPair>
SPVWallet::generateKeyPairs(const std::vector<RedeemScriptGenerator> &scriptGenerators) {
    if(!isInitialized()) {
        throw std::runtime_error("wallet not initialized");
    }

    std::vector<uchar_vector> scripts;
    std::vector<Coin::KeyPair> keyPairs;
    uint32_t numKeys = scriptGenerators.size();

    std::vector<Coin::PrivateKey> keys = _store.generateKeys(numKeys, [&scriptGenerators, &scripts](const Coin::PublicKey & pubKey, uint32_t n){
        RedeemScriptInfo info = scriptGenerators[n](pubKey);
        scripts.push_back(info.redeemScript);
        return info;
    });

    updateBloomFilter(scripts);

    for(auto key : keys) {
        keyPairs.push_back(Coin::KeyPair(key));
    }

    return keyPairs;
}

std::list<Coin::P2SHAddress> SPVWallet::listAddresses() {
    return _store.listAddresses();
}

Coin::P2SHAddress
SPVWallet::generateReceiveAddress()
{
    if(!isInitialized()) {
        throw std::runtime_error("wallet not initialized");
    }

    uchar_vector redeemScript;

    _store.generateKey([&redeemScript](const Coin::PublicKey & pubKey) {
       Coin::P2PKScriptPubKey script(pubKey);
       redeemScript = script.serialize();
       return redeemScript;
    });

    updateBloomFilter({redeemScript});

    return Coin::P2SHAddress(_network, Coin::RedeemScriptHash::fromRawScript(redeemScript));
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

std::string SPVWallet::getSeedWords() const {
    return _store.getSeedWords();
}

Coin::UnspentOutputSet
SPVWallet::lockOutputs(uint64_t minValue, uint32_t minimalConfirmations, const Store::RedeemScriptFilter &scriptFilter) {
    if(!isInitialized()) {
        throw std::runtime_error("wallet not initialized");
    }
    
    std::lock_guard<std::mutex> lock(_utxoMutex);

    Coin::UnspentOutputSet selectedOutputs;

    // Assume outputs are sorted in descending value
    std::list<std::shared_ptr<Coin::UnspentOutput>> unspentOutputs(_store.getUnspentTransactionsOutputs(minimalConfirmations, bestHeight(), scriptFilter));

    for(std::shared_ptr<Coin::UnspentOutput> & utxo : unspentOutputs) {
        if(_lockedOutpoints.find(utxo->outPoint()) != _lockedOutpoints.end()) continue;

        selectedOutputs.insert(utxo);

        if(selectedOutputs.value() >= minValue) {
            break;
        }
    }

    if(selectedOutputs.value() < minValue) {
        // not enough utxo
        return Coin::UnspentOutputSet();
    }

    // Lock and return the selected utxos
    for(auto utxo : selectedOutputs) {
        _lockedOutpoints.insert(utxo->outPoint());
    }

    return selectedOutputs;
}

uint SPVWallet::unlockOutputs(const Coin::UnspentOutputSet &outputs) {
    std::lock_guard<std::mutex> lock(_utxoMutex);

    uint unlockedCount = 0;

    for(auto utxo : outputs) {
        unlockedCount += _lockedOutpoints.erase(utxo->outPoint());
    }

    return unlockedCount;
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

void SPVWallet::updateBloomFilter(const std::vector<uchar_vector> redeemScripts) {

    for(auto &script : redeemScripts) {
        _bloomFilterScripts.insert(script);
    }

    const uint filterSize = _bloomFilterScripts.size() * 2;

    if(filterSize == 0) return;

    Coin::BloomFilter filter(filterSize, 0.0001, 0,0);

    for(const uchar_vector & script : _bloomFilterScripts) {
        // For capturing inputs: Redeem script will be last data item pushed to stack in input scriptSig
        filter.insert(script);

        // For capturing outputs: script hash
        Coin::P2SHAddress address = Coin::P2SHAddress(_network, Coin::RedeemScriptHash::fromRawScript(script));
        filter.insert(address.redeemScriptHash().toUCharVector());

        // Generate output scripts for direct comparison in createsWalletOutput() routine
        _scriptPubKeys.insert(address.toP2SHScriptPubKey().serialize());
    }

    _networkSync.setBloomFilter(filter);
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

    for(auto &redeemScript : _bloomFilterScripts) {
       // if redeem script length is greater than scriptSig length, then it cannot be part of it
       if(redeemScript.size() > txin.scriptSig.size()) continue;

       // Compare the redeemScript to the end of the scriptSig which would contain the redeem script
       // in a p2sh spending input
       if(std::equal(redeemScript.rbegin(), redeemScript.rend(), txin.scriptSig.rbegin())) return true;
    }

    return false;
}

bool SPVWallet::createsWalletOutput(const Coin::TxOut & txout) const {
    // If output script matches one of our wallet scriptPubKey
    return _scriptPubKeys.find(txout.scriptPubKey) != _scriptPubKeys.end();
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


Coin::Transaction SPVWallet::test_sendToAddress(uint64_t value, const Coin::P2SHAddress &destinationAddr, uint64_t fee) {

    // Get UnspentUTXO
    auto utxos(lockOutputs(value + fee, 0));

    if(utxos.value() < (value+fee)) {
        throw std::runtime_error("Not Enough Funds");
    }

    // Create an unsigned Transaction
    Coin::Transaction cointx;

    // Add output sending value to the destination address
    cointx.addOutput(Coin::TxOut(value, destinationAddr.toP2SHScriptPubKey().serialize()));

    uint64_t change = utxos.value() - (value + fee);

    if(change > 0) {
        // Create Change output
        Coin::P2SHAddress changeAddr = generateReceiveAddress();
        Coin::P2SHScriptPubKey changeScript(changeAddr.toP2SHScriptPubKey());
        cointx.addOutput(Coin::TxOut(change, changeScript.serialize()));
    }

    // Finance the transaction
    utxos.finance(cointx, Coin::SigHashType(Coin::SigHashType::MutuallyExclusiveType::all, false));

    broadcastTx(cointx);

    return cointx;
}


}
}
