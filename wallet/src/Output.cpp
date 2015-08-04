/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/Output.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

Output::Output(quint64 value, const QByteArray & pubKeyScript, quint64 walletKeyIndex)
    : _value(value)
    , _pubKeyScript(pubKeyScript)
    , _walletKeyIndex(walletKeyIndex) {
}

QSqlQuery Output::createTableQuery(QSqlDatabase db)  {

    return QSqlQuery("\
    CREATE TABLE Output (\
        value           INTEGER,\
        pubKeyScript    BLOB,\
        walletKeyIndex  INTEGER     NOT NULL,\
        PRIMARY KEY(value, pubKeyScript),\
        FOREIGN KEY(walletKeyIndex) REFERENCES WalletAddress(walletKeyIndex)\
    )", db);
}

QSqlQuery Output::unboundedInsertQuery(QSqlDatabase db) {

    return QSqlQuery("\
    INSERT INTO Output \
    (value, pubKeyScript, walletKeyIndex)\
    VALUES\
    (:value, :outPointOutputIndex, :scriptSig, :sequence)\
    ", db);
}

quint64 Output::value() const {
    return _value;
}

void Output::setValue(quint64 value) {
    _value = value;
}

QByteArray Output::pubKeyScript() const {
    return _pubKeyScript;
}

void Output::setPubKeyScript(const QByteArray & pubKeyScript) {
    _pubKeyScript = pubKeyScript;
}

quint64 Output::walletKeyIndex() const {
    return _walletKeyIndex;
}

void Output::setWalletKeyIndex(quint64 walletKeyIndex) {
    _walletKeyIndex = walletKeyIndex;
}
