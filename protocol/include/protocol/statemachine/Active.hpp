/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 11 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_ACTIVE_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_ACTIVE_HPP

#include <protocol/statemachine/CBStateMachine.hpp>
#include <boost/statechart/simple_state.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class Observe; // for some reason, we *must* have initial state for composite state
    class Active : public sc::simple_state<Active, CBStateMachine, Observe> {

    public:

    private:


    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_ACTIVE_HPP
