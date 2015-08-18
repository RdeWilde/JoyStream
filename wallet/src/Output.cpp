/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/Output.hpp>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace Output {

PK::PK() {
}

PK::PK(quint64 value, const QByteArray & scriptPubKey)
    : _value(value)
    , _scriptPubKey(scriptPubKey) {
}

Record::Record() {
}

Record::Record(const PK & pk, const QVariant & keyIndex)
    : _pk(pk)
    , _keyIndex(keyIndex) {
}

bool createTable(QSqlDatabase & db)  {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE Output ( "
        "value           INTEGER, "
        "scriptPubKey    BLOB, "
        "keyIndex        INTEGER     NOT NULL, "
        "PRIMARY KEY(value, scriptPubKey), "
        "FOREIGN KEY(keyIndex) REFERENCES Address(keyIndex) "
    ")");

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}


bool insert(QSqlDatabase & db, const Record & record) {

    // Prepare insert query
    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO Output "
        "(value, scriptPubKey, keyIndex) "
    "VALUES "
        "(:value, :scriptPubKey, :keyIndex) "
    );

    // bind wallet key values
    query.bindValue(":value", record._pk._value);
    query.bindValue(":scriptPubKey", record._pk._scriptPubKey);
    query.bindValue(":keyIndex", record._keyIndex);

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool exists(QSqlDatabase & db, const PK & pk, Record & r) {
    throw std::runtime_error("not implemented");
}

bool exists(QSqlDatabase & db, const PK & pk) {
    Record r;
    return exists(db, pk, r);
}

}
}
