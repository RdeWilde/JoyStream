/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#include <protocol/statemachine/event/Joined.hpp>

using namespace joystream::protocol::statemachine::event;

Joined::Joined(const Coin::KeyPair & contractKeys, const Coin::PubKeyHash & finalPkHash)
    : _contractKeys(contractKeys)
    , _finalPkHash(finalPkHash) {
}

Coin::KeyPair Joined::contractKeys() const {
    return _contractKeys;
}

Coin::PubKeyHash Joined::finalPkHash() const {
    return _finalPkHash;
}
