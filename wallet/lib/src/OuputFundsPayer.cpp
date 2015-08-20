/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#include <wallet/OuputFundsPayer.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

namespace Wallet {
namespace OuputFundsPayer {

Record::Record(quint64 value, const QByteArray & scriptPubKey, quint64 payerId)
    : _value(value)
    , _scriptPubKey(scriptPubKey)
    , _payerId(payerId){

}

QSqlQuery Record::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unBoundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":value", _value);
    query.bindValue(":scriptPubKey", _scriptPubKey);
    query.bindValue(":payerId", _payerId);

    return query;
}

QSqlQuery createTableQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE OuputFundsPayer ( "
        "value           INTEGER, "
        "scriptPubKey    BLOB, "
        "payerId         INTEGER, "
        "PRIMARY KEY(value, scriptPubKey, payerId), "
        "FOREIGN KEY payerId REFERENCES Payer(id) "
    ")");

    return query;
}

QSqlQuery unBoundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO Payer "
        "(value, scriptPubKey, payerId) "
    "VALUES "
        "(:value, :scriptPubKey, :payerId) "
    );

    return query;
}
}
}
