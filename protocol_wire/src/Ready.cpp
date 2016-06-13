/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol_wire/Ready.hpp>
#include <protocol_wire/MessageType.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::OutPoint

namespace joystream {
namespace protocol_wire {

    Ready::Ready()
        : Ready(0, Coin::typesafeOutPoint(), Coin::PublicKey(), Coin::RedeemScriptHash()) {
    }

    Ready::Ready(quint64 value, const Coin::typesafeOutPoint & anchor, const Coin::PublicKey & contractPk, const Coin::RedeemScriptHash & finalScriptHash)
        : _value(value)
        , _anchor(anchor)
        , _contractPk(contractPk)
        , _finalScriptHash(finalScriptHash) {
    }

    Ready::Ready(QDataStream & stream) {
        stream >> _value >> _anchor >> _contractPk >> _finalScriptHash;
    }

    bool Ready::operator==(const Ready & rhs) const {
        return _value == rhs.value() &&
               _anchor == rhs.anchor() &&
               _contractPk == rhs.contractPk() &&
               _finalScriptHash == rhs.finalScriptHash();
    }

    MessageType Ready::messageType() const {
        return MessageType::ready;
    }

    quint32 Ready::length() const {
        return sizeof(quint64) + Coin::typesafeOutPoint::length() + Coin::PublicKey::length() + Coin::PubKeyHash::length();
    }

    void Ready::write(QDataStream & stream) const {
        stream << _value << _anchor << _contractPk << _finalScriptHash;
    }

    quint64 Ready::value() const {
        return _value;
    }

    Coin::typesafeOutPoint Ready::anchor() const {
        return _anchor;
    }

    Coin::PublicKey Ready::contractPk() const {
        return _contractPk;
    }

    Coin::RedeemScriptHash Ready::finalScriptHash() const {
        return _finalScriptHash;
    }

}
}
