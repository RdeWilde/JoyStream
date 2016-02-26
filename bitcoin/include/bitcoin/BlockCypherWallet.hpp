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
    BlockCypherWallet(QString storePath, Coin::Network network,
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

    Coin::PrivateKey GetKey(bool createReceiveAddress);
    std::vector<Coin::PrivateKey> GetKeys(uint32_t numKeys, bool createReceiveAddress);
    std::vector<Coin::KeyPair> GetKeyPairs(uint32_t num_pairs, bool createReceiveAddress);
    void ReleaseKey(const Coin::PrivateKey &sk);
    Coin::P2PKHAddress GetReceiveAddress();

    std::list<Coin::UnspentP2PKHOutput> GetUnspentOutputs(uint64_t minValue, uint32_t minimalConfirmatinos = 0);
    Coin::UnspentP2PKHOutput GetOneUnspentOutput(uint64_t minValue);
    void ReleaseUnspentOutputs(const std::list<Coin::UnspentP2PKHOutput> outputs);

    uint64_t Balance() const { return _utxoManager->balance(); }
    uint64_t UnconfirmedBalance() const { return _utxoManager->unconfirmedBalance(); }

    Coin::Network network() const { return _network; }

    Q_INVOKABLE void BroadcastTx(const Coin::Transaction & tx);

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

    Coin::P2PKHAddress KeychainToP2PKHAddress(const Coin::HDKeychain & keychain);
};

}
}
#endif // BITCOIN_BLOCKCYPHERWALLET_HPP
