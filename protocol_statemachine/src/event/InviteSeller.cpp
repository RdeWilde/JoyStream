/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 26 2016
 */

#include <protocol_statemachine/event/InviteSeller.hpp>

namespace joystream {
namespace protocol_statemachine {
namespace event {

    InviteSeller::InviteSeller(quint64 value, const Coin::KeyPair & buyerContractKeyPair, const Coin::PubKeyHash & finalPkHash)
        : _value(value)
        , _buyerContractKeyPair(buyerContractKeyPair)
        , _finalPkHash(finalPkHash) {
    }

    quint64 InviteSeller::value() const {
        return _value;
    }

    Coin::KeyPair InviteSeller::buyerContractKeyPair() const {
        return _buyerContractKeyPair;
    }

    Coin::PubKeyHash InviteSeller::finalPkHash() const {
        return _finalPkHash;
    }
}
}
}
