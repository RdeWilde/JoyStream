/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/Input.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace Input {

Record::Record(const OutPoint::Record & outPoint, const QByteArray & scriptSig, quint32 sequence)
    : _outPoint(outPoint)
    , _scriptSig(scriptSig)
    , _sequence(sequence) {
}

/**
Input::Input(const QSqlRecord & record) {
}
*/

QSqlQuery Record::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unBoundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":outPointTransactionId", _outPoint.transactionId().toByteArray());
    query.bindValue(":outPointOutputIndex", _outPoint.outputIndex());
    query.bindValue(":scriptSig", _scriptSig);
    query.bindValue(":sequence", _sequence);

    return query;
}

OutPoint::Record Record::outPoint() const {
    return _outPoint;
}

void Record::setOutPoint(const OutPoint::Record & outPoint){
    _outPoint = outPoint;
}

QByteArray Record::scriptSig() const {
    return _scriptSig;
}

void Record::setScriptSig(const QByteArray & scriptSig) {
    _scriptSig = scriptSig;
}

quint32 Record::sequence() const {
    return _sequence;
}

void Record::setSequence(quint32 sequence) {
    _sequence = sequence;
}

QSqlQuery createTableQuery(QSqlDatabase db) {

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

    return query;
}

QSqlQuery unBoundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO Input "
        "(outPointTransactionId, outPointOutputIndex, scriptSig, sequence) "
    "VALUES "
        "(:outPointTransactionId, :outPointOutputIndex, :scriptSig, :sequence) "
    );

    return query;
}

}
}
