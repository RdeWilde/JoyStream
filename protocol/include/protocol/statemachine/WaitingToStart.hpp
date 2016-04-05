/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATE_MACHINE_WAITINGTOSTART_HPP
#define JOYSTREAM_PROTOCOL_STATE_MACHINE_WAITINGTOSTART_HPP

#include <protocol/statemachine/Selling.hpp>
#include <protocol/statemachine/event/Recv.hpp>
#include <wire/Ready.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class WaitingToStart : public sc::simple_state<WaitingToStart, Selling> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::Recv<joystream::wire::Ready>>
                                > reactions;

        WaitingToStart();

        // Event handlers
        sc::result react(const event::Recv<joystream::wire::Ready> &);

    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_STATE_MACHINE_WAITINGTOSTART_HPP
