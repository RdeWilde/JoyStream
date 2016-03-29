/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 27 2016
 */

#include <protocol/statemachine/WaitingForSellerToJoin.hpp>

#include <protocol/statemachine/SellerJoined.hpp>


namespace joystream {
namespace protocol {
namespace statemachine {

    WaitingForSellerToJoin::WaitingForSellerToJoin() {
        std::cout << "Entering WaitingForSellerToJoin state." << std::endl;
    }

    sc::result WaitingForSellerToJoin::react(const event::Recv<wire::JoiningContract> & e) {

        std::cout << "Reacting to event::Recv<wire::JoiningContract> event." << std::endl;

        // Get reference to outer state
        Buying & buying = context<Buying>();

        // Update payor based on rsvp
        ContractRSVP rsvp = e.message()->rsvp();
        buying._payor.setPayeeContractPk(rsvp.contractPk());
        buying._payor.setPayeeFinalPkHash(rsvp.finalPkHash());

        // Send client notification about seller joining
        context<CBStateMachine>().getSellerJoined()();

        // Start waiting for contract to be prepared for joined seller
        transit<SellerJoined>();
    }

}
}
}
