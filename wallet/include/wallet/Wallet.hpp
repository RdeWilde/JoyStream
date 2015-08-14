/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#ifndef WALLET_HPP
#define WALLET_HPP

#include <CoinCore/hdkeys.h>
#include <wallet/Seed.hpp>
#include <common/KeyPair.hpp>

#include <QObject>
#include <QString>
#include <QtSql>
#include <QDateTime>
#include <QByteArray>

class WalletKey;
class ReceiveAddress;
class Payer;
class Payee;
class Slot;

namespace Coin {
    enum class Network;
    //class KeyPair;
}

// The number keys in a newly populated key pool
#define KEY_POOL_FRESH_SIZE 100

// When the key pool falls below this limit, create new keys so
// as to make the size KEY_POOL_FRESH_SIZE
#define KEY_POOL_REPOPOLUATION_LIMIT 20

// Name of database type to use with QSqlDatabase::addDatabas
#define DATABASE_TYPE "QSQLITE"

class Wallet : public QObject
{
    Q_OBJECT
public:


    // Opens wallet
    explicit Wallet(const QString & walletFile);

    // Create an empty wallet
    static void createNewWallet(const QString & walletFile, Coin::Network network, const Seed & seed);

    // Check basic integrity of wallet database
    static bool validateWalletStructure(QSqlDatabase & db);

    /**
     * List wallet content
     */

    // List all receive addresses in the wallet
    //QList<ReceiveAddress> listReceiveAddresses();

    // List wallet utxo set
    //QList<Output listUtxo();


    // Number of transaction
    quint32 numberOfTransactions(); // const

    // Number of keys in the wallet
    quint64 numberOfKeysInWallet();  // const

    /**
     * Get
     */

    // Network wallet corresponds to
    Coin::Network network() const;

    // Time when wallet was created
    QDateTime created() const;

    // Get and utxo
    // utxo getUtxo();

    // Generate receive address
    // ReceiveAddress getReceiveAddress();

    // Returns the given number of fresh key pairs
    QList<Coin::KeyPair> getFreshKeys(quint8 numberOfKeys);

    // Return the given set of keys to key pool.
    // It is checked that a given key is actually not in use
    // before it is placed in the key pool.
    void releaseKeys(const QSet<Coin::KeyPair> & keys);

    //Entry getAndLockEntry();
    //UnspentP2PKHOutput getUtxo(quint64 minimalValue, quint32 minimalNumberOfConfirmations);

    quint64 lastComputedZeroConfBalance();

    /**
     * State modifying operation
     */

    // Scraps current key pool, and rebuilds based on dbase
    void updateKeyPool();

    // Scraps current utxo, and rebuilds based on dbase
    void updateUtxo();

    /**
     * Attempts to insert object in wallet database,
     * (explain what happens in the event of failure)

    void add(const WalletKey & walletKey);
    void add(const WalletKey & walletKey);
    void add(const ReceiveAddress & receiveAddress);

    void add(const Payer & payer);
    void add(const Payee & payee);
    void add(const Slot & slot);
    */

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
    //    QByteArray _seed;
    Seed _seed;

    /**
     * HD key chain manager
     */

    //
    //Coin::HDSeed _seedManager;

    // Key chain used in wallet for get derivation
    Coin::HDKeychain _keyChain;

    /**
     * State
     */

    // Latest known block height
    qint64 _latestBlockHeight;

    // Value of last run of computeBalance(0), which is initially run in ctr
    quint64 _lastComputedZeroConfBalance;

    // Key pair pool <== perhaps drop as managed state, since its such an easy thing to derive?
    QSet<Coin::KeyPair> _keyPairPool;

    // Utxo <== keep as managed state, since it becomes kind of expensive to keep rederiving
    //QMap< outpoint, output> _utxo;
};

#endif // WALLET_HPP
