/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 13 2015
 */

#ifndef METADATA_HPP
#define METADATA_HPP

#include <common/CoinWrappers.hpp>

class Seed;
class QSqlQuery;
class QSqlDatabase;
class QByteArray;
class QDateTime;

// Hold wallet metadata.
// Key value store built on top of sql table, bit weird.
class Metadata {

public:

    // Create key-value store and set initial values
    static void createKeyValueStore(QSqlDatabase db, const Seed & seed, Coin::Network network, const QDateTime & created);

    // Key-val getter
    static QByteArray get(QSqlDatabase db, const QByteArray & key);

    // Key-val getters and updaters
    static void update(QSqlDatabase db, const QByteArray & key, const QByteArray & value);

    // setter: Do later if requried
    //static QByteArray set(QSqlDatabase db, const QByteArray & key, const QByteArray & value);

    // Getters and setters (setters assume row already exists for key)
    static Coin::Network getNetwork(QSqlDatabase db);
    static void setNetwork(QSqlDatabase db, Coin::Network network);

    static Seed getSeed(QSqlDatabase db);
    static void setSeed(QSqlDatabase db, const Seed & seed);

    static QDateTime getCreated(QSqlDatabase db);
    static void setCreated(QSqlDatabase db, const QDateTime & created);

private:

    // Generates an unbounded query
    static QSqlQuery createInsertQuery(QSqlDatabase db);
    static QSqlQuery createUpdateQuery(QSqlDatabase db);
    static QSqlQuery createSelectQuery(QSqlDatabase db);

    // Key used to store corresponding
    static const QByteArray _networkKey;
    static const QByteArray _seedKey;
    static const QByteArray _createdKey;

    // Encode network as blob
    static QByteArray encodeNetwork(Coin::Network network);
    static Coin::Network decodeNetwork(const QByteArray & blob);

    static QByteArray encodeDateTime(const QDateTime & dateTime);
    static QDateTime decodeDateTime(const QByteArray & encodedDateTime);
};

#endif // METADATA_HPP
