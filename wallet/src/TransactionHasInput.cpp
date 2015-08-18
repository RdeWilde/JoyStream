/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/TransactionHasInput.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace TransactionHasInput {

Record::PK::PK() {
}

Record::PK::PK(const Coin::TransactionId & transactionId, quint32 index)
    : _transactionId(transactionId)
    , _index(index) {
}

Record::Record() {
}

Record::Record(const PK & pk, const Input::Record & input)
    : _pk(pk)
    , _input(input) {
}

QSqlQuery Record::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unBoundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":transactionId", _pk._transactionId.toByteArray());
    query.bindValue(":index", _pk._index);
    query.bindValue(":outPointTransactionId", _input._pk._outPointPK._transactionId.toByteArray());
    query.bindValue(":outPointOutputIndex", _input._pk._outPointPK._outputIndex);
    query.bindValue(":scriptSig",  _input._pk._scriptSig);
    query.bindValue(":sequence", _input._pk._sequence);

    return query;
}

QSqlQuery createTable(QSqlDatabase db) {

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

    return query;
}

QSqlQuery unBoundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare (
    "INSERT INTO TransactionHasInput "
        "(transactionId, index, outPointTransactionId, outPointOutputIndex, scriptSig, sequence) "
    "VALUES "
        "(:transactionId, :index, :outPointTransactionId, :outPointOutputIndex, :scriptSig, :sequence) ");

    return query;
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
