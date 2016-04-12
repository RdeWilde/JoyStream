/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#include <protocol_statemachine/PreparingContract.hpp>
#include <protocol_statemachine/ReadyToRequestPiece.hpp>
#include <protocol_wire/Ready.hpp>

namespace joystream {
namespace protocol_statemachine {

    PreparingContract::PreparingContract() {
        std::cout << "Entering PreparingContract state." << std::endl;
    }

    sc::result PreparingContract::react(const event::ContractPrepared & e) {

        std::cout << "Reacting to ContractPrepared event." << std::endl;

        // Send ready message
        context<CBStateMachine>()._sendMessage(new joystream::protocol_wire::Ready(e.anchor()));

        // Set anchor in payor
        context<CBStateMachine>()._payor.setAnchor(e.anchor());

        // Now ready to request first piece
        return transit<ReadyToRequestPiece>();
    }
}
}
