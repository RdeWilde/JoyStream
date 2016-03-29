/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#include <protocol/statemachine/WaitingForFullPiece.hpp>
#include <protocol/statemachine/ProcessingPiece.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    WaitingForFullPiece::WaitingForFullPiece() {
        std::cout << "Entering WaitingForFullPiece state." << std::endl;
    }

    sc::result WaitingForFullPiece::react(const event::Recv<wire::FullPiece> & e) {

        std::cout << "Reacting to event::Recv<wire::FullPiece> event." << std::endl;

        // Send piece to client
        context<CBStateMachine>().getReceivedFullPiece()(e.message()->pieceData());

        // Wait for client side piece validation and storage
        return transit<ProcessingPiece>();
    }
}
}
}
