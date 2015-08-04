/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */


#include <wallet/InBoundPayment.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

InBoundPayment::InBoundPayment(quint64 id, quint64 receiveAddressWalletKeyIndex, const QString & note, const QDateTime & created)
    : _id(id)
    , _receiveAddressWalletKeyIndex(receiveAddressWalletKeyIndex)
    , _note(note)
    , _created(created) {
}

//InBoundPayment::InBoundPayment(const QSqlRecord & record);

QSqlQuery InBoundPayment::createTableQuery() {

    return QSqlQuery("\
    CREATE TABLE InBoundPayment (\
        id                              INTEGER,\
        receiveAddressWalletKeyIndex    INTEGER     NOT NULL,\
        note                            TEXT        NOT NULL,\
        created                         INTEGER     NOT NULL,\
        PRIMARY KEY(id),\
        FOREIGN KEY receiveAddressWalletKeyIndex REFERENCES WalletAddress(walletKeyIndex)\
    )");
}

QSqlQuery InBoundPayment::unboundedInsertQuery() {

    return QSqlQuery("\
    INSERT INTO TransactionHasInput \
    (id, receiveAddressWalletKeyIndex, note, created)\
    VALUES\
    (:id, :receiveAddressWalletKeyIndex, :note, :created)\
    ");
}

QSqlQuery InBoundPayment::insertQuery() {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery();

    // Bind values to query fields
    query.bindValue(":id", _id);
    query.bindValue(":receiveAddressWalletKeyIndex", _receiveAddressWalletKeyIndex);
    query.bindValue(":note", _note);
    query.bindValue(":created", _created.toMSecsSinceEpoch());

    return query;
}

quint64 InBoundPayment::paymentId() const {
    return _id;
}

void InBoundPayment::setPaymentId(quint64 id) {
    _id = id;
}

quint64 InBoundPayment::receiveAddressWalletKeyIndex() const {
    return _receiveAddressWalletKeyIndex;
}

void InBoundPayment::setReceiveAddressWalletKeyIndex(quint64 receiveAddressWalletKeyIndex) {
    _receiveAddressWalletKeyIndex = receiveAddressWalletKeyIndex;
}

QString InBoundPayment::note() const {
    return _note;
}

void InBoundPayment::setNote(const QString & note) {
    _note = note;
}

QDateTime InBoundPayment::created() const {
    return _created;
}

void InBoundPayment::setCreated(const QDateTime & created) {
    _created = created;
}
