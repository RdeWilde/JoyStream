/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/OutBoundPayment.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace OutBoundPayment {

Record::Record(quint64 paymentId, const Coin::P2PKHAddress & toAddress, quint64 changeAddressWalletKeyIndex, quint64 amount, quint64 fee, const QString & note, const QDateTime & created)
    : _id(paymentId)
    , _toAddress(toAddress)
    , _changeAddressWalletKeyIndex(changeAddressWalletKeyIndex)
    , _amount(amount)
    , _fee(fee)
    , _note(note)
    , _created(created) {
}

// Record::Record(const QSqlRecord & record);
QSqlQuery Record::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unBoundedInsertQuery(db);

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

quint64 Record::id() const {
    return _id;
}

void Record::setId(quint64 paymentId) {
    _id = paymentId;
}

Coin::P2PKHAddress Record::toAddress() const {
    return _toAddress;
}

void Record::setToAddress(const Coin::P2PKHAddress & toAddress) {
    _toAddress = toAddress;
}

quint64 Record::changeAddressWalletKeyIndex() const {
    return _changeAddressWalletKeyIndex;
}

void Record::setChangeAddressWalletKeyIndex(quint64 changeAddressWalletKeyIndex) {
    _changeAddressWalletKeyIndex = changeAddressWalletKeyIndex;
}

quint64 Record::amount() const {
    return _amount;
}

void Record::setAmount(quint64 amount) {
    _amount = amount;
}

quint64 Record::fee() const {
    return _fee;
}

void Record::setFee(quint64 fee) {
    _fee = fee;
}

QString Record::note() const {
    return _note;
}

void Record::setNote(const QString & note) {
    _note = note;
}

QDateTime Record::created() const {
    return _created;
}

void Record::setCreated(const QDateTime & created) {
    _created = created;
}

QSqlQuery createTableQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE OutBoundPayment ( "
        "paymentId                       INTEGER, "
        "toAddress                       BLOB        NOT NULL, "
        "changeAddressWalletKeyIndex     INTEGER, "
        "amount                          INTEGER     NOT NULL, "
        "fee                             INTEGER     NOT NULL, "
        "note                            TEXT, "
        "created                         INTEGER     NOT NULL, "
        "PRIMARY KEY(paymentId), "
        "FOREIGN KEY changeAddressWalletKeyIndex REFERENCES WalletAddress(walletKeyIndex) "
    ")");

    return query;
}

QSqlQuery unBoundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO TransactionHasInput "
        "(paymentId, toAddress, changeAddressWalletKeyIndex, amount, fee, note, created) "
    "VALUES "
        "(:paymentId, :toAddress, :changeAddressWalletKeyIndex, :amount, :fee, :note, :created) "
    );
}


}
}
