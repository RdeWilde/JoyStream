/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/OutPoint.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::OutPoint

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace OutPoint {

PK::PK() {
}

PK::PK(const Coin::TransactionId & transactionId, quint32 outputIndex)
    : _transactionId(transactionId)
    , _outputIndex(outputIndex) {
}

PK::PK(const Coin::OutPoint & o)
    : PK(uchar_vector(o.hash, Coin::TransactionId::length()), o.index) {
}

Record::Record() {
}

Record::Record(const PK & pk)
    : _pk(pk) {
}

bool createTable(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE OutPoint ( "
        "transactionId       BLOB, "
        "outputIndex         INTEGER, "
        "PRIMARY KEY(transactionId, outputIndex) "
    ")");

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool insert(QSqlDatabase db, const Record & record) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO OutPoint "
        "(transactionId, outputIndex) "
    "VALUES "
        "(:transactionId, :outputIndex) "
    );

    // Bind values to query fields
    query.bindValue(":transactionId", record._pk._transactionId.toByteArray());
    query.bindValue(":outputIndex", record._pk._outputIndex);

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool exists(QSqlDatabase db, const PK & pk, Record & r) {
    throw std::runtime_error("not implemented");
}

bool exists(QSqlDatabase db, const PK & pk) {
    Record r;
    return exists(db, pk, r);
}

}
}
