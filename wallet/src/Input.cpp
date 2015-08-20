/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/Input.hpp>

#include <CoinCore/CoinNodeData.h> // TxIn
#include <common/Utilities.hpp> // toByteArray()

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace Input {

PK::PK() {
}

PK::PK(const OutPoint::PK & outPointPK, const QByteArray & scriptSig, quint32 sequence)
    : _outPoint(outPointPK)
    , _scriptSig(scriptSig)
    , _sequence(sequence) {
}

PK::PK(const Coin::TxIn & in)
    : PK(in.previousOut, Coin::toByteArray(in.scriptSig), in.sequence){
}

Record::Record() {
}

Record::Record(const PK & pk)
    : _pk(pk) {
}

Record::Record(const QSqlRecord & record) {

    // outpoint
    Coin::TransactionId outPointTransactionId = record.value("outPointTransactionId").toByteArray();
    quint32 outPointOutputIndex = record.value("outPointOutputIndex").toUInt();

    OutPoint::PK outPointPk(outPointTransactionId, outPointOutputIndex);

    // scriptSig
    QByteArray scriptSig = record.value("scriptSig").toByteArray();

    // sequence
    quint32 sequence = record.value("sequence").toUInt();

    _pk = Input::PK(outPointPk, scriptSig, sequence);
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
    query.bindValue(":outPointTransactionId", record._pk._outPoint._transactionId.toByteArray());
    query.bindValue(":outPointOutputIndex", record._pk._outPoint._index);
    query.bindValue(":scriptSig", record._pk._scriptSig);
    query.bindValue(":sequence", record._pk._sequence);

    query.exec();

    return (query.lastError().type() == QSqlError::NoError);
}


bool exists(QSqlDatabase & db, const PK & pk, Record & r) {

    // Prepare select query
    QSqlQuery query(db);

    query.prepare("SELECT * FROM Input WHERE "
                  "outPointTransactionId = :outPointTransactionId AND "
                  "outPointOutputIndex = :outPointOutputIndex AND "
                  "scriptSig = :scriptSig AND "
                  "sequence = :sequence");

    // Bind values to query fields
    query.bindValue(":outPointTransactionId", pk._outPoint._transactionId.toByteArray());
    query.bindValue(":outPointOutputIndex", pk._outPoint._index);
    query.bindValue(":scriptSig", pk._scriptSig);
    query.bindValue(":sequence", pk._sequence);

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
