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

// Construct from members
Record::Record(const Coin::TransactionId & transactionId, quint32 index, const Output::Record & output)
    : _transactionId(transactionId)
    , _index(index)
    , _output(output) {
}

// Constructor from record
// Record(const QSqlRecord & record);

QSqlQuery Record::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unBoundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":transactionId", _transactionId.toByteArray());
    query.bindValue(":index", _index);
    query.bindValue(":value",  _output.value());
    query.bindValue(":pubKeyScript", _output.pubKeyScript());

    return query;
}

Coin::TransactionId Record::transactionId() const {
    return _transactionId;
}

void Record::setTransactionId(const Coin::TransactionId & transactionId) {
    _transactionId = transactionId;
}

quint32 Record::index() const {
    return _index;
}

void Record::setIndex(quint32 index) {
    _index = index;
}

Output::Record Record::outPut() const {
    return _output;
}

void Record::setOutput(const Output::Record & output) {
    _output = output;
}

QSqlQuery createTableQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE TransactionHasOutput ( "
        "transactionId   BLOB, "
        "[index]         INTEGER, "
        "value           INTEGER     NOT NULL, "
        "pubKeyScript    BLOB        NOT NULL, "
        "PRIMARY KEY(transactionId, [index]), "
        "FOREIGN KEY(transactionId) REFERENCES [Transaction](transactionId), "
        "FOREIGN KEY(value, pubKeyScript) REFERENCES Output(value, pubKeyScript) "
    ")");

    return query;
}

QSqlQuery unBoundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO TransactionHasOutput "
        "(transactionId, [index], value, pubKeyScript)"
    "VALUES "
        "(:transactionId, :index, :value, :pubKeyScript)");

    return query;
}

}
}
