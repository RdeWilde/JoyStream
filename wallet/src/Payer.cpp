/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#include <wallet/Payer.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

quint8 encodeState(Payer::State state) {

    switch(state) {
        case Payer::State::waiting_for_full_set_of_sellers: return 0;
        case Payer::State::waiting_for_full_set_of_refund_signatures: return 1;
        case Payer::State::can_pay: return 2;
        case Payer::State::all_contract_outputs_spent: return 3;
        default:
            Q_ASSERT(false); // we should never come here
    }
}

Payer::State decodeState(quint8 state) {

    switch(state) {
        case 0: return Payer::State::waiting_for_full_set_of_sellers;
        case 1: return Payer::State::waiting_for_full_set_of_refund_signatures;
        case 2: return Payer::State::can_pay;
        case 3: return Payer::State::all_contract_outputs_spent;
        default:
            Q_ASSERT(false); // we should never come here
    }
}

Payer::Payer(quint64 id, const Coin::TransactionId & contractTransactionId, State state, const QDateTime & created, const QString & description)
    : _id(id)
    , _contractTransactionId(contractTransactionId)
    , _state(state)
    , _created(created)
    , _description(description) {
}

QSqlQuery Payer::createTableQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE Payer ( "
        "id                      INTEGER, "
        "contractTransactionId   BLOB, "
        "contractFee             INTEGER     NOT NULL, "
        "stateId                 INTEGER     NOT NULL, "
        "created                 INTEGER     NOT NULL, "
        "description             TEXT, "
        "PRIMARY KEY(id), "
        "FOREIGN KEY contractTransactionId REFERENCES Transaction(transactionId) "
    ")");

    return query;
}

QSqlQuery Payer::unboundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO Payer "
        "(id, contractTransactionId, contractFee, stateId, created, description) "
    "VALUES "
        "(:id, :contractTransactionId, :contractFee, :stateId, :created, :description) ");
}

QSqlQuery Payer::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":id", _id);
    query.bindValue(":contractTransactionId", _contractTransactionId.toByteArray());
    query.bindValue(":contractFee", _contractFee);
    query.bindValue(":stateId", encodeState(_state));
    query.bindValue(":created", _created.toMSecsSinceEpoch());
    query.bindValue(":description", _description);

    return query;
}

quint64 Payer::id() const {
    return _id;
}

void Payer::setId(quint64 id) {
    _id = id;
}

Coin::TransactionId Payer::contractTransactionId() const {
    return _contractTransactionId;
}

void Payer::setContractTransactionId(const Coin::TransactionId &contractTransactionId) {
    _contractTransactionId = contractTransactionId;
}

quint64 Payer::contractFee() const {
    return _contractFee;
}

void Payer::setContractFee(quint64 contractFee) {
    _contractFee = contractFee;
}

Payer::State Payer::state() const {
    return _state;
}

void Payer::setState(State state) {
    _state = state;
}

QDateTime Payer::created() const {
    return _created;
}

void Payer::setCreated(const QDateTime & created) {
    _created = created;
}

QString Payer::description() const {
    return _description;
}

void Payer::setDescription(const QString & description) {
    _description = description;
}
