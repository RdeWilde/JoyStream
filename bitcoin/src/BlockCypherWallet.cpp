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

}
}
