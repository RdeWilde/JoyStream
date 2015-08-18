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

Record::PK::PK() {
}

Record::PK::PK(quint64 value, const QByteArray & pubKeyScript)
    : _value(value)
    , _pubKeyScript(pubKeyScript) {
}

Record::Record() {
}

Record::Record(const PK & pk, quint64 keyIndex)
    : _pk(pk)
    , _keyIndex(keyIndex) {
}

QSqlQuery Record::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unBoundedInsertQuery(db);

    // bind wallet key values
    query.bindValue(":value", _pk._value);
    query.bindValue(":pubKeyScript", _pk._pubKeyScript);
    query.bindValue(":keyIndex", _keyIndex);

    return query;
}

QSqlQuery createTable(QSqlDatabase db)  {

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

bool exists(QSqlDatabase & db, const Record::PK & pk, Record & r) {
    throw std::runtime_error("not implemented");
}

bool exists(QSqlDatabase & db, const Record::PK & pk) {
    Record r;
    return exists(db, pk, r);
}

}
}
