/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#include <protocol/statemachine/ReadyToRequestPiece.hpp>
#include <protocol/statemachine/WaitingForFullPiece.hpp>
#include <wire/RequestFullPiece.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    ReadyToRequestPiece::ReadyToRequestPiece() {
        std::cout << "Entering ReadyToRequestPiece state." << std::endl;
    }

    sc::result ReadyToRequestPiece::react(const event::RequestPiece & e) {

        std::cout << "Reacting to RequestPiece event." << std::endl;

        // Request piece from seller
        context<CBStateMachine>()._sendMessage(new joystream::wire::RequestFullPiece(e.pieceIndex()));

        // Wait for
        return transit<WaitingForFullPiece>();
    }
}
}
}
