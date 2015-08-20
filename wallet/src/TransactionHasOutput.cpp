/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/TransactionHasOutput.hpp>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace TransactionHasOutput {

PK::PK() {
}

PK::PK(const Coin::TransactionId & transactionId, quint32 index)
    : _transactionId(transactionId)
    , _index(index) {
}

Record::Record() {
}

Record::Record(const PK & pk, const Output::PK & output)
    : _pk(pk)
    , _output(output) {
}

Record::Record(const QSqlRecord & record) {

    Coin::TransactionId transactionId = record.value("transactionId").toByteArray();
    quint32 index = record.value("[index]").toUInt();
    //quint32 index = record.value("index").toUInt();
    _pk = PK(transactionId, index);

    quint64 value = record.value("value").toULongLong();
    QByteArray scriptPubKey = record.value("scriptPubKey").toByteArray();

    _output = Output::PK(value, scriptPubKey);
}

bool createTable(QSqlDatabase & db) {

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

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool insert(QSqlDatabase & db, const Record & record) {

    // Prepare insert query
    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO TransactionHasOutput "
        "(transactionId, [index], value, scriptPubKey)"
    "VALUES "
        "(:transactionId, :index, :value, :scriptPubKey)");

    // Bind values to query fields
    query.bindValue(":transactionId", record._pk._transactionId.toByteArray());
    query.bindValue(":index", record._pk._index);
    query.bindValue(":value",  record._output._value);
    query.bindValue(":scriptPubKey", record._output._scriptPubKey);

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool exists(QSqlDatabase & db, const PK & pk, Record & r) {

    // Prepare select query
    QSqlQuery query(db);

    query.prepare("SELECT * FROM TransactionHasOutput WHERE "
                  "transactionId = :transactionId AND "
                  "[index] = :index");

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

bool exists(QSqlDatabase & db, const PK & pk) {
    Record r;
    return exists(db, pk, r);
}

}
}
