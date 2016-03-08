/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#include <protocol/statemachine/CBStateMachine.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    CBStateMachine::CBStateMachine(State state)
        : _state(state)
        , _buy(this)
        , _sell(this) {
    }

    CBStateMachine::State CBStateMachine::state() const {
        return _state;
    }

}
}
}
