/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/TransactionHasInput.hpp>
#include <CoinCore/CoinNodeData.h>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace TransactionHasInput {

PK::PK() {
}

PK::PK(const Coin::TransactionId & transactionId, quint32 index)
    : _transactionId(transactionId)
    , _index(index) {
}

Record::Record() {
}

Record::Record(const PK & pk, const Input::PK & input)
    : _pk(pk)
    , _input(input) {
}

Record::Record(const QSqlRecord & record) {

    Coin::TransactionId transactionId = record.value("transactionId").toByteArray();
    quint32 index = record.value("[index]").toUInt();
    //quint32 index = record.value("index").toUInt();
    _pk = PK(transactionId, index);

    QByteArray outPointTransactionId = record.value("outPointTransactionId").toByteArray();
    quint32 outPointOutputIndex = record.value("outPointOutputIndex").toUInt();
    QByteArray scriptSig = record.value("scriptSig").toByteArray();
    quint32 sequence = record.value("sequence").toUInt();

    _input = Input::PK(OutPoint::PK(outPointTransactionId, outPointOutputIndex), scriptSig, sequence);
}

bool createTable(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE TransactionHasInput ( "
        "transactionId               BLOB        NOT NULL, "
        "[index]                     INTEGER     NOT NULL, "
        "outPointTransactionId       BLOB        NOT NULL, "
        "outPointOutputIndex         INTEGER     NOT NULL, "
        "scriptSig                   BLOG        NOT NULL, "
        "sequence                    INTEGER     NOT NULL, "
        "PRIMARY KEY(transactionId, [index]), "
        "FOREIGN KEY(transactionId) REFERENCES [Transaction](transactionId), "
        "FOREIGN KEY(outPointTransactionId, outPointOutputIndex, scriptSig, sequence) REFERENCES Input(outPointTransactionId, outPointOutputIndex, scriptSig, sequence) "
    ")");

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool insert(QSqlDatabase db, const Record & record) {

    // Prepare insert query
    QSqlQuery query(db);

    query.prepare (
    "INSERT INTO TransactionHasInput "
        "(transactionId, [index], outPointTransactionId, outPointOutputIndex, scriptSig, sequence) "
    "VALUES "
        "(:transactionId, :index, :outPointTransactionId, :outPointOutputIndex, :scriptSig, :sequence) ");

    // Bind values to query fields
    query.bindValue(":transactionId", record._pk._transactionId.toByteArray());
    query.bindValue(":index", record._pk._index);
    query.bindValue(":outPointTransactionId", record._input._outPoint._transactionId.toByteArray());
    query.bindValue(":outPointOutputIndex", record._input._outPoint._index);
    query.bindValue(":scriptSig",  record._input._scriptSig);
    query.bindValue(":sequence", record._input._sequence);

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}

bool exists(QSqlDatabase db, const PK & pk, Record & r) {

    // Prepare select query
    QSqlQuery query(db);

    query.prepare("SELECT * FROM TransactionHasInput WHERE "
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

bool exists(QSqlDatabase db, const PK & pk) {
    Record r;
    return exists(db, pk, r);
}

std::vector<Coin::TxIn> inputsOfTransaction(QSqlDatabase db, const Coin::TransactionId & transactionId) {

    std::vector<Coin::TxIn> inputs;

    // Prepare select query
    QSqlQuery query(db);

    // Order by increasing index so that we can add to inputs vector in order
    query.prepare("SELECT * FROM TransactionHasInput WHERE "
                  "transactionId = :transactionId ORDER BY [index] ASC");

    // Bind values to query fields
    query.bindValue(":transactionId", transactionId.toByteArray());

    query.exec();

    Q_ASSERT(query.lastError().type() == QSqlError::NoError);

    // Iterate record
    int index = 0; // assert variable
    while(query.next()) {

        // Get raw record
        QSqlRecord rawRecord = query.record();

        bool ok;
        Q_ASSERT(rawRecord.value("index").toUInt(&ok) == index);
        Q_ASSERT(ok);

        // Create input record
        Input::Record inputRecord(rawRecord);

        // Convert to input
        Coin::TxIn in = inputRecord.toInput();

        // Add to vector
        inputs.push_back(in);

        index++;
    }

    return inputs;
}

}
}
