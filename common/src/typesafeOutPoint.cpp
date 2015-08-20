/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/typesafeOutPoint.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::OutPoint

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

typesafeOutPoint::typesafeOutPoint(const typesafeOutPoint & o)
    : typesafeOutPoint(o.transactionId(), o.index()) {
}

typesafeOutPoint::typesafeOutPoint(const OutPoint & outPoint)
    : _txId(outPoint.getHashLittleEndian())
    , _index(outPoint.index) {
}

typesafeOutPoint & typesafeOutPoint::operator=(const typesafeOutPoint& o) {

    _txId = o.transactionId();
    _index = o.index();

    return *this;
}

bool operator==(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs) {
    return (lhs.transactionId() < rhs.transactionId()) || ((lhs.transactionId() == rhs.transactionId()) && (lhs.index() < rhs.index()));
}

bool operator!=(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs) {
    return !(lhs == rhs);
}

bool operator<(const typesafeOutPoint & lhs, const typesafeOutPoint & rhs) {
    return (lhs.transactionId() < rhs.transactionId()) || ((lhs.transactionId() == rhs.transactionId()) && (lhs.index() < rhs.index()));
}

QString typesafeOutPoint::toString() const {
    return _txId.toHex() + "-" + QString::number(_index);
}

Coin::OutPoint typesafeOutPoint::getClassicOutPoint() const {
    return Coin::OutPoint(_txId.toUCharVector(), _index);
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
