/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#include <protocol/statemachine/Active.hpp>

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_OBSERVE_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_OBSERVE_HPP

namespace sc = boost::statechart;

namespace joystream {
namespace protocol {
namespace statemachine {

    class Observing : public sc::simple_state<Observing, Active> {

    public:

        Observing();

    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_OBSERVE_HPP

