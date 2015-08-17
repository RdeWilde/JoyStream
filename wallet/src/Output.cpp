/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/Output.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace Output {

Record::Record(quint64 value, const QByteArray & pubKeyScript, quint64 keyIndex)
    : _value(value)
    , _pubKeyScript(pubKeyScript)
    , _keyIndex(keyIndex) {
}

quint64 Record::value() const {
    return _value;
}

void Record::setValue(quint64 value) {
    _value = value;
}

QByteArray Record::pubKeyScript() const {
    return _pubKeyScript;
}

void Record::setPubKeyScript(const QByteArray & pubKeyScript) {
    _pubKeyScript = pubKeyScript;
}

quint64 Record::keyIndex() const {
    return _keyIndex;
}

void Record::setKeyIndex(quint64 keyIndex) {
    _keyIndex = keyIndex;
}

QSqlQuery createTableQuery(QSqlDatabase db)  {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE Output ( "
        "value           INTEGER, "
        "pubKeyScript    BLOB, "
        "keyIndex        INTEGER     NOT NULL, "
        "PRIMARY KEY(value, pubKeyScript), "
        "FOREIGN KEY(keyIndex) REFERENCES Address(keyIndex) "
    ")");

    return query;
}

QSqlQuery unBoundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO Output "
        "(value, pubKeyScript, keyIndex) "
    "VALUES "
        "(:value, :pubKeyScript, :keyIndex) "
    );

    return query;
}

}
}
