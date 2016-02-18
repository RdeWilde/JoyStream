#ifndef BITCOIN_BLOCKCYPHERWALLET_HPP
#define BITCOIN_BLOCKCYPHERWALLET_HPP

#include <QObject>
#include <common/UnspentP2PKHOutput.hpp>
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
    void Create();

    // Create a new wallet with provided seed (useful for recovering a wallet from seed)
    void Create(Coin::Seed seed);

    // Open the wallet. Will throw exception on failure.
    void Open();

    // Re-synchronize the wallet with network. This should be called
    // when websocket connectivity is lost.
    bool Sync(uint tries = 1);

    Coin::P2PKHAddress KeychainToP2PKHAddress(const Coin::HDKeychain & keychain);

    Coin::HDKeychain GetKey(bool createReceiveAddress);
    std::vector<Coin::HDKeychain> GetKeys(uint32_t numKeys, bool createReceiveAddress);
    std::vector<Coin::KeyPair> GetKeyPairs(uint32_t num_pairs, bool createReceiveAddress);
    Coin::P2PKHAddress GetReceiveAddress();

    std::list<Coin::UnspentP2PKHOutput> GetUnspentOutputs(uint64_t minValue, uint32_t minimalConfirmatinos, uint32_t currentBlockHeight);
    void ReleaseUnspentOutputs(const std::list<Coin::UnspentP2PKHOutput> outputs);

    // The wallet is considered in sync when we have both successfully initialized the
    // utxo manager at some point and the websocket client is connected
    bool InSync() { return _utxoManager != nullptr && _utxoManagerIsInitialized && _wsClient->isConnected(); }

signals:

    void BalanceChanged(uint64_t confirmedBalance, uint64_t unconfirmedBalance);

public slots:

private:
    Store _store;
    std::string _storePath;

    Coin::Network _network;

    BlockCypher::Client *_restClient;
    BlockCypher::WebSocketClient *_wsClient;

    BlockCypher::UTXOManager *_utxoManager;

    // true if utxo manager is successfully initialized
    // will be false when Wallet is created, will be set to true once we
    // successfully sync for the first time
    bool _utxoManagerIsInitialized;
};

}
}
#endif // BITCOIN_BLOCKCYPHERWALLET_HPP
