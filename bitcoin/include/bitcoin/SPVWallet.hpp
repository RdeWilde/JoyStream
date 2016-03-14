#ifndef BITCOIN_SPVWALLET_HPP
#define BITCOIN_SPVWALLET_HPP

#include <QObject>
#include <common/Network.hpp>
#include <common/UnspentP2PKHOutput.hpp>

#include <CoinQ/CoinQ_netsync.h>
#include <bitcoin/Store.hpp>

namespace joystream {
namespace bitcoin {

class SPVWallet : public QObject
{
    Q_OBJECT

public:

    explicit SPVWallet(QString storePath, Coin::Network network = Coin::Network::testnet3);

    // Create a new wallet with auto generated seed
    void Create();

    // Create a new wallet with provided seed (useful for recovering a wallet from seed)
    void Create(Coin::Seed seed);

    // Open the wallet. Will throw exception on failure.
    void Open();

    // Start Synching the wallet with peer at host:port
    void Sync(std::string host, int port);

    Coin::PrivateKey GetKey(bool createReceiveAddress);
    std::vector<Coin::PrivateKey> GetKeys(uint32_t numKeys, bool createReceiveAddress);
    std::vector<Coin::KeyPair> GetKeyPairs(uint32_t num_pairs, bool createReceiveAddress);
    void ReleaseKey(const Coin::PrivateKey &sk);
    Coin::P2PKHAddress GetReceiveAddress();

    std::list<Coin::UnspentP2PKHOutput> GetUnspentOutputs(uint64_t minValue, uint32_t minimalConfirmatinos = 0);
    Coin::UnspentP2PKHOutput GetOneUnspentOutput(uint64_t minValue);
    void ReleaseUnspentOutputs(const std::list<Coin::UnspentP2PKHOutput> outputs);

    uint64_t Balance() const;
    uint64_t UnconfirmedBalance() const;

    Coin::Network network() const { return _network; }

    Q_INVOKABLE void BroadcastTx(Coin::Transaction & tx);

signals:

    void BalanceChanged(uint64_t confirmedBalance, uint64_t unconfirmedBalance);


public slots:


private:
    std::string _storePath;

    Store _store;

    Coin::Network _network;

    CoinQ::Network::NetworkSync _networkSync;

};

}
}
#endif // BITCOIN_SPVWALLET_HPP
