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

Record::Record() {
}

Record::Record(PK keyIndex, const Coin::P2PKHAddress & address)
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

/**
 * Wallet::Address
 */

bool createTable(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE Address ( "
        "keyIndex        INTEGER, "
        "address         TEXT        NOT NULL, "
        "PRIMARY KEY(keyIndex), "
        "FOREIGN KEY(keyIndex) REFERENCES Key, " // (index)
        "UNIQUE(address) "
    ")");

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool insert(QSqlDatabase db, const Record & record) {

    // Create insert query
    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO Address "
        "(keyIndex, address) "
    "VALUES "
        "(:keyIndex, :address) "
    );

    // bind wallet key values
    uint d = static_cast<uint>(record._keyIndex);
    query.bindValue(":keyIndex", d);
    query.bindValue(":address", record._address.toBase58CheckEncoding());

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
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

bool exists(QSqlDatabase & db, const PK & pk, Record & r) {
    throw std::runtime_error("not implemented");
}

bool exists(QSqlDatabase & db, const PK & pk) {
    Record r;
    return exists(db, pk, r);
}

bool findFromAddress(QSqlDatabase & db, const Coin::P2PKHAddress & address, Record & r) {

    // Select record with given address field
    QSqlQuery query(db);
    query.prepare("SELECT * FROM Address WHERE address = :address");
    query.bindValue(":address", address.toBase58CheckEncoding());
    query.exec();

    QSqlError e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);

    if(query.size() == 0)
        return false;
    else if(query.size() == 1) {

        // Grab record
        QSqlRecord sqlRecord = query.record();

        // Write record to destination
        r = Record(sqlRecord);

        return true;

    } else
        Q_ASSERT(false); // unique field, so at most one result
}

}
}
