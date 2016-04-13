/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 17 2016
 */

#include <protocol_statemachine/ReadyForInvitation.hpp>
#include <protocol_statemachine/Invited.hpp>
#include <protocol_statemachine/exception/InvitedToJoinContractByNonBuyer.hpp>

#include <iostream>

namespace joystream {
namespace protocol_statemachine {

    ReadyForInvitation::ReadyForInvitation() {
        std::cout << "Entering ReadyForInvitation state." << std::endl;
    }

    sc::result ReadyForInvitation::react(const event::Recv<protocol_wire::JoinContract> & e) {

        std::cout << "Reacting to Recv<wire::JoinContract>." << std::endl;

        // Make sure the peer is actually a buyer, otherwise throw exception
        ModeAnnounced mode = context<CBStateMachine>().announcedModeAndTermsFromPeer().modeAnnounced();

        if(mode != ModeAnnounced::buy)
            throw exception::InvitedToJoinContractByNonBuyer(mode);

        // If invitation doesn't match most recent terms sent,
        if(e.message().index() != context<CBStateMachine>()._index) {

            // then notify user
            context<CBStateMachine>()._invitedToOutdatedContract();

            // and drop event
            return discard_event();
        } else {

            // Notify client
            context<CBStateMachine>()._invitedToJoinContract();

            // and make transition
            return transit<Invited>();
        }
    }
}
}
