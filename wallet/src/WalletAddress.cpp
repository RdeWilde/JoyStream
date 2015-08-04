/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/WalletAddress.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

WalletAddress::WalletAddress(quint64 walletKeyIndex, const Coin::P2PKHAddress & address)
    : _walletKeyIndex(walletKeyIndex)
    , _address(address) {
}

QSqlQuery WalletAddress::createTableQuery() {

    return QSqlQuery("\
    CREATE TABLE WalletAddress (\
        walletKeyIndex  INTEGER,\
        address         BLOB        NOT NULL,\
        PRIMARY KEY(walletKeyIndex),\
        FOREIGN KEY REFERENCES walletKeyIndex(id),\
        UNIQUE(address)\
    )");
}

QSqlQuery WalletAddress::unboundedInsertQuery() {

    return QSqlQuery("\
                     INSERT INTO WalletAddress \
                     (walletKeyIndex, address)\
                     VALUES\
                     (:walletKeyIndex, :address)\
                     ");
}

QSqlQuery WalletAddress::insertQuery() {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery();

    // bind wallet key values
    uint d = static_cast<uint>(_walletKeyIndex);
    query.bindValue(":walletKeyIndex", QVariant(d));
    query.bindValue(":address", Coin::uchar_vector_to_QByteArray(_address.raw()));

    return query;
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
