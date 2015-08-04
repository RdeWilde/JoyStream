/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/TransactionHasOutput.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

// Construct from members
TransactionHasOutput::TransactionHasOutput(const Coin::TransactionId & transactionId, quint32 index, const Output & output)
    : _transactionId(transactionId)
    , _index(index)
    , _output(output) {
}

// Constructor from record
// TransactionHasOutput(const QSqlRecord & record);

QSqlQuery TransactionHasOutput::createTableQuery(QSqlDatabase db) {

    return QSqlQuery("\
    CREATE TABLE TransactionHasInput (\
        transactionId   BLOB,\
        index           INTEGER,\
        value           INTEGER     NOT NULL,\
        pubKeyScript    BLOB        NOT NULL,\
        PRIMARY KEY(transactionId, index),\
        FOREIGN KEY transactionId REFERENCES Transaction(transactionId),\
        FOREIGN KEY (value, pubKeyScript) REFERENCES Output(value, pubKeyScript)\
    )", db);
}

QSqlQuery TransactionHasOutput::unboundedInsertQuery(QSqlDatabase db) {

    return QSqlQuery("\
    INSERT INTO TransactionHasInput \
    (transactionId, index, value, pubKeyScript)\
    VALUES\
    (:transactionId, :index, :value, :pubKeyScript)\
    ", db);
}

QSqlQuery TransactionHasOutput::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":transactionId", _transactionId.toByteArray());
    query.bindValue(":index", _index);
    query.bindValue(":value",  _output.value());
    query.bindValue(":pubKeyScript", _output.pubKeyScript());

    return query;
}

Coin::TransactionId TransactionHasOutput::transactionId() const {
    return _transactionId;
}

void TransactionHasOutput::setTransactionId(const Coin::TransactionId & transactionId) {
    _transactionId = transactionId;
}

quint32 TransactionHasOutput::index() const {
    return _index;
}

void TransactionHasOutput::setIndex(quint32 index) {
    _index = index;
}

Output TransactionHasOutput::output() const {
    return _output;
}

void TransactionHasOutput::setOutput(const Output & output) {
    _output = output;
}
