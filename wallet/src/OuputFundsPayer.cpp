/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#include <wallet/OuputFundsPayer.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

OuputFundsPayer::OuputFundsPayer(quint64 value, const QByteArray & pubKeyScript, quint64 payerId)
    : _value(value)
    , _pubKeyScript(pubKeyScript)
    , _payerId(payerId){

}

QSqlQuery OuputFundsPayer::createTableQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "CREATE TABLE OuputFundsPayer ( "
        "value           INTEGER, "
        "pubKeyScript    BLOB, "
        "payerId         INTEGER, "
        "PRIMARY KEY(value, pubKeyScript, payerId), "
        "FOREIGN KEY payerId REFERENCES Payer(id) "
    ")");

    return query;
}

QSqlQuery OuputFundsPayer::unboundedInsertQuery(QSqlDatabase db) {

    QSqlQuery query(db);

    query.prepare(
    "INSERT INTO Payer "
        "(value, pubKeyScript, payerId) "
    "VALUES "
        "(:value, :pubKeyScript, :payerId) "
    );

    return query;
}

QSqlQuery OuputFundsPayer::insertQuery(QSqlDatabase db) {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery(db);

    // Bind values to query fields
    query.bindValue(":value", _value);
    query.bindValue(":pubKeyScript", _pubKeyScript);
    query.bindValue(":payerId", _payerId);

    return query;
}

quint64 OuputFundsPayer::value() const {
    return _value;
}

void OuputFundsPayer::setValue(quint64 value) {
    _value = value;
}

QByteArray OuputFundsPayer::pubKeyScript() const {
    return _pubKeyScript;
}

void OuputFundsPayer::setPubKeyScript(const QByteArray & pubKeyScript) {
    _pubKeyScript = pubKeyScript;
}

quint64 OuputFundsPayer::payerId() const {
    return _payerId;
}

void OuputFundsPayer::setPayerId(quint64 payerId) {
    _payerId = payerId;
}
