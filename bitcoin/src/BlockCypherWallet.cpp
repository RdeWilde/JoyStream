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

bool BlockCypherWallet::Open() {

    // Only open the store once
    if(!_store.connected()){
        if(!_store.open(_storePath)) return false;
    }

    // initialize utxo manager
    if(!_utxoManager) {
        Sync();
    }

    return _utxoManager != nullptr;
}

void BlockCypherWallet::Sync() {
    std::list<Coin::P2PKHAddress> addresses = _store.listReceiveAddresses();

    if(_utxoManager) {
        _utxoManagerIsInitialized = _utxoManager->refreshUtxoState(_restClient, addresses);
    } else {
        _utxoManager = BlockCypher::UTXOManager::createManager(_wsClient, _restClient, addresses);
        _utxoManagerIsInitialized = _utxoManager != nullptr;
    }
}

}
}
