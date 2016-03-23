/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 17 2016
 */

#include <protocol/statemachine/Invited.hpp>
#include <protocol/wire/JoiningContract.hpp>
#include <protocol/statemachine/WaitingToStart.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    Invited::Invited() {
        std::cout << "Entering Invited state." << std::endl;
    }

    sc::result Invited::react(const event::Joined & e) {

        std::cout << "Reacting to event::JoinContract." << std::endl;

        // Store contract rsvp information in payee
        Selling & sellingState = context<Selling>();
        sellingState._payee.setPayeeContractKeys(e.contractKeys());
        sellingState._payee.setPayeeFinalPkHash(e.finalPkHash());

        // Send message for joining contract
        ContractRSVP rsvp(e.contractKeys().pk(), e.finalPkHash());
        context<CBStateMachine>().sendMessage()(new wire::JoiningContract(rsvp));

        // Transition to WaitingToStart state
        return transit<WaitingToStart>();
    }
}
}
}
