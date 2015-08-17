/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/OutPoint.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it


namespace Wallet {
namespace OutPoint {

Record::Record(const Coin::TransactionId & transactionId, quint32 outputIndex)
    : _transactionId(transactionId)
    , _outputIndex(outputIndex) {
}

QSqlQuery Record::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unBoundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":transactionId", _transactionId.toByteArray());
    query.bindValue(":outputIndex", _outputIndex);

    return query;
}

Coin::TransactionId Record::transactionId() const {
    return _transactionId;
}

void Record::setTransactionId(const Coin::TransactionId &transactionId) {
    _transactionId = transactionId;
}

quint32 Record::outputIndex() const {
    return _outputIndex;
}

void Record::setOutputIndex(quint32 outputIndex) {
    _outputIndex = outputIndex;
}


QSqlQuery createTableQuery(QSqlDatabase db) {

    QSqlQuery query(db);
    query.prepare(
    "CREATE TABLE OutPoint ( "
        "transactionId       BLOB, "
        "outputIndex         INTEGER, "
        "PRIMARY KEY(transactionId, outputIndex) "
    ")");

    return query;
}

QSqlQuery unBoundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO OutPoint "
        "(transactionId, outputIndex) "
    "VALUES "
        "(:transactionId, :outputIndex) "
    );

    return query;
}

}
}
