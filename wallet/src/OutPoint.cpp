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
#include <QSqlRecord>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace OutPoint {

PK::PK() {
}

PK::PK(const Coin::TransactionId & transactionId, quint32 outputIndex)
    : _transactionId(transactionId)
    , _index(outputIndex) {
}

PK::PK(const Coin::OutPoint & o)
    : PK(Coin::TransactionId::fromRPCByteOrder(uchar_vector(o.hash, Coin::TransactionId::length())), o.index) {
}

Record::Record() {
}

Record::Record(const PK & pk)
    : _pk(pk) {
}

Record::Record(const QSqlRecord & record) {

    Coin::TransactionId transactionId = Coin::TransactionId::fromRPCByteOrder(record.value("transactionId").toByteArray());
    quint32 index = record.value("index").toUInt();

    _pk = PK(transactionId, index);
}

Coin::OutPoint Record::toOutPoint() const {
    return Coin::OutPoint(_pk._transactionId.toUCharVector(), _pk._index);
}

bool createTable(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE OutPoint ( "
        "transactionId       BLOB, "
        "[index]             INTEGER, "
        "PRIMARY KEY(transactionId, [index]) "
    ")");

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool insert(QSqlDatabase db, const Record & record) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO OutPoint "
        "(transactionId, [index]) "
    "VALUES "
        "(:transactionId, :index) "
    );

    // Bind values to query fields
    query.bindValue(":transactionId", record._pk._transactionId.toByteArray());
    query.bindValue(":index", record._pk._index);

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool exists(QSqlDatabase db, const PK & pk, Record & r) {

    // Prepare select query
    QSqlQuery query(db);

    query.prepare("SELECT * FROM OutPoint WHERE transactionId = :transactionId AND [index] = :index");

    // Bind values to query fields
    query.bindValue(":transactionId", pk._transactionId.toByteArray());
    query.bindValue(":index", pk._index);

    query.exec();

    Q_ASSERT(query.lastError().type() == QSqlError::NoError);

    if(!query.first())
        return false;

    r = Record(query.record());

    Q_ASSERT(!query.next());

    return true;
}

bool exists(QSqlDatabase db, const PK & pk) {
    Record r;
    return exists(db, pk, r);
}

}
}
