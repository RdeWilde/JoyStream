#ifndef BITCOIN_SPVWALLET_HPP
#define BITCOIN_SPVWALLET_HPP

#include <QObject>
#include <common/Network.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <bitcoin/SynchedStore.hpp>

namespace joystream {
namespace bitcoin {

class SPVWallet : public QObject
{
    Q_OBJECT

public:
    enum TxConfirmationStatus {
        unconfirmed,
        first_confirmation
    };

    explicit SPVWallet(QString storePath, Coin::Network network);

    // Create a new wallet with auto generated seed
    void Create();

    // Create a new wallet with provided seed (useful for recovering a wallet from seed)
    void Create(Coin::Seed seed);

    // Open the wallet. Will throw exception on failure.
    void Open();

    // Re-synchronize the wallet with network. This should be called
    // when network connectivity is lost
    bool Sync(uint tries = 1);

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

    Q_INVOKABLE void BroadcastTx(const Coin::Transaction & tx);

signals:

    void BalanceChanged(uint64_t confirmedBalance, uint64_t unconfirmedBalance);

    // Is it required to signal on each Tx update ?
    void TxUpdate(const Coin::Transaction & tx, TxConfirmationStatus status);

public slots:
    // Slots to handle all the spvclient Signals


private:
    Store _Store;
    std::string _storePath;
    Coin::Network _network;

    // CoinQ::Network::NetworkSync 'wrapper' - maintains basic internal state and emits Qt Signals
    // Doesn't know anything about a store, The wallet will coordinate between the spv client and store
    joystream::bitcoin::SPVClient  _spvClient;

    Coin::BloomFilter generateBloomfilter();
};

}
}
#endif // BITCOIN_SPVWALLET_HPP
