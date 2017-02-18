/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#include <protocol_statemachine/ReadyToRequestPiece.hpp>
#include <protocol_statemachine/WaitingForFullPiece.hpp>
#include <protocol_wire/protocol_wire.hpp>

namespace joystream {
namespace protocol_statemachine {

    ReadyToRequestPiece::ReadyToRequestPiece() {
        std::cout << "Entering ReadyToRequestPiece state." << std::endl;
    }

    sc::result ReadyToRequestPiece::react(const event::RequestPiece & e) {

        std::cout << "Reacting to RequestPiece event." << std::endl;

        // Request piece from seller
        context<CBStateMachine>()._sendRequestFullPieceMessage(protocol_wire::RequestFullPiece(e.pieceIndex()));

        // Wait for
        return transit<WaitingForFullPiece>();
    }
}
}
