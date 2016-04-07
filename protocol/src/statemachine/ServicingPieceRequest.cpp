/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#include <protocol/statemachine/ServicingPieceRequest.hpp>

using namespace joystream::protocol::statemachine;

ServicingPieceRequest::ServicingPieceRequest() {
    std::cout << "Entering ServicingPieceRequest state." << std::endl;
}

sc::result ServicingPieceRequest::react(const event::Recv<wire::Observe> & e) {

    std::cout << "Reacting to Recv<wire::Observe> event." << std::endl;

    // Send client notification about this interruption,
    CBStateMachine & machine = context<CBStateMachine>();
    machine.peerInterruptedPayment()();

    // and update new peer mode
    machine.peerToObserveMode();

    // Terminate machine
    return terminate();
}

sc::result ServicingPieceRequest::react(const event::Recv<wire::Buy> & e) {

    std::cout << "Reacting to Recv<wire::Buy> event." << std::endl;

    // Send client notification about this interruption,
    context<CBStateMachine>().peerInterruptedPayment()();

    // and update new peer mode
    context<CBStateMachine>().peerToBuyMode(e.message()->terms());

    // Terminate machine
    return terminate();
}

sc::result ServicingPieceRequest::react(const event::Recv<wire::Sell> & e) {

    std::cout << "Reacting to Recv<wire::Sell> event." << std::endl;

    // Send client notification about this interruption,
    context<CBStateMachine>().peerInterruptedPayment()();

    // and update new peer mode
    wire::Sell const * m = e.message();
    context<CBStateMachine>().peerToSellMode(m->terms(), m->index());

    // Terminate machine
    return terminate();
}
