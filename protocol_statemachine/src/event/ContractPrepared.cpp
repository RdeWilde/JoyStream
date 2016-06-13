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

    ContractPrepared::ContractPrepared() {
    }

    ContractPrepared::ContractPrepared(const Coin::typesafeOutPoint & anchor, const Coin::KeyPair & buyerContractKeyPair, const Coin::RedeemScriptHash & finalScriptHash, quint64 value)
        : _anchor(anchor)
        , _buyerContractKeyPair(buyerContractKeyPair)
        , _finalScriptHash(finalScriptHash)
        , _value(value) {
    }

    Coin::typesafeOutPoint ContractPrepared::anchor() const {
        return _anchor;
    }

    Coin::KeyPair ContractPrepared::contractKeyPair() const {
        return _buyerContractKeyPair;
    }

    Coin::RedeemScriptHash ContractPrepared::finalScriptHash() const {
        return _finalScriptHash;
    }

    quint64 ContractPrepared::value() const {
        return _value;
    }
}
}
}
