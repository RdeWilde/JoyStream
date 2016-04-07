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

sc::result ReadyForPieceRequest::react(const event::Recv<joystream::wire::RequestFullPiece> & e) {

    std::cout << "Reacting to Recv<wire::RequestFullPiece> event." << std::endl;

    // and check that piece requested is valid
    int pieceIndex = e.message()->pieceIndex();

    // if not, send notification and terminate
    if(pieceIndex < 0 || pieceIndex > context<CBStateMachine>().MAX_PIECE_INDEX()) {

        context<CBStateMachine>().invalidPieceRequested()();

        return terminate();
    }

    // otherwise send success notification, and
    context<CBStateMachine>().pieceRequested()(pieceIndex);

    // get ready to load the piece
    return transit<ServicingPieceRequest>();
}
