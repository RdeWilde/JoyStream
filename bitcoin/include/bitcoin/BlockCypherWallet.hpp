#ifndef BITCOIN_BLOCKCYPHERWALLET_HPP
#define BITCOIN_BLOCKCYPHERWALLET_HPP

#include <QObject>
#include <blockcypher/UTXOManager.hpp>
#include <bitcoin/Store.hpp>

namespace joystream {
namespace bitcoin {

class BlockCypherWallet : public QObject
{
    Q_OBJECT

public:
    BlockCypherWallet(QString storePath,
                      Coin::Network network,
                      BlockCypher::Client * restClient,
                      BlockCypher::WebSocketClient * wsClient);

    // Create a new wallet with auto generated seed
    bool Create();

    // Create a new wallet with provided seed (useful for recovering a wallet from seed)
    bool Create(Coin::Seed seed);

    // Open the wallet. Returns true on success.
    bool Open();

    // Re-synchronize the wallet with network
    void Sync();

signals:

public slots:

private:
    Store _store;
    std::string _storePath;

    Coin::Network _network;

    BlockCypher::Client *_restClient;
    BlockCypher::WebSocketClient *_wsClient;

    BlockCypher::UTXOManager *_utxoManager;

    // true if utxo manager is successfully initialized
    // will be false when Wallet is created
    // when websocket client disconnects, we set this value to false
    // because the utxo manager will need to be re-initialized
    bool _utxoManagerIsInitialized;

    // The wallet is considered in sync when we have both successfully initialized the
    // utxo manager and the websocket client is connected
    bool InSync() { return _utxoManager != nullptr && _utxoManagerIsInitialized && _wsClient->isConnected(); }
};

}
}
#endif // BITCOIN_BLOCKCYPHERWALLET_HPP
