/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#include <protocol/statemachine/ReadyForPieceRequest.hpp>
#include <protocol/statemachine/ServicingPieceRequest.hpp>

using namespace joystream::protocol::statemachine;

ReadyForPieceRequest::ReadyForPieceRequest() {
    std::cout << "Entering ReadyForPieceRequest state." << std::endl;
}

sc::result ReadyForPieceRequest::react(const event::Recv<joystream::protocol::wire::RequestFullPiece> & e) {

    std::cout << "Reacting to Recv<wire::RequestFullPiece> event." << std::endl;

    // Get reference to state machine
    CBStateMachine & machine = context<CBStateMachine>();

    // and check that piece requested is valid
    int pieceIndex = e.message()->pieceIndex();

    if(pieceIndex < 0 || pieceIndex >  machine.MAX_PIECE_INDEX())
        machine.invalidPieceRequested()();
    else
        machine.pieceRequested()(pieceIndex);

    // Get ready to load the piece
    return transit<ServicingPieceRequest>();
}
