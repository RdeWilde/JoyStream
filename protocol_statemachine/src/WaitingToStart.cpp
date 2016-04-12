/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#include <protocol_statemachine/WaitingToStart.hpp>
#include <protocol_statemachine/ReadyForPieceRequest.hpp>

#include <iostream>

namespace joystream {
namespace protocol_statemachine {

    WaitingToStart::WaitingToStart() {
        std::cout << "Entering WaitingToStart state." << std::endl;
    }

    sc::result WaitingToStart::react(const event::Recv<protocol_wire::Ready> & e) {

        std::cout << "Reacting to Recv<wire::Ready> event." << std::endl;

        // Get contract anchor
        Coin::typesafeOutPoint anchor = e.message()->anchor();

        // Store anchor in payee
        context<CBStateMachine>()._payee.setContractOutPoint(anchor);

        // Switch peer state
        context<CBStateMachine>()._contractIsReady(anchor);

        // Transition to deep history
        return transit<ReadyForPieceRequest>();
    }

}
}
