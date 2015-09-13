/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 13 2015
 */

#ifndef WALLET_METADATA_HPP
#define WALLET_METADATA_HPP

#include <common/Network.hpp>

#include <QByteArray>

namespace Coin {
    class Seed;
}

class QSqlQuery;
class QSqlDatabase;
class QDateTime;

namespace Wallet {
namespace Metadata {

    // Hold wallet metadata.
    // Key value store built on top of sql table, bit weird.

    // Create key-value store and set initial values
    void createKeyValueStore(QSqlDatabase db, const Coin::Seed & seed, Coin::Network network, const QDateTime & created);

    // Key-val getter
    QByteArray get(QSqlDatabase db, const QByteArray & key);

    // Key-val getters and updaters
    void update(QSqlDatabase db, const QByteArray & key, const QByteArray & value);

    // setter: Do later if requried
    //static QByteArray set(QSqlDatabase db, const QByteArray & key, const QByteArray & value);

    // Getters and setters (setters assume row already exists for key)
    Coin::Network getNetwork(QSqlDatabase db);
    void setNetwork(QSqlDatabase db, Coin::Network network);

    Coin::Seed getSeed(QSqlDatabase db);
    void setSeed(QSqlDatabase db, const Coin::Seed & seed);

    QDateTime getCreated(QSqlDatabase db);
    void setCreated(QSqlDatabase db, const QDateTime & created);

    // Generates an unbounded query
    QSqlQuery unBoundedInsertQuery(QSqlDatabase db);
    QSqlQuery unBoundedUpdateQuery(QSqlDatabase db);
    QSqlQuery unBoundedSelectQuery(QSqlDatabase db);

    // Key used to store corresponding
    const QByteArray _networkKey = QByteArray("network");
    const QByteArray _seedKey = QByteArray("seed");
    const QByteArray _createdKey = QByteArray("created");

    // Encode network as blob
    QByteArray encodeNetwork(Coin::Network network);
    Coin::Network decodeNetwork(const QByteArray & blob);


}
}

#endif // WALLET_METADATA_HPP
