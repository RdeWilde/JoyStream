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

        std::cout << "Reacting to event::Recv<wire::JoinContract>." << std::endl;

        // Get message
        wire::JoinContract const * message = e.message();
        CBStateMachine & machine = context<CBStateMachine>();

        // Make sure the peer is actually a buyer, otherwise throw exception
        ModeAnnounced mode = machine.peerAnnouncedMode().modeAnnounced();

        if(mode != ModeAnnounced::buy)
            throw exception::InvitedToJoinContractByNonBuyer(mode);

        // Get reference to Selling state
        Selling & sellingState = context<Selling>();

        // If invitation doesn't match most recent terms sent,
        if(message->index() != sellingState.index()) {

            // then notify user
            machine.invitedToOutdatedContract()();

            // and drop event
            return discard_event();
        } else {

            // Store invitation information in payee
            ContractInvitation invitation = message->invitation();
            sellingState._payee.setFunds(invitation.value());
            sellingState._payee.setPayorContractPk(invitation.contractPk());
            sellingState._payee.setPayorFinalPkHash(invitation.finalPkHash());

            // otherwise, notify user
            machine.invitedToJoinContract()(message->invitation());

            // and make transition
            return transit<Invited>();
        }
    }
}
}
}
