/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/Address.hpp>

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace Address {

/**
 * Wallet::Address::Record
 */

Record::Record(quint64 keyIndex, const Coin::P2PKHAddress & address)
    : _keyIndex(keyIndex)
    , _address(address) {
}

Record::Record(const QSqlRecord & record) {

    // walletKeyIndex column
    bool ok;
    _keyIndex = record.value("keyIndex").toULongLong(&ok);
    Q_ASSERT(ok);

    // address
   _address = Coin::P2PKHAddress::fromBase58CheckEncoding(record.value("address").toString());
}

QSqlQuery Record::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery(db);

    // bind wallet key values
    uint d = static_cast<uint>(_keyIndex);
    query.bindValue(":keyIndex", d);
    query.bindValue(":address", _address.toBase58CheckEncoding());

    return query;
}

quint64 Record::keyIndex() const {
    return _keyIndex;
}

void Record::setKeyIndex(quint64 keyIndex) {
    _keyIndex = keyIndex;
}

Coin::P2PKHAddress Record::address() const {
    return _address;
}

void Record::setAddress(const Coin::P2PKHAddress & address) {
    _address = address;
}

/**
 * Wallet::Address
 */

QSqlQuery createTableQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE Address ( "
        "keyIndex        INTEGER, "
        "address         TEXT        NOT NULL, "
        "PRIMARY KEY(keyIndex), "
        "FOREIGN KEY(keyIndex) REFERENCES Key, " // (index)
        "UNIQUE(address) "
    ")");

    return query;
}

QSqlQuery unboundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO Address "
        "(keyIndex, address) "
    "VALUES "
        "(:keyIndex, :address) "
    );

    return query;
}


QList<Record> allRecords(QSqlDatabase db) {

    // List of addresses
    QList<Wallet::Address::Record> list;

    // Select count
    QSqlQuery query("SELECT * FROM Address", db);
    QSqlError e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);

    // Iterate records
    while(query.next()) {

        // Grab record
        QSqlRecord sqlRecord = query.record();

        // Create record
        Record record(sqlRecord);

        // Add to list
        list.append(record);
    }

    return list;
}

}
}
