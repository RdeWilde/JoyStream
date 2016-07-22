#ifndef BITCOIN_SPVWALLET_HPP
#define BITCOIN_SPVWALLET_HPP

#include <QObject>
#include <common/Network.hpp>
#include <common/UnspentOutput.hpp>
#include <common/P2SHScriptPubKey.hpp>

#include <CoinQ/CoinQ_netsync.h>
#include <bitcoin/Store.hpp>

#include <mutex>

class Test;

namespace Coin {
    class UnspentOutputSet;
    class Entropy;
}

namespace joystream {
namespace bitcoin {

class SPVWallet : public QObject
{
    Q_OBJECT

public:

    enum class wallet_status_t
    {
        UNINITIALIZED,      // wallet database file not yet opened, netsync stopped
                            // this is the starting state of the wallet, it will never go back
                            // to this state after successful opening/creating of the wallet database

        OFFLINE,            // wallet database ready, netsync stopped

        CONNECTING,         // netsync connecting, starting background threads

        SYNCHING_HEADERS,   // netsync is retrieving headers and updating blocktree

        SYNCHING_BLOCKS,    // netsync is retreiving filtered blocks and the wallet is
                            // updating the store block headers and transactions

        SYNCHED             // receiving mempool (unconfirmed transactions) and waiting for next block
    };

    explicit SPVWallet(std::string storePath, std::string blockTreeFile, Coin::Network network = Coin::Network::testnet3);

    // Create a new wallet (auto generated bip39 seed)
    void create();

    // Create a new wallet bip39 seed generated from provided entropy
    // Important - Creating a wallet with a predefined entropy does not by itself restore
    // a wallet.
    void create(const Coin::Entropy &entropy, uint32_t timestamp = 0);

    // Open the wallet. Will throw exception on failure
    void open(std::string passphrase = "");

    bool encrypted() const;
    bool locked() const;

    void unlock(std::string passphrase);
    void lock();

    void encrypt(std::string passphrase);
    void decrypt(std::string passphrase);

    // Start Synching the wallet with peer at host:port
    void sync(std::string host, int port, unsigned int timeout = 0);
    void stopSync();

    void loadBlockTree(std::function<void(std::string)> feedback = nullptr);

    wallet_status_t status() const { return _walletStatus; }

    bool isInitialized() const { return _walletStatus != wallet_status_t::UNINITIALIZED; }
    bool isOffline() const { return _walletStatus == wallet_status_t::OFFLINE; }
    bool isConnected() const { return (isInitialized() && !isOffline()); }
    bool isSynchingHeaders() const { return _walletStatus == wallet_status_t::SYNCHING_HEADERS;}
    bool isSynchingBlocks() const { return _walletStatus == wallet_status_t::SYNCHING_BLOCKS;}
    bool isSynched() const { return _walletStatus == wallet_status_t::SYNCHED;}

    // Generate Keys with custom redeem scripts
    Coin::PrivateKey generateKey(const RedeemScriptGenerator & scriptGenerator);
    std::vector<Coin::PrivateKey> generateKeys(const std::vector<RedeemScriptGenerator> & scriptGenerators);
    std::vector<Coin::KeyPair> generateKeyPairs(const std::vector<RedeemScriptGenerator> & scriptGenerators);

    // Generate a Key for external use
    Coin::PublicKey generateReceivePublicKey();

    // Generate a Key for internal use (change address)
    Coin::PublicKey generateChangePublicKey();

    //Generate addresses..
    Coin::P2PKHAddress generateReceiveAddress();
    Coin::P2PKHAddress generateChangeAddress();

    Coin::PrivateKey generateReceivePrivateKey();
    Coin::PrivateKey generateChangePrivateKey();

    Coin::UnspentOutputSet lockOutputs(uint64_t minValue, uint32_t minimalConfirmations = 0, const Store::RedeemScriptFilter &scriptFilter = nullptr);
    uint unlockOutputs(const Coin::UnspentOutputSet & outputs);

    uint64_t balance() const;
    uint64_t unconfirmedBalance() const;

    Coin::Network network() const { return _network; }

    Q_INVOKABLE void broadcastTx(Coin::Transaction cointx);

    int32_t bestHeight() const;

    std::string getSeedWords() const;
    
    std::vector<Coin::P2PKHAddress> listReceiveAddresses() const;

signals:

    void syncStarted();

    // A general status message to display in the UI
    void statusMessageUpdated(std::string);

    // Error signals    
    void storeUpdateFailed(std::string);

    void blockTreeUpdateFailed(std::string);
    void blockTreeWriteFailed(std::string);

    void protocolError(std::string);
    void connectionError(std::string);
    void connectionTimedOut();

    // Wallet Status change
    void statusChanged(wallet_status_t);

    // Signal corresponding to state transition
    void offline();
    void connecting();
    void disconnected();
    void connected();
    void synchingHeaders();
    void synchingBlocks();
    void synched();

    // Signal emitted when wallet balance changes
    void balanceChanged(uint64_t confirmedBalance, uint64_t unconfirmedBalance);

    void txUpdated(Coin::TransactionId txid, int confirmations);

public slots:


private:
    friend class ::Test;

    std::string _storePath;

    Store _store;

    Coin::Network _network;

    CoinQ::Network::NetworkSync _networkSync;
    wallet_status_t _walletStatus;

    void updateStatus(wallet_status_t status);

    bool _blockTreeLoaded;
    std::string _blockTreeFile;

    uint64_t _confirmedBalance;
    uint64_t _unconfirmedBalance;

    // NetSync event handlers
    void onBlockTreeError(const std::string &error, int code);
    void onBlockTreeChanged();
    void onSynchingHeaders();
    void onHeadersSynched();
    void onSynchingBlocks();
    void onBlocksSynched();
    void onNewTx(const Coin::Transaction& cointx);
    void onTxConfirmed(const ChainMerkleBlock& chainmerkleblock, const bytes_t& txhash, unsigned int txindex, unsigned int txcount);
    void onMerkleTx(const ChainMerkleBlock& chainmerkleblock, const Coin::Transaction& cointx, unsigned int txindex, unsigned int txcount);
    void onMerkleBlock(const ChainMerkleBlock& chainmerkleblock);

    std::set<uchar_vector> _bloomFilterScripts;
    std::set<Coin::PublicKey> _bloomFilterPubKeys;
    std::set<uchar_vector> _scriptPubKeys;

    void updateBloomFilter(const std::vector<uchar_vector> redeemScripts, const std::vector<Coin::PublicKey> publicKeys);

    bool transactionShouldBeStored(const Coin::Transaction &) const;
    bool spendsWalletOutput(const Coin::TxIn &) const;
    bool createsWalletOutput(const Coin::TxOut &) const;

    void recalculateBalance();

    mutable std::mutex _utxoMutex;
    std::set<Coin::typesafeOutPoint> _lockedOutpoints;
    
    // Prefix methods only required from unit tests with test_
    void test_syncBlocksStaringAtHeight(int32_t height);
    int32_t test_netsyncBestHeight() const { return _networkSync.getBestHeight(); }
    Coin::Transaction test_sendToAddress(uint64_t value, const Coin::P2PKHAddress &destinationAddr, uint64_t fee);
    Coin::Transaction test_sendToAddress(uint64_t value, const Coin::P2SHAddress &destinationAddr, uint64_t fee);
    Coin::Transaction test_sendToAddress(uint64_t value, const uchar_vector &scriptPubKey, uint64_t fee);

};

}
}
#endif // BITCOIN_SPVWALLET_HPP
