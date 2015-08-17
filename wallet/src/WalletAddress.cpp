/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/WalletAddress.hpp>

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant> // QSqlQuery::bind needs it

WalletAddress::WalletAddress(quint64 walletKeyIndex, const Coin::P2PKHAddress & address)
    : _walletKeyIndex(walletKeyIndex)
    , _address(address) {
}

WalletAddress::WalletAddress(const QSqlRecord & record) {

    // walletKeyIndex column
    bool ok;
    _walletKeyIndex = record.value("walletKeyIndex").toULongLong(&ok);
    Q_ASSERT(ok);

    // address
   _address = Coin::P2PKHAddress::fromBase58CheckEncoding(record.value("address").toString());
}

QSqlQuery WalletAddress::createTableQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE WalletAddress ( "
        "walletKeyIndex  INTEGER, "
        "address         TEXT        NOT NULL, "
        "PRIMARY KEY(walletKeyIndex), "
        "FOREIGN KEY(walletKeyIndex) REFERENCES WalletKey, " // (index)
        "UNIQUE(address) "
    ")");

    return query;
}

QSqlQuery WalletAddress::unboundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO WalletAddress "
        "(walletKeyIndex, address) "
    "VALUES "
        "(:walletKeyIndex, :address) "
    );

    return query;
}

QSqlQuery WalletAddress::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery(db);

    // bind wallet key values
    uint d = static_cast<uint>(_walletKeyIndex);
    query.bindValue(":walletKeyIndex", d);
    query.bindValue(":address", _address.toBase58CheckEncoding());

    return query;
}

QList<WalletAddress> WalletAddress::listAllAddresss(QSqlDatabase db) {

    // List of addresses
    QList<WalletAddress> list;

    // Select count
    QSqlQuery query("SELECT * FROM WalletAddress", db);
    QSqlError e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);

    // Iterate records
    while(query.next()) {

        // Grab record
        QSqlRecord record = query.record();

        // Create wallet address
        WalletAddress walletAddress(record);

        // Add to ist
        list.append(walletAddress);
    }

    return list;
}

quint64 WalletAddress::walletKeyIndex() const {
    return _walletKeyIndex;
}

void WalletAddress::setWalletKeyIndex(quint64 walletKeyIndex) {
    _walletKeyIndex = walletKeyIndex;
}

Coin::P2PKHAddress WalletAddress::address() const {
    return _address;
}

void WalletAddress::setAddress(const Coin::P2PKHAddress & address) {
    _address = address;
}
