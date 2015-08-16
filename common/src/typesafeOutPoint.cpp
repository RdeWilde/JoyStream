/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/typesafeOutPoint.hpp>
//#include <common/Utilities.hpp>

//#include <QJsonObject>
//#include <QJsonValue>
#include <QStringList>
#include <QHash>

namespace Coin {

typesafeOutPoint::typesafeOutPoint()
    : _index(0){
}

typesafeOutPoint::typesafeOutPoint(const TransactionId & txId, quint32 index)
    : _txId(txId)
    , _index(index) {
}

typesafeOutPoint::typesafeOutPoint(const typesafeOutPoint& o)
    : typesafeOutPoint(o.transactionId(), o.index()) {
}

typesafeOutPoint & typesafeOutPoint::operator=(const typesafeOutPoint& o) {

    _txId = o.transactionId();
    _index = o.index();

    return *this;
}

/**
OutPoint::OutPoint(const QJsonObject & json) {

    // _hash
    _txId = TxId(Utilities::GET_STRING(json, "hash"));

    // _index
    _index = Utilities::GET_DOUBLE(json, "index");
}

QJsonObject OutPoint::json() const {
    return QJsonObject {
        {"hash", _txId.toString()},
        {"index", static_cast<int>(_index)}
    };
}
*/

/*
bool OutPoint::operator==(const OutPoint & o) {
    return _txId == o.txId() && _index == o.index();
}
*/

bool operator==(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs) {
    return (lhs.transactionId() < rhs.transactionId()) || ((lhs.transactionId() == rhs.transactionId()) && (lhs.index() < rhs.index()));
}

bool operator!=(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs) {
    return !(lhs == rhs);
}

/*
bool OutPoint::operator<(const OutPoint & o) {
    return (_txId < o.txId()) || ((_txId == o.txId()) && (_index < o.index()));
}
*/

bool operator<(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs) {
    return (lhs.transactionId() < rhs.transactionId()) || ((lhs.transactionId() == rhs.transactionId()) && (lhs.index() < rhs.index()));
}

/**
OutPoint::OutPoint(const QString & string) {

    QStringList list = string.split("-");

    if(list.size() != 2)
        throw new std::runtime_error("Only a single delimiter - should occur in string.");

    // Parse _hash
    _txId = TxId(list[0]);

    // Parse _index
    bool ok;
    QString rightToken = list[1];

    _index = rightToken.toInt(&ok);

    if(!ok)
        throw new std::runtime_error("Could not convert second token.");
}
*/

QString typesafeOutPoint::toString() const {
    return _txId.toHex() + "-" + QString::number(_index);
}


TransactionId typesafeOutPoint::transactionId() const {
    return _txId;
}

void typesafeOutPoint::setTransactionId(const TransactionId & txId){
    _txId = txId;
}

quint32 typesafeOutPoint::index() const {
    return _index;
}

void typesafeOutPoint::setIndex(quint32 index) {
    _index = index;
}

}

uint qHash(const Coin::typesafeOutPoint & o) {
    return qHash(o.toString());
}
