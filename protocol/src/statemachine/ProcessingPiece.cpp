/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#include <protocol/statemachine/ProcessingPiece.hpp>
#include <protocol/statemachine/ReadyToRequestPiece.hpp>
#include <wire/Payment.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    ProcessingPiece::ProcessingPiece() {
        std::cout << "Entering ProcessingPiece state." << std::endl;
    }

    sc::result ProcessingPiece::react(const event::SendPayment &) {

        std::cout << "Reacting to SendPayment event." << std::endl;

        // Make payment signature
        Coin::Signature sig = context<CBStateMachine>()._payor.makePayment();

        // Send payment
        context<CBStateMachine>()._sendMessage(new joystream::wire::Payment(sig));

        // Now able to request another piece
        return transit<ReadyToRequestPiece>();
    }

    sc::result ProcessingPiece::react(const event::InvalidPieceReceived &) {

        std::cout << "Reacting to InvalidPieceReceived event." << std::endl;

        // Stop state machine
        return terminate();
    }

}
}
}

