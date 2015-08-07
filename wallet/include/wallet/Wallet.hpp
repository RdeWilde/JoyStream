/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#ifndef WALLET_HPP
#define WALLET_HPP

#include <common/CoinWrappers.hpp>

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

class Wallet : public QObject
{
    Q_OBJECT
public:

    // Opens wallet
    explicit Wallet(const QString & walletFile);

    // Create an empty wallet
    static void createEmptyWallet(const QString & walletFile, Coin::Network network, const QByteArray & seed);

    // Check basic integrity of wallet database
    static bool validateWalletStructure(QSqlDatabase & db);

    /**
     * List wallet content
     */

    // List all receive addresses in the wallet
    //QList<ReceiveAddress> listReceiveAddresses();

    // List wallet utxo set
    //QList<Output listUtxo();

    /**
     * Get
     */

    // Get and utxo
    // utxo getUtxo();

    // Generate receive address
    // ReceiveAddress getReceiveAddress();


    //Entry getAndLockEntry();
    //UnspentP2PKHOutput getUtxo(quint64 minimalValue, quint32 minimalNumberOfConfirmations);

    quint64 lastComputedZeroConfBalance();

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
    QByteArray _seed;

    /**
     * State
     */

    // Latest known block height
    qint64 _latestBlockHeight;

    // Value of last run of computeBalance(0), which is initially run in ctr
    quint64 _lastComputedZeroConfBalance;



    // key pools

    // utxo

    /**
    // SQL table schemas
    static QSqlQuery createTransactionTableQuery();
    static QSqlQuery createInputTableQuery();
    static QSqlQuery createScriptTypeTableQuery();
    static QSqlQuery createOutputTableQuery();
    static QSqlQuery createPayerTableQuery();
    static QSqlQuery createSlotTableQuery();
    static QSqlQuery createSlotStateTableQuery();
    static QSqlQuery createPayeeTableQuery();
    static QSqlQuery createPayeeStateTableQuery();

    // SQL insert statement !template!
    // Add make bingings to use

    static QSqlQuery insertReceiveAddressQuery();
    static QSqlQuery insertReceiveAddressPurposeQuery();
    //static QSqlQuery insertPrivateKeyControllingReceiveAddressQuery();
    static QSqlQuery insertTransactionQuery();
    static QSqlQuery insertInputQuery();
    static QSqlQuery insertScriptTypeQuery();
    static QSqlQuery insertOutputQuery();
    static QSqlQuery insertPayerQuery();
    static QSqlQuery insertSlotQuery();
    static QSqlQuery insertSlotStateQuery();
    static QSqlQuery insertPayeeQuery();
    static QSqlQuery insertPayeeStateQuery();
    */
};

#endif // WALLET_HPP
