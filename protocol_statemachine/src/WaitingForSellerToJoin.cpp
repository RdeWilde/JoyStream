/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 27 2016
 */

#include <protocol_statemachine/WaitingForSellerToJoin.hpp>
#include <protocol_statemachine/SellerHasJoined.hpp>

namespace joystream {
namespace protocol_statemachine {

    WaitingForSellerToJoin::WaitingForSellerToJoin() {
        std::cout << "Entering WaitingForSellerToJoin state." << std::endl;
    }

    sc::result WaitingForSellerToJoin::react(const event::Recv<protocol_wire::JoiningContract> & e) {

        std::cout << "Reacting to Recv<protocol_wire::JoiningContract> event." << std::endl;

        // Update payor based on rsvp
        context<CBStateMachine>()._payor.setPayeeContractPk(e.message().contractPk());
        context<CBStateMachine>()._payor.setPayeeFinalPkHash(e.message().finalPkHash());

        // Send client notification about seller joining
        context<CBStateMachine>()._sellerJoined.enqueue();

        // Start waiting for contract to be prepared for joined seller
        return transit<SellerHasJoined>();
    }

}
}
