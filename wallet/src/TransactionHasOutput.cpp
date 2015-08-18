/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/TransactionHasOutput.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace TransactionHasOutput {

Record::PK::PK() {
}

Record::PK::PK(const Coin::TransactionId & transactionId, quint32 index)
    : _transactionId(transactionId)
    , _index(index) {
}

Record::Record() {
}

Record::Record(const PK & pk, const Output::PK & output)
    : _pk(pk)
    , _output(output) {
}

QSqlQuery createTable(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE TransactionHasOutput ( "
        "transactionId   BLOB, "
        "[index]         INTEGER, "
        "value           INTEGER     NOT NULL, "
        "scriptPubKey    BLOB        NOT NULL, "
        "PRIMARY KEY(transactionId, [index]), "
        "FOREIGN KEY(transactionId) REFERENCES [Transaction](transactionId), "
        "FOREIGN KEY(value, scriptPubKey) REFERENCES Output(value, scriptPubKey) "
    ")");

    return query;
}

QSqlQuery Record::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unBoundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":transactionId", _pk._transactionId.toByteArray());
    query.bindValue(":index", _pk._index);
    query.bindValue(":value",  _output._value);
    query.bindValue(":scriptPubKey", _output._scriptPubKey);

    return query;
}

QSqlQuery unBoundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO TransactionHasOutput "
        "(transactionId, [index], value, scriptPubKey)"
    "VALUES "
        "(:transactionId, :index, :value, :scriptPubKey)");

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
