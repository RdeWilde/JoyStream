/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_CHOOSEMODE_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_CHOOSEMODE_HPP

#include <protocol/statemachine/CBStateMachine.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class ChooseMode : public sc::simple_state<ChooseMode, CBStateMachine> {

    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_CHOOSEMODE_HPP
