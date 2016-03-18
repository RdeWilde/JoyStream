/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 17 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATE_MACHINE_INVITED_HPP
#define JOYSTREAM_PROTOCOL_STATE_MACHINE_INVITED_HPP

#include <protocol/statemachine/Selling.hpp>
#include <protocol/statemachine/event/Recv.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class Invited : public sc::simple_state<Invited, Selling> {

    public:

        Invited();

    };
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATE_MACHINE_INVITED_HPP

