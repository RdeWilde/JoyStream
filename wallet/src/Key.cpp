/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#include <wallet/Key.hpp>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant> // QSqlQuery::bind needs it

/**
 * Wallet::Key::Record
 */

namespace Wallet {
namespace Key {

Record::Record(quint64 index, const Coin::PrivateKey & privateKey, const QDateTime & generated, bool issued)
    : _index(index)
    , _sk(privateKey)
    , _generated(generated)
    , _issued(issued) {
}

QSqlQuery Record::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery(db);

    // bind wallet key values
    query.bindValue(":index", static_cast<uint>(_index));
    query.bindValue(":privateKey", _sk.toByteArray());
    query.bindValue(":generated", _generated.toMSecsSinceEpoch());
    query.bindValue(":issued", _issued);

    //query.bindValue(":keyPurposeId", WalletKey::encodePurpose(walletKey.purpose()));

    return query;
}

quint64 Record::index() const {
    return _index;
}

void Record::setIndex(quint64 index) {
    _index = index;
}

Coin::PrivateKey Record::sk() const {
    return _sk;
}

void Record::setSk(const Coin::PrivateKey & sk) {
    _sk = sk;
}

QDateTime Record::generated() const {
    return _generated;
}

void Record::setGenerated(const QDateTime & generated) {
    _generated = generated;
}

bool Record::issued() const {
    return _issued;
}

void Record::setIssued(bool issued) {
    _issued = issued;
}

/**
Purpose Record::purpose() const {
    return _purpose;
}

void Record::setPurpose(Purpose purpose) {
    _purpose = purpose;
}
*/

/**
 * Wallet::Key
 */

QSqlQuery createTableQuery(QSqlDatabase db) {

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

    return query;
}

QSqlQuery unboundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO Key "
        "([index], privateKey, generated, issued) "
    "VALUES "
        "(:index, :privateKey, :generated, :issued) "
    );

    return query;
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
