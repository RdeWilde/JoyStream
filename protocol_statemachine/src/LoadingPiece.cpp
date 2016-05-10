/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#include <protocol_statemachine/LoadingPiece.hpp>
#include <protocol_statemachine/WaitingForPayment.hpp>
#include <protocol_wire/protocol_wire.hpp>

#include <iostream>

namespace joystream {
namespace protocol_statemachine {

    LoadingPiece::LoadingPiece() {
        std::cout << "Entering PieceRequested state." << std::endl;
    }

    sc::result LoadingPiece::react(const event::PieceLoaded & e) {

        std::cout << "Reacting to PieceLoaded event." << std::endl;

        // Send piece
        context<CBStateMachine>()._sendMessage(joystream::protocol_wire::FullPiece(e.pieceData()));

        // Transition to WaitingForPayment state
        return transit<WaitingForPayment>();
    }

}
}
