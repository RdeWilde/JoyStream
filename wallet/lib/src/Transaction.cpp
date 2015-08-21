/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/Transaction.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::Transaction

#include <QSqlQuery>
#include <QSqlError>

namespace Wallet {
namespace Transaction {

Record::Record(){
}

Record::Record(const PK & pk,
                quint32 version,
                quint32 lockTime,
                const QDateTime & seen,
                const QVariant & fee)
        : _pk(pk)
        , _version(version)
        , _lockTime(lockTime)
        , _seen(seen)
        , _fee(fee) {
}

Record::Record(const Coin::Transaction & transaction, const QDateTime & seen)
    : Record(transaction.getHashLittleEndian(), transaction.version, transaction.lockTime, seen, QVariant()) {
}

Record::Record(const QSqlRecord & record) {

    _pk = record.value("transactionId").toByteArray();
    _version = record.value("version").toUInt();
    _lockTime = record.value("lockTime").toUInt();
    _seen = record.value("seen").toDateTime();
    _fee = record.value("fee");
}

Coin::Transaction Record::toTransaction() const {

    Coin::Transaction tx;
    tx.version = _version;
    tx.lockTime = _lockTime;

    return tx;
}

quint64 Record::fee() const {
    return _fee.toULongLong();
}

bool createTable(QSqlDatabase & db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE [Transaction] ( "
        "transactionId       BLOB, "
        "version             INTEGER     NOT NULL, "
        "lockTime            INTEGER     NOT NULL, "
        "seen                DATETIME,"
        "fee                 INTEGER, "
        "PRIMARY KEY(transactionId) "
    ")");

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool insert(QSqlDatabase & db, const Record & record) {

    // Prepare insert query
    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO [Transaction] "
        "(transactionId, version, lockTime, seen, fee) "
    "VALUES "
        "(:transactionId, :version, :lockTime, :seen, :fee)");

    // Bind values to query fields
    query.bindValue(":transactionId", record._pk.toByteArray());
    query.bindValue(":version", record._version);
    query.bindValue(":lockTime", record._lockTime);
    query.bindValue(":seen", record._seen);
    query.bindValue(":fee", record._fee);

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}


QList<Record> allTransactions(QSqlDatabase & db) {
    throw std::runtime_error("not implemented");
}

quint64 getTransactionCount(QSqlDatabase & db) {

    QSqlQuery query(db);

    query.prepare("SELECT COUNT(*) FROM [Transaction]");
    query.exec();

    Q_ASSERT(query.lastError().type() == QSqlError::NoError);
    Q_ASSERT(query.first());

    return query.value(0).toULongLong();
}

bool exists(QSqlDatabase & db, const PK & pk, Record & r) {

    // Prepare select query
    QSqlQuery query(db);

    query.prepare("SELECT * FROM [Transaction] WHERE transactionId = :transactionId");

    // Bind values to query fields
    query.bindValue(":transactionId", pk.toByteArray());

    query.exec();

    Q_ASSERT(query.lastError().type() == QSqlError::NoError);

    if(!query.first())
        return false;

    r = Record(query.record());

    Q_ASSERT(!query.next());

    return true;
}

bool exists(QSqlDatabase & db, const PK & pk) {
    Record r;
    return exists(db, pk, r);
}

}
}
