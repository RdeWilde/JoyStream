/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#include <protocol_statemachine/event/ContractPrepared.hpp>

namespace joystream {
namespace protocol_statemachine {
namespace event {

    ContractPrepared::ContractPrepared(const Coin::typesafeOutPoint & anchor, const Coin::KeyPair & buyerContractKeyPair, const Coin::PubKeyHash & finalPkHash, quint64 value)
        : _anchor(anchor)
        , _buyerContractKeyPair(buyerContractKeyPair)
        , _finalPkHash(finalPkHash)
        , _value(value) {
    }

    Coin::typesafeOutPoint ContractPrepared::anchor() const {
        return _anchor;
    }

    Coin::KeyPair ContractPrepared::contractKeyPair() const {
        return _buyerContractKeyPair;
    }

    Coin::PubKeyHash ContractPrepared::finalPkHash() const {
        return _finalPkHash;
    }

    quint64 ContractPrepared::value() const {
        return _value;
    }
}
}
}
