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

    Ready::Ready() {
    }

    Ready::Ready(const Coin::typesafeOutPoint & anchor)
        : _anchor(anchor) {
    }

    Ready::Ready(QDataStream & stream) {

        //stream >>
        stream >> _anchor;

        // contractPk
        stream >> _contractPk;

        // finalPkHash
        stream >> _finalPkHash;
    }

    MessageType Ready::messageType() const {
        return MessageType::ready;
    }

    quint32 Ready::length() const {
        return Coin::typesafeOutPoint::length() + Coin::PublicKey::length() + Coin::PubKeyHash::length();
    }

    void Ready::write(QDataStream & stream) const {

        // _anchor
        stream << _anchor;

        // _contractPk
        stream << _contractPk;

        // _finalPkHash
        stream << _finalPkHash;
    }

    Coin::typesafeOutPoint Ready::anchor() const {
        return _anchor;
    }

    Coin::PublicKey Ready::contractPk() const {
        return _contractPk;
    }

    Coin::PubKeyHash Ready::finalPkHash() const {
        return _finalPkHash;
    }
}
}
