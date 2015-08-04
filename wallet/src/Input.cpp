/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/Input.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

Input::Input(const OutPoint & outPoint, const QByteArray & scriptSig, quint32 sequence)
    : _outPoint(outPoint)
    , _scriptSig(scriptSig)
    , _sequence(sequence) {
}

/**
Input::Input(const QSqlRecord & record) {
}
*/

QSqlQuery Input::createTableQuery(QSqlDatabase db) {

    return QSqlQuery("\
    CREATE TABLE Input (\
        outPointTransactionId       BLOB,\
        outPointOutputIndex         INTEGER     NOT NULL,\
        scriptSig                   BLOG        NOT NULL,\
        sequence                    INTEGER     NOT NULL,\
        PRIMARY KEY(outPointTransactionId, outPointOutputIndex, scriptSig, sequence),\
        FOREIGN KEY (outPointTransactionId, outPointOutputIndex) REFERENCES OutPoint(transactionId, outputIndex)\
    )", db);
}

QSqlQuery Input::unboundedInsertQuery(QSqlDatabase db) {

    return QSqlQuery("\
    INSERT INTO Input \
    (outPointTransactionId, outPointOutputIndex, scriptSig, sequence)\
    VALUES\
    (:outPointTransactionId, :outPointOutputIndex, :scriptSig, :sequence)\
    ", db);
}

QSqlQuery Input::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":outPointTransactionId", _outPoint.transactionId().toByteArray());
    query.bindValue(":outPointOutputIndex", _outPoint.outputIndex());
    query.bindValue(":scriptSig", _scriptSig);
    query.bindValue(":sequence", _sequence);

    return query;
}

OutPoint Input::outPoint() const {
    return _outPoint;
}

void Input::setOutPoint(const OutPoint & outPoint){
    _outPoint = outPoint;
}

QByteArray Input::scriptSig() const {
    return _scriptSig;
}

void Input::setScriptSig(const QByteArray & scriptSig) {
    _scriptSig = scriptSig;
}

quint32 Input::sequence() const {
    return _sequence;
}

void Input::setSequence(quint32 sequence) {
    _sequence = sequence;
}
