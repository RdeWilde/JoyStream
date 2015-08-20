/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2015
 */

#include <wallet/TransactionMinedInBlock.hpp>

#include <QSqlQuery>
#include <QSqlError>

namespace Wallet {
namespace TransactionMinedInBlock {

PK::PK() {
}

PK::PK(const Transaction::PK & transaction, const BlockHeader::PK & block)
    : _transactionId(transaction)
    , _blockId(block) {
}


Record::Record() {
}

Record::Record(const PK & pk, const QByteArray & merkleBranch, quint32 index)
    : _pk(pk)
    , _merkleBranch(merkleBranch)
    , _index(index) {
}

Record::Record(const QSqlRecord & record) {

    Coin::TransactionId transactionId = record.value("transactionId").toByteArray();
    Coin::BlockId blockId = record.value("blockId").toByteArray();
    _pk = PK(transactionId, blockId);

    _merkleBranch = record.value("merkleBranch").toByteArray();
    _index = record.value("[index]").toUInt();
}

bool createTable(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE TransactionMinedInBlock ( "
        "transactionId       BLOB, "
        "blockId             BLOB, "
        "merkleBranch        BLOB       NOT NULL, "
        "[index]             INTEGER    NOT NULL, "
        "PRIMARY KEY(transactionId, blockId), "
        "FOREIGN KEY(transactionId) REFERENCES [Transaction], "
        "FOREIGN KEY(blockId) REFERENCES BlockHeader(blockId) "
    ")");

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool insert(QSqlDatabase db, const Record & record) {

    // Prepare insert query
    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO [Transaction] "
        "(transactionId, blockId, merkleBranch, index) "
    "VALUES "
        "(:transactionId, :blockId, :merkleBranch, :index)");

    // Bind values to query fields
    query.bindValue(":transactionId", record._pk._transactionId.toByteArray());
    query.bindValue(":blockId", record._pk._blockId.toByteArray());
    query.bindValue(":merkleBranch", record._merkleBranch);
    query.bindValue(":index", record._index);

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool exists(QSqlDatabase & db, const PK & pk, Record & r) {

    // Prepare select query
    QSqlQuery query(db);

    query.prepare("SELECT * FROM TransactionMinedInBlock WHERE "
                  "transactionId = :transactionId AND "
                  "blockId = :blockId");

    // Bind values to query fields
    query.bindValue(":transactionId", pk._transactionId.toByteArray());
    query.bindValue(":blockId", pk._blockId.toByteArray());

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
