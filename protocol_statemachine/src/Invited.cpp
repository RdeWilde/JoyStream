/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 17 2016
 */

#include <protocol_statemachine/Invited.hpp>
#include <protocol_statemachine/WaitingToStart.hpp>
#include <protocol_wire/protocol_wire.hpp>

namespace joystream {
namespace protocol_statemachine {

    Invited::Invited() {
        std::cout << "Entering Invited state." << std::endl;
    }

    sc::result Invited::react(const event::Joined & e) {

        std::cout << "Reacting to JoinContract." << std::endl;

        // Store contract rsvp information in payee
        context<CBStateMachine>()._payee.setPayeeContractKeys(e.contractKeys());
        context<CBStateMachine>()._payee.setPayeeFinalPkHash(e.finalPkHash());

        // Send message for joining contract
        context<CBStateMachine>()._sendJoiningContractMessage(protocol_wire::JoiningContract(e.contractKeys().pk(), e.finalPkHash()));

        // Transition to WaitingToStart state
        return transit<WaitingToStart>();
    }
}
}
