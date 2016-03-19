/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 17 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATE_MACHINE_INVITED_HPP
#define JOYSTREAM_PROTOCOL_STATE_MACHINE_INVITED_HPP

#include <protocol/statemachine/Selling.hpp>
#include <protocol/statemachine/event/Joined.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class Invited : public sc::simple_state<Invited, Selling> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::Joined>
                                > reactions;

        Invited();

        // Event handlers
        sc::result react(const event::Joined &);
    };
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATE_MACHINE_INVITED_HPP
