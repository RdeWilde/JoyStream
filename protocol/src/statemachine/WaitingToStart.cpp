/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#include <protocol/statemachine/WaitingToStart.hpp>
#include <protocol/statemachine/ReadyForPieceRequest.hpp>

#include <iostream>

using namespace joystream::protocol::statemachine;

WaitingToStart::WaitingToStart() {
    std::cout << "Entering WaitingToStart state." << std::endl;
}

sc::result WaitingToStart::react(const event::Recv<joystream::protocol::wire::Ready> & e) {

    std::cout << "Reacting to Recv<wire::Ready> event." << std::endl;

    // Switch peer state
    context<CBStateMachine>().contractIsReady()(e.message()->anchor());

    // Transition to deep history
    return transit<ReadyForPieceRequest>();
}
