/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */


#include <wallet/InBoundPayment.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace InBoundPayment {

Record::Record(quint64 id, quint64 receiveAddressWalletKeyIndex, const QString & note, const QDateTime & created)
    : _id(id)
    , _receiveAddressWalletKeyIndex(receiveAddressWalletKeyIndex)
    , _note(note)
    , _created(created) {
}

//InBoundPayment::InBoundPayment(const QSqlRecord & record);

QSqlQuery Record::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unBoundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":id", _id);
    query.bindValue(":receiveAddressWalletKeyIndex", _receiveAddressWalletKeyIndex);
    query.bindValue(":note", _note);
    query.bindValue(":created", _created.toMSecsSinceEpoch());

    return query;
}

QSqlQuery createTableQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE InBoundPayment ( "
        "id                              INTEGER, "
        "receiveAddressWalletKeyIndex    INTEGER     NOT NULL, "
        "note                            TEXT        NOT NULL, "
        "created                         INTEGER     NOT NULL, "
        "PRIMARY KEY(id), "
        "FOREIGN KEY receiveAddressWalletKeyIndex REFERENCES WalletAddress(walletKeyIndex) "
    ")");

    return query;
}

QSqlQuery unBoundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO TransactionHasInput "
        "(id, receiveAddressWalletKeyIndex, note, created) "
    "VALUES "
        "(:id, :receiveAddressWalletKeyIndex, :note, :created) "
    );

    return query;
}

}
}
