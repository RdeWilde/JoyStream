/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/TransactionHasInput.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

TransactionHasInput::TransactionHasInput(const Coin::TransactionId & transactionId, const Input & input)
    : _transactionId(transactionId)
    , _input(input) {
}

QSqlQuery TransactionHasInput::createTableQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE TransactionHasInput ( "
        "transactionId               BLOB        NOT NULL, "
        "index                       INTEGER     NOT NULL, "
        "outPointTransactionId       BLOB        NOT NULL, "
        "outPointOutputIndex         INTEGER     NOT NULL, "
        "scriptSig                   BLOG        NOT NULL, "
        "sequence                    INTEGER     NOT NULL, "
        "PRIMARY KEY(transactionId, index), "
        "FOREIGN KEY transactionId REFERENCES Transaction(transactionId), "
        "FOREIGN KEY (outPointTransactionId, outPointOutputIndex, scriptSig, sequence) REFERENCES Input(outPointTransactionId, outPointOutputIndex, scriptSig, sequence) "
    ")");

    return query;
}

QSqlQuery TransactionHasInput::unboundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare (
    "INSERT INTO TransactionHasInput "
        "(transactionId, index, outPointTransactionId, outPointOutputIndex, scriptSig, sequence) "
    "VALUES "
        "(:transactionId, :index, :outPointTransactionId, :outPointOutputIndex, :scriptSig, :sequence) ");

    return query;
}

QSqlQuery TransactionHasInput::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":transactionId", _transactionId.toByteArray());
    query.bindValue(":index", _index);

    OutPoint outPoint = _input.outPoint();
    query.bindValue(":outPointTransactionId", outPoint.transactionId().toByteArray());
    query.bindValue(":outPointOutputIndex", outPoint.outputIndex());
    query.bindValue(":scriptSig",  _input.scriptSig());
    query.bindValue(":sequence", _input.sequence());

    return query;
}

Coin::TransactionId TransactionHasInput::transactionId() const {
    return _transactionId;
}

void TransactionHasInput::setTransactionId(const Coin::TransactionId & transactionId) {
    _transactionId = transactionId;
}

quint32 TransactionHasInput::index() const {
    return _index;
}

void TransactionHasInput::setIndex(quint32 index) {
    _index = index;
}

Input TransactionHasInput::input() const {
    return _input;
}

void TransactionHasInput::setInput(const Input & input) {
    _input = input;
}
