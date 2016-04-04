/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <wire/SignRefund.hpp>
#include <wire/MessageType.hpp>

#include <QDataStream>

namespace joystream {
namespace wire {

    SignRefund::SignRefund() {
    }

    SignRefund::SignRefund(const Coin::TransactionId & contractTxId, quint32 index, quint64 value, const Coin::PublicKey & contractPk, const Coin::PublicKey & finalPk)
        : _contractTxId(contractTxId)
        , _index(index)
        , _value(value)
        , _contractPk(contractPk)
        , _finalPk(finalPk) {
    }

    SignRefund::SignRefund(QDataStream & stream) {

        // DOESN'T LINK: stream >> _contractTxId >> _index >> _value >> _contractPk >> _finalPk;
        Coin::operator >> (stream, _contractTxId);
        stream >> _index;
        stream >> _value;
        Coin::operator >> (stream, _contractPk);
        Coin::operator >> (stream, _finalPk);
    }

    MessageType SignRefund::messageType() const {
        return MessageType::sign_refund;
    }

    quint32 SignRefund::length() const {
        return Coin::TransactionId::length() + sizeof(quint32) + sizeof(quint64) + Coin::PublicKey::length() + Coin::PublicKey::length();
    }

    void SignRefund::write(QDataStream & stream) const {

        // DOESN'T LINK: stream << _contractTxId << _index << _value << _contractPk << _finalPk
        Coin::operator << (stream, _contractTxId);
        stream << _index;
        stream << _value;
        Coin::operator << (stream, _contractPk);
        Coin::operator << (stream, _finalPk);
    }

    Coin::TransactionId SignRefund::contractTxId() const {
        return _contractTxId;
    }

    void SignRefund::setContractTxId(const Coin::TransactionId & contractTxId) {
        _contractTxId = contractTxId;
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

    Coin::PublicKey SignRefund::contractPk() const {
        return _contractPk;
    }

    void SignRefund::setContractPk(const Coin::PublicKey & contractPk) {
        _contractPk = contractPk;
    }

    Coin::PublicKey SignRefund::finalPk() const {
        return _finalPk;
    }

    void SignRefund::setFinalPk(const Coin::PublicKey & finalPk) {
        _finalPk = finalPk;
    }

}
}
