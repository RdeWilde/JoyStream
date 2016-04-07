/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 17 2016
 */

#include <protocol/statemachine/ReadyForInvitation.hpp>
#include <protocol/statemachine/Invited.hpp>
#include <protocol/statemachine/exception/InvitedToJoinContractByNonBuyer.hpp>

#include <iostream>

namespace joystream {
namespace protocol {
namespace statemachine {

    ReadyForInvitation::ReadyForInvitation() {
        std::cout << "Entering ReadyForInvitation state." << std::endl;
    }

    sc::result ReadyForInvitation::react(const event::Recv<wire::JoinContract> & e) {

        std::cout << "Reacting to Recv<wire::JoinContract>." << std::endl;

        // Get message
        wire::JoinContract const * message = e.message();

        // Make sure the peer is actually a buyer, otherwise throw exception
        ModeAnnounced mode = context<CBStateMachine>().peerAnnouncedMode().modeAnnounced();

        if(mode != ModeAnnounced::buy)
            throw exception::InvitedToJoinContractByNonBuyer(mode);

        // If invitation doesn't match most recent terms sent,
        if(message->index() != context<CBStateMachine>()._index) {

            // then notify user
            context<CBStateMachine>().invitedToOutdatedContract()();

            // and drop event
            return discard_event();
        } else {

            // Store invitation information in payee
            joystream::wire::ContractInvitation invitation = message->invitation();
            context<CBStateMachine>()._payee.setFunds(invitation.value());
            context<CBStateMachine>()._payee.setPayorContractPk(invitation.contractPk());
            context<CBStateMachine>()._payee.setPayorFinalPkHash(invitation.finalPkHash());

            // otherwise, notify user
            context<CBStateMachine>().invitedToJoinContract()(message->invitation());

            // and make transition
            return transit<Invited>();
        }
    }
}
}
}
