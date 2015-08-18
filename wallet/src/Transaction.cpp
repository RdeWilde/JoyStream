/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/Transaction.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::Transaction

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace Transaction {

Record::Record(){
}

Record::Record(const PK & pk,
                quint32 version,
                quint32 lockTime,
                QDateTime seen,
                const Coin::BlockId & blockId,
                quint64 fee)
        : _pk(pk)
        , _version(version)
        , _lockTime(lockTime)
        , _seen(seen)
        , _blockId(blockId)
        , _fee(fee) {
}

QSqlQuery Record::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unBoundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":transactionId", _pk.toByteArray());
    query.bindValue(":version", _version);
    query.bindValue(":lockTime", _lockTime);
    query.bindValue(":seen", _seen.toMSecsSinceEpoch());
    query.bindValue(":blockId", _blockId.toByteArray());
    query.bindValue(":fee", _fee);

    return query;
}

QSqlQuery createTable(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE [Transaction] ( "
        "transactionId       BLOB, "
        "version             INTEGER     NOT NULL, "
        "lockTime            INTEGER     NOT NULL, "
        "seen                INTEGER     NOT NULL, "
        "blockId             BLOB, "
        "fee                 INTEGER     NOT NULL, "
        "PRIMARY KEY(transactionId), "
        "FOREIGN KEY(blockId) REFERENCES BlockHeader " // (bockId)
    ")");

    return query;
}

QSqlQuery unBoundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO [Transaction] "
        "(transactionId, version, lockTime, seen, blockId, fee) "
    "VALUES "
        "(:transactionId, :version, :lockTime, :seen, :blockId, :fee)");

    return query;
}

Record getTransaction(QSqlDatabase db, const Record::PK & pk) {
    throw std::runtime_error("not implemented");
}

QList<Record> allTransactions(QSqlDatabase db) {
    throw std::runtime_error("not implemented");
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
