/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 13 2015
 */

#include <wallet/Metadata.hpp>
#include <wallet/Utilities.hpp> // date conversion
#include <common/Seed.hpp>

#include <QByteArray>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>
#include <QSqlError>

namespace Wallet {

//const QByteArray Metadata::_networkKey = QByteArray("network");
//const QByteArray Metadata::_seedKey = QByteArray("seed");
//const QByteArray Metadata::_createdKey = QByteArray("created");

void Metadata::createKeyValueStore(QSqlDatabase db, const Coin::Seed & seed, Coin::Network network, const QDateTime & created, const quint32 version) {

    // Create (two-column) table

    // Try to execute query
    QSqlQuery result = db.exec(
    "CREATE TABLE Metadata ("
        "[key] BLOB,"
        "value BLOB,"
        "PRIMARY KEY([key])"
    ")");

    Q_ASSERT(result.lastError().type() == QSqlError::NoError);

    // Create one row per key-val pair you wish to represents

    // Create query
    QSqlQuery query = unBoundedInsertQuery(db);

    // Error variable for each query
    QSqlError e;

    // network
    query.bindValue(":key", _networkKey);
    query.bindValue(":value", encodeNetwork(network));
    query.exec();
    e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);

    // seed
    query.bindValue(":key", _seedKey);
    query.bindValue(":value", seed.toByteArray());
    query.exec();
    e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);

    // created
    query.bindValue(":key", _createdKey);
    query.bindValue(":value", Wallet::Utilities::encodeDateTime(created));
    query.exec();
    e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);

    // version
    query.bindValue(":key", _versionKey);
    query.bindValue(":value", QByteArray::number(version));
    query.exec();
    e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);
}

QByteArray Metadata::get(QSqlDatabase db, const QByteArray & key) {

    // Get select query
    QSqlQuery query = Metadata::unBoundedSelectQuery(db);
    query.bindValue(":key", key);

    // Make select query
    query.exec();

    QSqlError e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);
    if(query.first()){
        QVariant valueField = query.value(0);
        return valueField.toByteArray();
    } else {
        return QByteArray();
    }
}

void Metadata::update(QSqlDatabase db, const QByteArray & key, const QByteArray & value) {

    // Get update query
    QSqlQuery query = Metadata::unBoundedUpdateQuery(db);
    query.bindValue(":key", key);
    query.bindValue(":value", value);

    // Make update query
    query.exec();

    QSqlError e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);
}

Coin::Network Metadata::getNetwork(QSqlDatabase db) {

    // Read key-val pair
    QByteArray network = Metadata::get(db, _networkKey);

    // Decode and return
    return decodeNetwork(network);
}

void Metadata::setNetwork(QSqlDatabase db, Coin::Network network) {
    Metadata::update(db, _networkKey, encodeNetwork(network));
}

Coin::Seed Metadata::getSeed(QSqlDatabase db) {

    // Read key-val pair
    QByteArray seed = Metadata::get(db, _seedKey);

    // Decode and return
    return Coin::Seed(seed);
}

void Metadata::setSeed(QSqlDatabase db, const Coin::Seed & seed) {
    Metadata::update(db, _seedKey, seed.toByteArray());
}

QDateTime Metadata::getCreated(QSqlDatabase db) {

    // Read key-val pair
    QByteArray created = Metadata::get(db, _createdKey);

    // Decode and return
    return Wallet::Utilities::decodeDateTime(created);
}

void Metadata::setCreated(QSqlDatabase db, const QDateTime & created) {
    Metadata::update(db, _createdKey, QByteArray::number(created.toMSecsSinceEpoch()));
}

quint32 Metadata::getVersion(QSqlDatabase db) {

    // Read key-val pair
    QByteArray version = Metadata::get(db, _versionKey);

    // Decode and return
    return version.toInt();
}

QSqlQuery Metadata::unBoundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);
    query.prepare("INSERT INTO Metadata (key, value) VALUES (:key, :value)");

    return query;
}

QSqlQuery Metadata::unBoundedUpdateQuery(QSqlDatabase db) {

    QSqlQuery query(db);
    query.prepare("UPDATE Metadata SET value = :value WHERE key = :key");

    return query;
}

QSqlQuery Metadata::unBoundedSelectQuery(QSqlDatabase db) {

    QSqlQuery query(db);
    query.prepare("SELECT value FROM Metadata WHERE [key] = :key");

    return query;
}

QByteArray Metadata::encodeNetwork(Coin::Network network) {

    switch(network) {
        case Coin::Network::testnet3: return QByteArray::number(0);
        case Coin::Network::mainnet: return QByteArray::number(1);
        case Coin::Network::regtest: return QByteArray::number(2);
        default:
                Q_ASSERT(false);
    }
}

Coin::Network Metadata::decodeNetwork(const QByteArray & blob) {

    bool ok;
    uint encodedNetwork = blob.toUInt(&ok);

    Q_ASSERT(ok);

    switch(encodedNetwork) {

        case 0: return Coin::Network::testnet3;
        case 1: return Coin::Network::mainnet;
        case 2: return Coin::Network::regtest;
        default:
            Q_ASSERT(false);
    }
}

}
