/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#ifndef WALLET_MANAGER_HPP
#define WALLET_MANAGER_HPP

#include <wallet/UtxoCreated.hpp>
#include <wallet/UtxoDestroyed.hpp>
#include <wallet/Transaction.hpp>

#include <CoinCore/hdkeys.h>
#include <common/Seed.hpp>
#include <common/TransactionId.hpp> // cannot be forward declard
#include <common/UnspentP2PKHOutput.hpp>
#include <common/typesafeOutPoint.hpp> // for qHash for QSet

#include <QObject>
#include <QString>
#include <QtSql>
#include <QDateTime>
#include <QByteArray>

namespace Coin {
    enum class Network;
    class PrivateKey;
    class P2PKHAddress;
    class OutPoint;
    class Input;
    class TxIn;
    class TxOut;
    class BlockHeader;
    class Transaction;
    class KeyPair;

    class CoinBlockHeader;
}

namespace CoinQ {
    namespace Network {
        class NetworkSync;
    }
}

class CoinQBlockTreeMem;

// The number keys in a newly populated key pool
#define KEY_POOL_FRESH_SIZE 100

// When the key pool falls below this limit, create new keys so
// as to make the size KEY_POOL_FRESH_SIZE
#define KEY_POOL_REPOPOLUATION_LIMIT 20

// Name of database type to use with QSqlDatabase::addDatabas
#define DATABASE_TYPE "QSQLITE"

namespace Wallet {

class SPVClient;

namespace Key {
    class Record;
}
namespace Address {
    class Record;
}
namespace Payer {
    class Record;
}
namespace Payee {
    class Record;
}
namespace Slot {
    class Record;
}

class Manager : public QObject
{
    Q_OBJECT
public:

    // Opens a connection to a database in given path, gives connection name of file
    static QSqlDatabase openDatabaseConnection(const QString & walletFile);

    // Opens wallet
    explicit Manager(const QString & walletFile);

    ~Manager();

    // Create an empty wallet
    static void createNewWallet(const QString & walletFile, Coin::Network network, const Coin::Seed & seed);

    // Check basic integrity of wallet database
    static bool validateWalletStructure(QSqlDatabase & db);

    // Setup signals and slots
    void startSPVClient(const QString & blockHeaderStore, const QString & host);

    /**
     * Getters : Unsynched because these values are read-only,
     * is therefore also const since mutex is not needed.
     */

    // Network wallet corresponds to
    Coin::Network network() const;

    // Time when wallet was created
    QDateTime created() const;

    // Wallet seed
    Coin::Seed seed() const;

    // Last 0-confirmation balance computed
    quint64 lastComputedZeroConfBalance();

    /**
     * Keys
     */

    // Returns a fresh private key which persists in wallet
    // **NB: These keys are have no corresponding addresses
    // which are monitored for inbound/outbound spends.
    Coin::PrivateKey issueKey();

    // Returns a list of key pairs, with ordered ascendingly in terms
    // of index.
    // **NB: These keys are have no corresponding addresses
    // which are monitored for inbound/outbound spends.
    QList<Coin::KeyPair> issueKeyPairs(quint64 numberOfPairs);

    // Number of keys in the wallet
    quint64 numberOfKeysInWallet();

    /**
     * Addresses
     */

    // Generate p2pkh receive address corresponding to a fresh private.
    // These addresses are monitored for incoming and outgoing spends.
    Coin::P2PKHAddress getReceiveAddress();

    // List all receive addresses in the wallet
    QList<Coin::P2PKHAddress> listReceiveAddresses();

    // KEY POOL MANAGMENET: (for the future)
    // ====================================
    // Return the given set of keys to key pool.
    // It is checked that a given key is actually not in use
    // before it is placed in the key pool.
    //void releaseKeys(const QSet<Coin::KeyPair> & keys);

    /**
     * Transactions
     */

    // Adds a block header to wallet, returns true IFF this was successfull (throws exception if it already exists)
    bool addBlockHeader(const Coin::CoinBlockHeader & blockHeader,
                        quint64 numberOfTransactions,
                        bool isOnMainChain,
                        quint32 totalProofOfWork,
                        quint64 blockHeight);

    // Add outpoint to wallet, throws exception if it already exists
    bool addOutPoint(const Coin::OutPoint & outPoint);

    // Add input to wallet, throws exception if it already exists
    bool addInput(const Coin::TxIn & txIn);

    // Add output to wallet, throws exception if it already exists
    bool addOutput(const Coin::TxOut & txOut);


    /**
    // Calculates fees for transaction
    bool calculateAndSetFee(const Coin::TransactionId & transactionId, quint64 & fee);

    // Gets best chain membership for given transaction, or throws exception if transaction is not in wallet
    Transaction::ChainMembership bestChainMemberShip(const Coin::TransactionId & transactionId);

    // Registers the membership of a transaction in a given block,
    // throws exceptions if either block or transaction are not already in the wallet
    bool registerTransactionInBlock(const Coin::TransactionId  & transactionId, const Coin::BlockId & blockId, .. merkle proof);
    */

    // Tries to recover the transaction with the given wallet it
    Coin::Transaction getTransaction(const Coin::TransactionId & transactionId);

    // Lists all transactions in wallet
    QList<Coin::Transaction> allTransactions();

    // Number of transaction
    quint64 numberOfTransactions();

    /**
     * Payments
     */



    /**
     * Utxo
     */

    // Lists the utxo with a minimal number of confirmations: DOES NOT
    QList<Coin::UnspentP2PKHOutput> listUtxo(quint64 minimalConfirmations = 0);

    // Lock utxo
    QList<Coin::UnspentP2PKHOutput> lockUtxo(quint64 minimalAmount, quint64 minimalConfirmations = 0);

    // Releases the corresponding output from locked set,
    // returns whether any output was actually locked with this outpoint
    bool releaseUtxo(const Coin::typesafeOutPoint & o);

    // List all wallet utxo which have been created
    QList<UtxoCreated> getAllUtxoCreated(quint64 minimalConfirmations = 0);

    // List all wallet utxo which has been destroyed
    QList<UtxoDestroyed> getAllUtxoDestroyed(quint64 minimalConfirmations = 0);

    // Find balance at given number of confirmations
    quint64 balance(quint64 minimalConfirmations = 0);

    /**
     * Bitcoin network communication
     */

    void broadcast(const Coin::Transaction & tx);

    /**
     * SPV client callbacks
     */
    void blockStoreLoaded(const CoinQBlockTreeMem & blocktree);



    /**
     * Raw database link: Remove later? only used for testing
     */

    QSqlDatabase db();

public slots:


    // Add transaction to wallet, throws exception if it already exists
    bool addTransaction(const Coin::Transaction & transaction);

signals:

    /**
     * Keys
     */

    void numberOfKeysInWalletChanged(quint64 numberOfKeysInWallet);

    // When updateKeyPool() is done
    //void keyPoolUpdated(quint32 diff);

    /**
     * Addresses
     */

    void addressCreated(const Coin::P2PKHAddress & address);

    /**
     * Transactions
     */

    // BlockHeader was added through addBlockHeader()
    void blockHeaderAdded(const Coin::CoinBlockHeader & blockHeader);

    // OutPoint was added through addOutPoint()
    void outPointAdded(const Coin::OutPoint & outPoint);

    // Input was added through addInput()
    void inputAdded(const Coin::TxIn & txIn);

    // Output was added through addOutPut()
    void outputAdded(const Coin::TxOut & txOut);

    // Transaction was added through addTransaction()
    void transactionAdded(const Coin::Transaction & transaction);

    // An output was added which allowed the computation of a transaction
    // for which the fee was not previously known
    void feeFound(const Coin::TransactionId & transacionId, quint64 fee);

    // Transaction has a new best chain membership status
    void newBestChainMemberShip(const Coin::TransactionId & transactionId, const Transaction::ChainMembership & membership);

    /**
     * Utxo
     */

    // Utxo which was created
    void utxoCreated(const UtxoCreated & event);

    // Utxo was destroyed
    void utxoDestroyed(const UtxoDestroyed & event);

    // Utxo was unlocked
    void utxoReleased(const Coin::typesafeOutPoint & o);

    // Balance change
    void zeroConfBalanceChanged(quint64);

private:

    // Synchronizes wallet calls
    // It may be worth adding different mutexes
    // for different routines or variables
    // in the future.
    QMutex _mutex;

    // Wallet (SQLite database) file
    QString _walletFile;

    // Database connection
    QSqlDatabase _db;

    // Network wallet corresponds to
    Coin::Network _network;

    // Time when wallet was created
    QDateTime _created;

    // Seed
    Coin::Seed _seed;

    // Key chain used in wallet for get derivation
    Coin::HDKeychain _keyChain;

    /**
     * SPV clients
     * host name -> spv client object for host
     */
    QMap<QString, CoinQ::Network::NetworkSync *> _clients;

    /**
     * State
     */

    // Index of next key
    quint64 _nextIndex;

    // Latest known block height
    quint64 _latestBlockHeight;

    // Value of last run of computeBalance(0), which is initially run in ctr
    quint64 _lastComputedZeroConfBalance;

    // Key pool
    //QSet<Coin::PrivateKey> _keyPool;

    // Unspent outputs on present main chain
    QSet<Coin::UnspentP2PKHOutput> _utxo;

    // Outpoints which are utxo, but which have been resereved for
    // use by some external client. While in this set
    // a corresponding output is never exporter in response to
    // a utxo request from a client.
    QSet<Coin::typesafeOutPoint> _lockedOutPoints;

    // Generate p2pkh receive address
    // corresponding to a fresh private
    Address::Record _createReceiveAddress();

    // Returns a fresh private key which persists in wallet
    // **NB: These keys are have no corresponding addresses
    // which are monitored for inbound/outbound? transactions**
    Key::Record _issueKey();

    // Determines address in output script and tries to recover record in wallet, if one exists
    bool getAddressForOutput(const Coin::TxOut & txOut, Address::Record & record);

};

}

#endif // WALLET_MANAGER_HPP
