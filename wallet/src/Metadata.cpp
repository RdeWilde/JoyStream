/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 13 2015
 */

#include <wallet/Metadata.hpp>
#include <wallet/Seed.hpp>
#include <wallet/WalletUtilities.hpp> // date conversion

#include <QByteArray>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>
#include <QSqlError>

const QByteArray Metadata::_networkKey = QByteArray("network");
const QByteArray Metadata::_seedKey = QByteArray("seed");
const QByteArray Metadata::_createdKey = QByteArray("created");

void Metadata::createKeyValueStore(QSqlDatabase db, const Seed & seed, Coin::Network network, const QDateTime & created) {

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
    QSqlQuery query = createInsertQuery(db);

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
    query.bindValue(":value", WalletUtilities::encodeDateTime(created));
    query.exec();

    e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);
}

QByteArray Metadata::get(QSqlDatabase db, const QByteArray & key) {

    // Get select query
    QSqlQuery query = Metadata::createSelectQuery(db);
    query.bindValue(":key", key);

    // Make select query
    query.exec();

    QSqlError e = query.lastError();
    Q_ASSERT(e.type() == QSqlError::NoError);
    Q_ASSERT(query.first());

    // Return result
    QVariant valueField = query.value(0);
    return valueField.toByteArray();
}

void Metadata::update(QSqlDatabase db, const QByteArray & key, const QByteArray & value) {

    // Get update query
    QSqlQuery query = Metadata::createUpdateQuery(db);
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

Seed Metadata::getSeed(QSqlDatabase db) {

    // Read key-val pair
    QByteArray seed = Metadata::get(db, _seedKey);

    // Decode and return
    return Seed(seed);
}

void Metadata::setSeed(QSqlDatabase db, const Seed & seed) {
    Metadata::update(db, _seedKey, seed.toByteArray());
}

QDateTime Metadata::getCreated(QSqlDatabase db) {

    // Read key-val pair
    QByteArray created = Metadata::get(db, _createdKey);

    // Decode and return
    return WalletUtilities::decodeDateTime(created);
}

void Metadata::setCreated(QSqlDatabase db, const QDateTime & created) {
    Metadata::update(db, _createdKey, QByteArray::number(created.toMSecsSinceEpoch()));
}

QSqlQuery Metadata::createInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);
    query.prepare("INSERT INTO Metadata (key, value) VALUES (:key, :value)");

    return query;
}

QSqlQuery Metadata::createUpdateQuery(QSqlDatabase db) {

    QSqlQuery query(db);
    query.prepare("UPDATE Metadata SET value = :value WHERE key = :key");

    return query;
}

QSqlQuery Metadata::createSelectQuery(QSqlDatabase db) {

    QSqlQuery query(db);
    query.prepare("SELECT value FROM Metadata WHERE [key] = :key");

    return query;
}

QByteArray Metadata::encodeNetwork(Coin::Network network) {

    switch(network) {
        case Coin::Network::testnet3: return QByteArray::number(0);
        case Coin::Network::mainnet: return QByteArray::number(1);
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
        default:
            Q_ASSERT(false);
    }
}
