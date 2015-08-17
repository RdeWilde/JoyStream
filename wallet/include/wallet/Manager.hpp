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
#include <common/KeyPair.hpp>

#include <QObject>
#include <QString>
#include <QtSql>
#include <QDateTime>
#include <QByteArray>

class Payer;
class Payee;
class Slot;

namespace Coin {
    enum class Network;
    class P2PKHAddress;
    class Transaction;
    //class KeyPair;   
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

class Manager : public QObject
{
    Q_OBJECT
public:

    // Opens wallet
    explicit Manager(const QString & walletFile);

    // Create an empty wallet
    static void createNewWallet(const QString & walletFile, Coin::Network network, const Seed & seed);

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
    Seed seed() const;

    /**
     * List wallet content
     */

    // List all receive addresses in the wallet
    //QList<ReceiveAddress> listReceiveAddresses();

    // List wallet utxo set
    //QList<Output listUtxo();

    // Get and utxo
    // utxo getUtxo();

    // Number of transaction
    quint64 numberOfTransactions();

    // Number of keys in the wallet
    quint64 numberOfKeysInWallet();

    // Last 0-confirmation balance computed
    quint64 lastComputedZeroConfBalance();

    /**
     * DONT INCLUDE, USAGE MUST BE SYNCHED
     * , WHICH PEOPLE WILL FORGET.
    // Gets hd key index of next key
    quint64 nextHdIndex();
    */

    /**
     * Read operations
     */

    // Returns a fresh private key which persists in wallet
    // **NB: These keys are have no corresponding addresses
    // which are monitored for inbound/outbound? transactions**
    Coin::PrivateKey issueKey();

    // Generate p2pkh receive address
    // corresponding to a fresh private.
    Coin::P2PKHAddress getReceiveAddress();

    // Returns a list of key pairs, with ordered ascendingly in terms
    // of index.
    // **NB: These keys are have no corresponding addresses
    // which are monitored for inbound/outbound? transactions**
    QList<Coin::KeyPair> issueKeyPairs(quint64 numberOfPairs);

    // KEY POOL MANAGMENET: (for the future)
    // Return the given set of keys to key pool.
    // It is checked that a given key is actually not in use
    // before it is placed in the key pool.
    //void releaseKeys(const QSet<Coin::KeyPair> & keys);

    //Entry getAndLockEntry();
    //UnspentP2PKHOutput getUtxo(quint64 minimalValue, quint32 minimalNumberOfConfirmations);

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
    Seed _seed;

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
