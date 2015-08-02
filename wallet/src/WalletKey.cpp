/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#include <wallet/WalletKey.hpp>

#include <QSqlQuery>
#include <QVariant>

WalletKey::WalletKey(quint64 index, const Coin::PrivateKey & privateKey, const QDateTime & generated)
    : _index(index)
    , _privateKey(privateKey)
    , _generated(generated) {
}

QSqlQuery WalletKey::createTableQuery() {

    return QSqlQuery("\
    CREATE TABLE WalletKey (\
        index           INTEGER     PRIMARY KEY,\
        privateKey      BLOB        NOT NULL,\
        generated       INTEGER     NOT NULL\
    )");
}

static QSqlQuery WalletKey::unboundedInsertQuery() {

    return QSqlQuery("\
    INSERT INTO PrivateKey \
    (index, privateKey, generated)\
    VALUES\
    (:index, :privateKey, :generated)\
    ");
}

QSqlQuery WalletKey::insertQuery() {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery();

    // bind wallet key values
    query.bindValue(":index", static_cast<uint>(_index));
    query.bindValue(":privateKey", Coin::uchar_vector_to_QByteArray(_privateKey.raw()));
    //query.bindValue(":keyPurposeId", WalletKey::encodePurpose(walletKey.purpose()));
    query.bindValue(":generated", _generated.toMSecsSinceEpoch());

    return query;
}

/*
quint8 WalletKey::encodePurpose(Purpose purpose) {

    switch(purpose) {
        case Purpose::Receive: return 0;
        case Purpose::ContractChange: return 1;
        case Purpose::PayerInContractOutput: return 2;
        case Purpose::ContractFinal: return 3;
        case Purpose::PayeeInContractOutput: return 4;
        case Purpose::ContractPayment: return 5;

        default:
            Q_ASSERT(false);
    }
}

WalletKey::Purpose WalletKey::decodePurpose(quint8 encoded) {

    switch(encoded) {
        case 0: return Purpose::Receive;
        case 1: return Purpose::ContractChange;
        case 2: return Purpose::PayerInContractOutput;
        case 3: return Purpose::ContractFinal;
        case 4: return Purpose::PayeeInContractOutput;
        case 5: return Purpose::ContractPayment;

        default:
            Q_ASSERT(false);
    }
}
*/

quint64 WalletKey::index() const {
    return _index;
}

void WalletKey::setIndex(quint64 index) {
    _index = index;
}

Coin::PrivateKey WalletKey::privateKey() const {
    return _privateKey;
}

void WalletKey::setPrivateKey(const Coin::PrivateKey & privateKey) {
    _privateKey = privateKey;
}

QDateTime WalletKey::generated() const {
    return _generated;
}

void WalletKey::setGenerated(const QDateTime & generated) {
    _generated = generated;
}



/*
WalletKey::Purpose WalletKey::purpose() const {
    return _purpose;
}

void WalletKey::setPurpose(Purpose purpose) {
    _purpose = purpose;
}
*/

