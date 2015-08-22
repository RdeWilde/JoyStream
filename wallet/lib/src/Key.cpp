/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#include <wallet/Key.hpp>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant> // QSqlQuery::bind needs it

/**
 * Wallet::Key::Record
 */

namespace Wallet {
namespace Key {

Record::Record() {
}

Record::Record(PK index,
               const Coin::PrivateKey & privateKey,
               const QDateTime & generated,
               bool issued)
    : _index(index)
    , _privateKey(privateKey)
    , _generated(generated)
    , _issued(issued) {
}

Record::Record(const QSqlRecord & record) {

    _index = record.value("index").toULongLong();
    _privateKey = record.value("privateKey").toByteArray();

    QString generated = record.value("generated").toString();
    _generated = QDateTime::fromString(generated, Qt::DateFormat::ISODate);
    _issued = record.value("issued").toBool();
}

bool createTable(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE Key ( "
        "[index]         INTEGER, "
        "privateKey      BLOB        NOT NULL, "
        "generated       DATETIME    NOT NULL, "
        "issued          BOOL        NOT NULL, "
        "PRIMARY KEY([index]) "
    ")");

    // PRIMARY KEY([index]) AUTOINCREMENT

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool insert(QSqlDatabase db, const Record & r) {

    // Create query
    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO Key "
        "([index], privateKey, generated, issued) "
    "VALUES "
        "(:index, :privateKey, :generated, :issued) "
    );

    // bind wallet key values
    query.bindValue(":index", static_cast<uint>(r._index));
    query.bindValue(":privateKey", r._privateKey.toByteArray());
    query.bindValue(":generated", r._generated.toMSecsSinceEpoch());
    query.bindValue(":issued", r._issued);

    //query.bindValue(":keyPurposeId", WalletKey::encodePurpose(walletKey.purpose()));

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

quint64 maxIndex(QSqlDatabase db) {

    // Select max value in column
    QSqlQuery query("SELECT MAX([index]) FROM Key", db);

    QSqlError e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);
    Q_ASSERT(query.first());

    // Grab result
    QVariant result = query.value(0);

    // If its NULL, then the table was empty
    if(result.isNull())
        throw std::runtime_error("Table was empty");
    else {

        bool ok;
        quint64 maxIndex = result.toULongLong(&ok);

        Q_ASSERT(ok);

        return maxIndex;
    }
}

quint64 numberOfKeysInWallet(QSqlDatabase db) {

    // Select row count from table
    QSqlQuery query("SELECT COUNT(*) FROM Key", db);

    QSqlError e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);
    Q_ASSERT(query.first());

    bool ok;
    quint64 numberOfKeysInWallet = query.value(0).toULongLong(&ok);

    Q_ASSERT(ok);

    return numberOfKeysInWallet;
}

bool exists(QSqlDatabase db, const PK & pk, Record & r) {

    // Prepare select query
    QSqlQuery query(db);

    query.prepare("SELECT * FROM Key WHERE [index] = :index");

    // Bind values to query fields
    query.bindValue(":index", pk);

    query.exec();

    Q_ASSERT(query.lastError().type() == QSqlError::NoError);

    if(!query.first())
        return false;

    r = Record(query.record());

    Q_ASSERT(!query.next());

    return true;
}

bool exists(QSqlDatabase db, const PK & pk) {

    Record r;
    return exists(db, pk, r);
}

/*
quint8 WalletKey::encodePurpose(Purpose purpose) {

    switch(purpose) {
        case Purpose::Receive: return 0;
        case Purpose::ContractChange: return 1;
        case Purpose::PayerInContractOutput: return 2;
        case Purpose::ContractFinal: return 3;
        case Purpose::PayeeInContractOutput: return 4;
        case Purpose::ContractPayment: return 5;

        default:
            Q_ASSERT(false);
    }
}

WalletKey::Purpose WalletKey::decodePurpose(quint8 encoded) {

    switch(encoded) {
        case 0: return Purpose::Receive;
        case 1: return Purpose::ContractChange;
        case 2: return Purpose::PayerInContractOutput;
        case 3: return Purpose::ContractFinal;
        case 4: return Purpose::PayeeInContractOutput;
        case 5: return Purpose::ContractPayment;

        default:
            Q_ASSERT(false);
    }
}
*/

}
}
