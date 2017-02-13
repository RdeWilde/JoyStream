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
        : Ready(0, Coin::typesafeOutPoint(), Coin::PublicKey(), Coin::PubKeyHash()) {
    }

    Ready::Ready(uint64_t value, const Coin::typesafeOutPoint & anchor, const Coin::PublicKey & contractPk, const Coin::PubKeyHash &finalPkHash)
        : _value(value)
        , _anchor(anchor)
        , _contractPk(contractPk)
        , _finalPkHash(finalPkHash) {
    }

    bool Ready::operator==(const Ready & rhs) const {
        return _value == rhs.value() &&
               _anchor == rhs.anchor() &&
               _contractPk == rhs.contractPk() &&
               _finalPkHash == rhs.finalPkHash();
    }

    MessageType Ready::messageType() const {
        return MessageType::ready;
    }

    uint64_t Ready::value() const {
        return _value;
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

    void Ready::setAnchor(const Coin::typesafeOutPoint &anchor) {
        _anchor = anchor;
    }

    void Ready::setContractPk(const Coin::PublicKey & contractPk) {
        _contractPk = contractPk;
    }

    void Ready::setFinalPkHash(const Coin::PubKeyHash & finalPkHash) {
        _finalPkHash = finalPkHash;
    }

    void Ready::setValue(uint64_t value) {
        _value = value;
    }
}
}
