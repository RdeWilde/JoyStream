/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 24 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_READYTOINVITESELLER_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_READYTOINVITESELLER_HPP

#include <protocol_statemachine/Buying.hpp>
#include <protocol_statemachine/event/InviteSeller.hpp>

namespace joystream {
namespace protocol_statemachine {

    class ReadyToInviteSeller : public sc::simple_state<ReadyToInviteSeller, Buying> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::InviteSeller>
                                > reactions;

        ReadyToInviteSeller();

        // Event handlers
        sc::result react(const event::InviteSeller &);

    };

}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_READYTOINVITESELLER_HPP
