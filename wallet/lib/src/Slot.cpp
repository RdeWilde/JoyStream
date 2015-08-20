/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#include <wallet/Slot.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

quint8 Slot::encodeState(State state) {

    switch(state) {
        case State::unassigned: return 0;
        case State::assigned: return 1;
        case State::refund_signed: return 2;

        default:
            Q_ASSERT(false);
    }
}

Slot::State Slot::decodeState(quint8 state) {

    switch(state) {
        case 0: return State::unassigned;
        case 1: return State::assigned;
        case 2: return State::refund_signed;

        default:
            Q_ASSERT(false);
    }
}

Slot::Slot(quint64 payerId,
           quint32 index,
           State state,
           quint64 price,
           quint64 numberOfPaymentsMade,
           quint64 funds,
           quint64 payerContractWalletKeyId,
           quint64 payerFinalWalletKeyId,
           const Coin::PublicKey & payeeContractPublicKey,
           const Coin::PublicKey & payeeFinalPublicKey,
           const Coin::Signature & refundSignature,
           const Coin::Signature & lastPaymentSignature,
           quint64 refundFee,
           quint64 paymentFee,
           quint32 refundLockTime)
    : _payerId(payerId)
    , _index(index)
    , _state(state)
    , _price(price)
    , _numberOfPaymentsMade(numberOfPaymentsMade)
    , _funds(funds)
    , _payerContractWalletKeyId(payerContractWalletKeyId)
    , _payerFinalWalletKeyId(payerFinalWalletKeyId)
    , _payeeContractPublicKey(payeeContractPublicKey)
    , _payeeFinalPublicKey(payeeFinalPublicKey)
    , _refundSignature(refundSignature)
    , _lastPaymentSignature(lastPaymentSignature)
    , _refundFee(refundFee)
    , _paymentFee(paymentFee)
    , _refundLockTime(refundLockTime) {
}

QSqlQuery Slot::createTableQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE Slot ( "
        "payerId                     INTEGER, "
        "index                       INTEGER, "
        "state                       INTEGER     NOT NULL,"
        "price                       INTEGER, "
        "numberOfPaymentsMade        INTEGER     NOT NULL, "
        "funds                       INTEGER     NOT NULL, "
        "payerContractWalletKeyId    INTEGER     NOT NULL, "
        "payerFinalWalletKeyId       INTEGER     NOT NULL, "
        "payeeContractPublicKey      BLOB, "
        "payeeFinalPublicKey         BLOB, "
        "refundSignature             BLOB, "
        "lastPaymentSignature        BLOB, "
        "refundFee                   INTEGER, "
        "paymentFee                  INTEGER, "
        "refundTransactionId         BLOB, "
        "refundLockTime              INTEGER, "
        "PRIMARY KEY(payerId,index), "
        "FOREIGN KEY payerId REFERENCES Payer(id), "
        "FOREIGN KEY payerContractWalletKeyId REFERENCES WalletKey(index), "
        "FOREIGN KEY payerFinalWalletKeyId REFERENCES WalletKey(index), "
        "FOREIGN KEY refundTransactionId REFERENCES Transaction(transactionId) "
    ")");

    return query;
}

QSqlQuery Slot::unboundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO Payer "
        "(payerId, index, state, price, numberOfPaymentsMade, funds, payerContractWalletKeyId, payerFinalWalletKeyId, payeeContractPublicKey, payeeFinalPublicKey, refundSignature, lastPaymentSignature, refundFee, paymentFee, refundTransactionId, refundLockTime) "
    "VALUES "
        "(:payerId, :index, :state, :price, :numberOfPaymentsMade, :funds, :payerContractWalletKeyId, :payerFinalWalletKeyId, :payeeContractPublicKey, :payeeFinalPublicKey, :refundSignature, :lastPaymentSignature, :refundFee, :paymentFee, :refundTransactionId, :refundLockTime) ");

    return query;
}

QSqlQuery Slot::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":payerId", _payerId);
    query.bindValue(":index", _index);
    query.bindValue(":state", encodeState(_state));
    query.bindValue(":price", _price);
    query.bindValue(":numberOfPaymentsMade", _numberOfPaymentsMade);
    query.bindValue(":funds", _funds);
    query.bindValue(":payerContractWalletKeyId", _payerContractWalletKeyId);
    query.bindValue(":payerFinalWalletKeyId", _payerFinalWalletKeyId);
    query.bindValue(":payeeContractPublicKey", _payeeContractPublicKey.toByteArray());
    query.bindValue(":payeeFinalPublicKey", _payeeFinalPublicKey.toByteArray());
    query.bindValue(":refundSignature", _refundSignature.toByteArray());
    query.bindValue(":lastPaymentSignature", _lastPaymentSignature.toByteArray());
    query.bindValue(":refundFee", _refundFee);
    query.bindValue(":paymentFee", _paymentFee);
    query.bindValue(":refundLockTime", _refundLockTime);

    return query;
}

quint64 Slot::payerId() const {
    return _payerId;
}

void Slot::setPayerId(quint64 payerId) {
    _payerId = payerId;
}

quint32 Slot::index() const {
    return _index;
}

void Slot::setIndex(quint32 index) {
    _index = index;
}

Slot::State Slot::state() const {
    return _state;
}

void Slot::setState(State state){
    _state = state;
}

quint64 Slot::price() const {
    return _price;
}

void Slot::setPrice(quint64 price) {
    _price = price;
}

quint64 Slot::numberOfPaymentsMade() const {
    return _numberOfPaymentsMade;
}

void Slot::setNumberOfPaymentsMade(quint64 numberOfPaymentsMade) {
    _numberOfPaymentsMade = numberOfPaymentsMade;
}

quint64 Slot::funds() const {
    return _funds;
}

void Slot::setFunds(quint64 funds) {
    _funds = funds;
}

quint64 Slot::payerContractWalletKeyId() const {
    return _payerContractWalletKeyId;
}

void Slot::setPayerContractWalletKeyId(quint64 payerContractWalletKeyId) {
    _payerContractWalletKeyId = payerContractWalletKeyId;
}

quint64 Slot::payerFinalWalletKeyId() const {
    return _payerFinalWalletKeyId;
}

void Slot::setPayerFinalWalletKeyId(quint64 payerFinalWalletKeyId) {
    _payerFinalWalletKeyId = payerFinalWalletKeyId;
}

Coin::PublicKey Slot::payeeContractPublicKey() const {
    return _payeeContractPublicKey;
}

void Slot::setPayeeContractPublicKey(const Coin::PublicKey & payeeContractPublicKey) {
    _payeeContractPublicKey = payeeContractPublicKey;
}

Coin::PublicKey Slot::payeeFinalPublicKey() const {
    return _payeeFinalPublicKey;
}

void Slot::setPayeeFinalPublicKey(const Coin::PublicKey & payeeFinalPublicKey) {
    _payeeFinalPublicKey = payeeFinalPublicKey;
}

Coin::Signature Slot::refundSignature() const {
    return _refundSignature;
}

void Slot::setRefundSignature(const Coin::Signature & refundSignature) {
    _refundSignature = refundSignature;
}

Coin::Signature Slot::lastPaymentSignature() const {
    return _lastPaymentSignature;
}

void Slot::setLastPaymentSignature(const Coin::Signature & lastPaymentSignature) {
    _lastPaymentSignature = lastPaymentSignature;
}

quint64 Slot::refundFee() const {
    return _refundFee;
}

void Slot::setRefundFee(quint64 refundFee) {
    _refundFee = refundFee;
}

quint64 Slot::paymentFee() const {
    return _paymentFee;
}

void Slot::setPaymentFee(quint64 paymentFee) {
    _paymentFee = paymentFee;
}

quint32 Slot::refundLockTime() const {
    return _refundLockTime;
}

void Slot::setRefundLockTime(quint32 refundLockTime) {
    _refundLockTime = refundLockTime;
}
