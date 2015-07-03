#ifndef WALLET_HPP
#define WALLET_HPP

#include "Network.hpp"

#include <QObject>
#include <QString>
#include <QtSql>
#include <QDateTime>
#include <QByteArray>

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

    // generate receive address

    // get utxo

    // list utxo

    // list receive addresses


signals:


    // new utxo

    // spent utxo

    // double spend detected

    // new transaction added

    // reorg?

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

    static QString createPrivateKeyTableQuery();
    static QString createKeyPurposeTableQuery();
    static QString createReceiveAddressTableQuery();
    static QString createPrivateKeyControllingReceiveAddressTableQuery();
    static QString createTransactionTableQuery();
    static QString createInputTableQuery();
    static QString createScriptTypeTableQuery();
    static QString createOutputTableQuery();
    static QString createPayerTableQuery();
    static QString createSlotTableQuery();
    static QString createSlotStateTableQuery();
    static QString createPayeeTableQuery();
    static QString createPayeeStateTableQuery();
};

#endif // WALLET_HPP
