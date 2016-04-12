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

    ContractPrepared::ContractPrepared(const Coin::typesafeOutPoint & anchor)
        : _anchor(anchor) {

    }

    Coin::typesafeOutPoint ContractPrepared::anchor() const {
        return _anchor;
    }

}
}
}
