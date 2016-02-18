#include <bitcoin/BlockCypherWallet.hpp>

namespace joystream {
namespace bitcoin {

BlockCypherWallet::BlockCypherWallet(QString storePath, Coin::Network network,
                                     BlockCypher::Client * restClient,
                                     BlockCypher::WebSocketClient * wsClient) :
    _storePath(storePath.toStdString()),
    _network(network),
    _restClient(restClient),
    _wsClient(wsClient),
    _utxoManager(nullptr),
    _utxoManagerIsInitialized(false)
{}

bool BlockCypherWallet::Create() {
    return _store.create(_storePath, _network);
}

bool BlockCypherWallet::Create(Coin::Seed seed) {
    return _store.create(_storePath, _network, seed, std::time(nullptr));
}

void BlockCypherWallet::Open() {

    // Only open the store once
    if(_store.connected()) {
        if(!_store.open(_storePath)) {
            throw std::runtime_error("failed to open wallet");
        }
    }

    // initialize utxo manager
    if(!_utxoManager) {
        _utxoManager = BlockCypher::UTXOManager::createManager(_wsClient);

        QObject::connect(_utxoManager, SIGNAL(balanceChanged()),
                         this, SIGNAL(balanceChanged()));
    }

    // make 2 attempts to initialize the utxo manager
    if(!Sync(2)) {
        throw std::runtime_error("unable to do initial sync");
    }
}

bool BlockCypherWallet::Sync(uint tries) {
    std::list<Coin::P2PKHAddress> addresses = _store.listReceiveAddresses();

    if(!_utxoManager) return false;

    for(uint i = 0; i < tries; i++){
        if(_utxoManager->refreshUtxoState(_restClient, addresses)) {
            _utxoManagerIsInitialized = true;
            return true;
        }
    }

    return false;
}

Coin::P2PKHAddress
BlockCypherWallet::KeychainToP2PKHAddress(const Coin::HDKeychain & keychain) {
    Coin::PrivateKey sk(keychain.privkey());
    return sk.toPublicKey().toP2PKHAddress(_network);
}

Coin::HDKeychain
BlockCypherWallet::getKey(bool createReceiveAddress) {
    Coin::HDKeychain keychain = _store.getKey(createReceiveAddress);

    if(createReceiveAddress)
        _utxoManager->addAddress(KeychainToP2PKHAddress(keychain));

    return keychain;
}

std::vector<Coin::HDKeychain>
BlockCypherWallet::getKeys(uint32_t numKeys, bool createReceiveAddress) {
    std::vector<Coin::HDKeychain> keychains = _store.getKeys(numKeys, createReceiveAddress);
    if(createReceiveAddress) {
        for(auto & keychain : keychains) {
            _utxoManager->addAddress(KeychainToP2PKHAddress(keychain));
        }
    }
    return keychains;
}

std::vector<Coin::KeyPair>
BlockCypherWallet::getKeyPairs(uint32_t num_pairs, bool createReceiveAddress) {
    std::vector<Coin::KeyPair> keyPairs = _store.getKeyPairs(num_pairs, createReceiveAddress);
    if (createReceiveAddress) {
        for (auto & keypair : keyPairs) {
            _utxoManager->addAddress(keypair.pk().toP2PKHAddress(_network));
        }
    }
    return keyPairs;
}

Coin::P2PKHAddress
BlockCypherWallet::getReceiveAddress()
{
    Coin::P2PKHAddress addr = _store.getReceiveAddress();
    _utxoManager->addAddress(addr);
    return addr;
}

std::list<Coin::UnspentP2PKHOutput>
BlockCypherWallet::GetUnspentOutputs(uint64_t minValue, uint32_t minimalConfirmatinos, uint32_t currentBlockHeight)
{
    std::set<BlockCypher::UTXO> utxos = _utxoManager->getUtxoSet(minValue, minimalConfirmatinos, currentBlockHeight);

    std::list<Coin::UnspentP2PKHOutput> unspentOutputs;

    for (const BlockCypher::UTXO &utxo : utxos) {
        Coin::PrivateKey sk;
        if(_store.loadKey(Coin::P2PKHAddress::fromBase58CheckEncoding(utxo.address()), sk)) {
            Coin::KeyPair keypair(sk);
            Coin::UnspentP2PKHOutput output(keypair, utxo.outPoint(), utxo.value());
            unspentOutputs.push_back(output);
        }
    }

    return unspentOutputs;
}

void BlockCypherWallet::ReleaseUnspentOutputs(const std::list<Coin::UnspentP2PKHOutput> outputs)
{
    std::set<BlockCypher::UTXO> utxos;

    for (auto & output : outputs) {
        BlockCypher::UTXO utxo(output.keyPair().pk().toP2PKHAddress(_network).toBase58CheckEncoding(),
                               output.outPoint(), output.value());
        utxos.insert(utxo);
    }

    _utxoManager->releaseUtxoSet(utxos);
}

}
}
