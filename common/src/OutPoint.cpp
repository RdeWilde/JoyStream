/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/OutPoint.hpp>
//#include <common/Utilities.hpp>

//#include <QJsonObject>
//#include <QJsonValue>
#include <QStringList>
#include <QHash>

namespace Coin {

OutPoint::OutPoint()
    : _index(0){
}

OutPoint::OutPoint(const TransactionId & txId, quint32 index)
    : _txId(txId)
    , _index(index) {
}

OutPoint::OutPoint(const OutPoint& o)
    : OutPoint(o.transactionId(), o.index()) {
}

/**
OutPoint & OutPoint::operator=(const OutPoint& o) {
    _txId = o.txId();
    _index = o.index();

    return *this;
}

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

bool operator==(const OutPoint & lhs, const OutPoint & rhs) {
    return (lhs.transactionId() < rhs.transactionId()) || ((lhs.transactionId() == rhs.transactionId()) && (lhs.index() < rhs.index()));
}

bool operator!=(const OutPoint & lhs, const OutPoint & rhs) {
    return !(lhs == rhs);
}

/*
bool OutPoint::operator<(const OutPoint & o) {
    return (_txId < o.txId()) || ((_txId == o.txId()) && (_index < o.index()));
}
*/

bool operator<(const OutPoint & lhs, const OutPoint & rhs) {
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

QString OutPoint::toString() const {
    return _txId.toString() + "-" + QString::number(_index);
}
*/

TransactionId OutPoint::transactionId() const {
    return _txId;
}

void OutPoint::setTransactionId(const TransactionId & txId){
    _txId = txId;
}

quint32 OutPoint::index() const {
    return _index;
}

void OutPoint::setIndex(quint32 index) {
    _index = index;
}

}

uint qHash(const Coin::OutPoint & o) {
    return qHash(o.toString());
}
