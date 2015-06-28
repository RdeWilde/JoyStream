/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "SignRefund.hpp"
#include "MessageType.hpp"

#include <QDataStream>

SignRefund::SignRefund() {
}

SignRefund::SignRefund(const TxId & hash, quint32 index, quint64 value, const PublicKey & contractPk, const PublicKey & finalPk)
    : _hash(hash)
    , _index(index)
    , _value(value)
    , _contractPk(contractPk)
    , _finalPk(finalPk) {
}

SignRefund::SignRefund(QDataStream & stream) {
    stream >> _hash >> _index >> _value >> _contractPk >> _finalPk;
}

MessageType SignRefund::messageType() const {
    return MessageType::sign_refund;
}

quint32 SignRefund::length() const {
    return TxId::length + sizeof(quint32) + sizeof(quint64) + PublicKey::length + PublicKey::length;
}

void SignRefund::write(QDataStream & stream) const {
    stream << _hash << _index << _value << _contractPk << _finalPk;
}

TxId SignRefund::hash() const {
    return _hash;
}

void SignRefund::setHash(const TxId & hash) {
    _hash = hash;
}

quint32 SignRefund::index() const {
    return _index;
}

void SignRefund::setIndex(quint32 index) {
    _index = index;
}

quint64 SignRefund::value() const {
    return _value;
}

void SignRefund::setValue(quint64 value) {
    _value = value;
}

PublicKey SignRefund::contractPk() const {
    return _contractPk;
}

void SignRefund::setContractPk(const PublicKey & contractPk) {
    _contractPk = contractPk;
}

PublicKey SignRefund::finalPk() const {
    return _finalPk;
}

void SignRefund::setFinalPk(const PublicKey & finalPk) {
    _finalPk = finalPk;
}
