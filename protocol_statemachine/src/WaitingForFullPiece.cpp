/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#include <protocol_statemachine/WaitingForFullPiece.hpp>
#include <protocol_statemachine/ProcessingPiece.hpp>

namespace joystream {
namespace protocol_statemachine {

    WaitingForFullPiece::WaitingForFullPiece() {
        std::cout << "Entering WaitingForFullPiece state." << std::endl;
    }

    sc::result WaitingForFullPiece::react(const event::Recv<protocol_wire::FullPiece> & e) {

        std::cout << "Reacting to Recv<wire::FullPiece> event." << std::endl;

        // Send piece to client
        context<CBStateMachine>()._receivedFullPiece.enqueue(e.message().pieceData());

        // Wait for client side piece validation and storage
        return transit<ProcessingPiece>();
    }
}
}
