/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#ifndef WALLET_HPP
#define WALLET_HPP

#include "Network.hpp"

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

    ~Wallet();

    // Create an empty wallet
    static void createEmptyWallet(const QString & walletFile, Network network, const QByteArray & seed);

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

public slots:

private:

    // Wallet (SQLite database) file
    QString _walletFile;

    // Database connection
    QSqlDatabase _db;

    // Network wallet corresponds to
    Network _network;

    // Time when wallet was created
    QDateTime _created;

    // Seed
    QByteArray _seed;

    // key pools

    // utxo

    /**
     * SQL table schemas
     */
    static QSqlQuery createWalletKeyTableQuery();
    static QSqlQuery createReceiveAddressTableQuery();
    static QSqlQuery createReceiveAddressPurposeTableQuery();
    static QSqlQuery createWalletKeyControllingReceiveAddressTableQuery();
    static QSqlQuery createTransactionTableQuery();
    static QSqlQuery createInputTableQuery();
    static QSqlQuery createScriptTypeTableQuery();
    static QSqlQuery createOutputTableQuery();
    static QSqlQuery createPayerTableQuery();
    static QSqlQuery createSlotTableQuery();
    static QSqlQuery createSlotStateTableQuery();
    static QSqlQuery createPayeeTableQuery();
    static QSqlQuery createPayeeStateTableQuery();

    /**
     * SQL insert statement !template!
     * Add make bingings to use
     */
    static QSqlQuery insertWalletKeyQuery();
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

    /**
     * SQL insert queries
     */
    static QSqlQuery insertQuery(const WalletKey & walletKey);
    static QSqlQuery insertQuery(const ReceiveAddress & receiveAddress);
    //
    static QSqlQuery insertQuery(const Payer & payer);
    static QSqlQuery insertQuery(const Payee & payee);
    static QSqlQuery insertQuery(const Slot & slot);
};

#endif // WALLET_HPP
