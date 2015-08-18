/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/Input.hpp>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace Input {

PK::PK() {
}

PK::PK(const OutPoint::PK & outPointPK, const QByteArray & scriptSig, quint32 sequence)
    : _outPointPK(outPointPK)
    , _scriptSig(scriptSig)
    , _sequence(sequence) {
}

Record::Record() {
}

Record::Record(const PK & pk)
    : _pk(pk) {
}

bool createTable(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE Input ( "
        "outPointTransactionId       BLOB, "
        "outPointOutputIndex         INTEGER     NOT NULL, "
        "scriptSig                   BLOG        NOT NULL, "
        "sequence                    INTEGER     NOT NULL, "
        "PRIMARY KEY(outPointTransactionId, outPointOutputIndex, scriptSig, sequence), "
        "FOREIGN KEY(outPointTransactionId, outPointOutputIndex) REFERENCES OutPoint(transactionId, outputIndex) "
    ")");

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}


bool insert(QSqlDatabase db, const Record & record) {

    // Create insert query
    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO Input "
        "(outPointTransactionId, outPointOutputIndex, scriptSig, sequence) "
    "VALUES "
        "(:outPointTransactionId, :outPointOutputIndex, :scriptSig, :sequence) "
    );

    // Bind values to query fields
    query.bindValue(":outPointTransactionId", record._pk._outPointPK._transactionId.toByteArray());
    query.bindValue(":outPointOutputIndex", record._pk._outPointPK._outputIndex);
    query.bindValue(":scriptSig", record._pk._scriptSig);
    query.bindValue(":sequence", record._pk._sequence);

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}


bool exists(QSqlDatabase & db, const PK & pk, Record & r) {
    throw std::runtime_error("not implemented");
}

bool exists(QSqlDatabase & db, const PK & pk) {
    Record r;
    return exists(db, pk, r);
}

}
}
