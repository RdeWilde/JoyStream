/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 17 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATE_MACHINE_READYFORINVITATION_HPP
#define JOYSTREAM_PROTOCOL_STATE_MACHINE_READYFORINVITATION_HPP

#include <protocol/statemachine/Selling.hpp>
#include <protocol/statemachine/event/Recv.hpp>
#include <wire/JoinContract.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class ReadyForInvitation : public sc::simple_state<ReadyForInvitation, Selling> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::Recv<joystream::wire::JoinContract>>
                                > reactions;

        ReadyForInvitation();

        // Event handlers
        sc::result react(const event::Recv<joystream::wire::JoinContract> &);

    };
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATE_MACHINE_READYFORINVITATION_HPP
