/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#include <protocol/statemachine/PieceRequested.hpp>
#include <protocol/statemachine/WaitingForPayment.hpp>
#include <protocol/wire/FullPiece.hpp>

#include <iostream>

using namespace joystream::protocol::statemachine;

PieceRequested::PieceRequested() {
    std::cout << "Entering PieceRequested state." << std::endl;
}

sc::result PieceRequested::react(const event::PieceLoaded & e) {

    std::cout << "Reacting to event::PieceLoaded event." << std::endl;

    // Send piece
    context<CBStateMachine>().sendMessage()(new wire::FullPiece(e.data(), e.length()));

    // Transition to WaitingForPayment state
    return transit<WaitingForPayment>();
}
