/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */


#include <wallet/OutBoundPayment.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

OutBoundPayment::OutBoundPayment(quint64 paymentId, const Coin::P2PKHAddress & toAddress, quint64 changeAddressWalletKeyIndex, quint64 amount, quint64 fee, const QString & note, const QDateTime & created)
    : _id(paymentId)
    , _toAddress(toAddress)
    , _changeAddressWalletKeyIndex(changeAddressWalletKeyIndex)
    , _amount(amount)
    , _fee(fee)
    , _note(note)
    , _created(created) {
}

// OutBoundPayment::OutBoundPayment(const QSqlRecord & record);

QSqlQuery OutBoundPayment::createTableQuery(QSqlDatabase db) {

    return QSqlQuery("\
    CREATE TABLE OutBoundPayment (\
        paymentId                       INTEGER,\
        toAddress                       BLOB        NOT NULL,\
        changeAddressWalletKeyIndex     INTEGER     ,\
        amount                          INTEGER     NOT NULL,\
        fee                             INTEGER     NOT NULL,\
        note                            TEXT,\
        created                         INTEGER     NOT NULL,\
        PRIMARY KEY(paymentId),\
        FOREIGN KEY changeAddressWalletKeyIndex REFERENCES WalletAddress(walletKeyIndex)\
    )", db);
}

QSqlQuery OutBoundPayment::unboundedInsertQuery(QSqlDatabase db) {

    return QSqlQuery("\
    INSERT INTO TransactionHasInput \
    (paymentId, toAddress, changeAddressWalletKeyIndex, amount, fee, note, created)\
    VALUES\
    (:paymentId, :toAddress, :changeAddressWalletKeyIndex, :amount, :fee, :note, :created)\
    ", db);
}

QSqlQuery OutBoundPayment::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":paymentId", _id);
    query.bindValue(":toAddress", _toAddress.pubKeyHash().toByteArray());
    query.bindValue(":changeAddressWalletKeyIndex", _changeAddressWalletKeyIndex);
    query.bindValue(":amount", _amount);
    query.bindValue(":fee", _fee);
    query.bindValue(":note", _note);
    query.bindValue(":created", _created);

    return query;
}

quint64 OutBoundPayment::id() const {
    return _id;
}

void OutBoundPayment::setId(quint64 paymentId) {
    _id = paymentId;
}

Coin::P2PKHAddress OutBoundPayment::toAddress() const {
    return _toAddress;
}

void OutBoundPayment::setToAddress(const Coin::P2PKHAddress & toAddress) {
    _toAddress = toAddress;
}

quint64 OutBoundPayment::changeAddressWalletKeyIndex() const {
    return _changeAddressWalletKeyIndex;
}

void OutBoundPayment::setChangeAddressWalletKeyIndex(quint64 changeAddressWalletKeyIndex) {
    _changeAddressWalletKeyIndex = changeAddressWalletKeyIndex;
}

quint64 OutBoundPayment::amount() const {
    return _amount;
}

void OutBoundPayment::setAmount(quint64 amount) {
    _amount = amount;
}

quint64 OutBoundPayment::fee() const {
    return _fee;
}

void OutBoundPayment::setFee(quint64 fee) {
    _fee = fee;
}

QString OutBoundPayment::note() const {
    return _note;
}

void OutBoundPayment::setNote(const QString & note) {
    _note = note;
}

QDateTime OutBoundPayment::created() const {
    return _created;
}

void OutBoundPayment::setCreated(const QDateTime & created) {
    _created = created;
}
