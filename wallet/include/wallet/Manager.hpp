/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#ifndef WALLET_MANAGER_HPP
#define WALLET_MANAGER_HPP

#include <CoinCore/hdkeys.h>
#include <common/Seed.hpp>
#include <common/TransactionId.hpp> // cannot be forward declard

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
}

// The number keys in a newly populated key pool
#define KEY_POOL_FRESH_SIZE 100

// When the key pool falls below this limit, create new keys so
// as to make the size KEY_POOL_FRESH_SIZE
#define KEY_POOL_REPOPOLUATION_LIMIT 20

// Name of database type to use with QSqlDatabase::addDatabas
#define DATABASE_TYPE "QSQLITE"

namespace Wallet {

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

class UtxoCreated;
class UtxoDestroyed;

class Manager : public QObject
{
    Q_OBJECT
public:

    // Opens wallet
    explicit Manager(const QString & walletFile);

    // Create an empty wallet
    static void createNewWallet(const QString & walletFile, Coin::Network network, const Coin::Seed & seed);

    // Check basic integrity of wallet database
    static bool validateWalletStructure(QSqlDatabase & db);

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
     * DONT INCLUDE, USAGE MUST BE SYNCHED
     * , WHICH PEOPLE WILL FORGET.
    // Gets hd key index of next key
    quint64 nextHdIndex();
    */

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
    //QList<ReceiveAddress> listReceiveAddresses();

    // KEY POOL MANAGMENET: (for the future)
    // ====================================
    // Return the given set of keys to key pool.
    // It is checked that a given key is actually not in use
    // before it is placed in the key pool.
    //void releaseKeys(const QSet<Coin::KeyPair> & keys);

    /**
     * Transactions
     */

    // Adds a block header to wallet, throws exception if it already exists
    void addBlockHeader(const Coin::BlockHeader & blockHeader);

    // Add outpoint to wallet, throws exception if it already exists
    void addOutPoint(const Coin::OutPoint & outPoint);

    // Add output to wallet, throws exception if it already exists
    void addOutPut(const Coin::TxOut & txOut);

    // Add input to wallet, throws exception if it already exists
    void addInput(const Coin::TxIn & txIn);

    // Add transaction to wallet, throws exception if it already exists
    void addTransaction(const Coin::Transaction & transaction);

    // Tries to recover the transaction with the given wallet it
    Coin::Transaction getTransaction(const Coin::TransactionId & transactionId);

    // Lists all transactions in wallet
    QList<Coin::Transaction> allTransactions();

    // Number of transaction
    quint64 numberOfTransactions();

    /**
     * Payments
     */

    // ....

    /**
     * Utxo
     */

    // Lock utxo
    //QList<Coin::UnspentP2PKHOutput> lockUtxo(quint64 minimalAmount, quint64 minimalConfirmations = 1);

    // Release a utxo
    //void releaseUtxo(const Coin::UnspentP2PKHOutput & utxo);

    // List all wallet utxo which have been created
    QList<UtxoCreated> getAllUtxoCreated();

    // List all wallet utxo which has been destroyed
    QList<UtxoDestroyed> getAllUtxoDestroyed();
    
    /**
     * State managing operation
     */

    // Scraps current key pool, and rebuilds based on dbase
    //void updateKeyPool();

    // Scraps current utxo, and rebuilds based on dbase
    void updateUtxoSet();

    /**
     * Bitcoin network communication
     */

    void broadcast(const Coin::Transaction & tx);

signals:

    // When updateKeyPool() is done
    void keyPoolUpdated(quint32 diff);

    // When updateUtxo() is done
    void utxoUpdated(quint32 diff);

    // Balance change
    void zeroConfBalanceChanged(quint64);

    /**
     * Transactions
     */

    // BlockHeader was added through addBlockHeader()
    void blockHeaderAdded(const Coin::BlockHeader & blockHeader);

    // OutPoint was added through addOutPoint()
    void outPointAdded(const Coin::OutPoint & outPoint);

    // Input was added through addInput()
    void inputAdded(const Coin::TxIn & txIn);

    // Output was added through addOutPut()
    void outPutAdded(const Coin::TxOut & txOut);

    // Transaction was added through addTransaction()
    void transactionAdded(const Coin::Transaction & transaction);

    /**
     * Utxo
     */

    // Wallet utxo which was created
    void utxoCreated(const UtxoCreated & event);

    // Wallet utxo was destroyed
    void utxoDestroyed(const UtxoDestroyed & event);

public slots:

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
     * State
     */

    // Index of next key
    quint64 _nextIndex;

    // Latest known block height
    qint64 _latestBlockHeight;

    // Value of last run of computeBalance(0), which is initially run in ctr
    quint64 _lastComputedZeroConfBalance;

    // Key pool
    //QSet<Coin::PrivateKey> _keyPool;

    // Utxo <== keep as managed state, since it becomes kind of expensive to keep rederiving
    //QMap< outpoint, output> _utxo;

    // Generate p2pkh receive address
    // corresponding to a fresh private
    Address::Record _createReceiveAddress();

    // Returns a fresh private key which persists in wallet
    // **NB: These keys are have no corresponding addresses
    // which are monitored for inbound/outbound? transactions**
    Key::Record _issueKey();
};

}

#endif // WALLET_MANAGER_HPP
