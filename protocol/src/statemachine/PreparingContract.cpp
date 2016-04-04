/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#include <protocol/statemachine/PreparingContract.hpp>
#include <protocol/statemachine/ReadyToRequestPiece.hpp>
#include <protocol/wire/Ready.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    PreparingContract::PreparingContract() {
        std::cout << "Entering PreparingContract state." << std::endl;
    }

    sc::result PreparingContract::react(const event::ContractPrepared & e) {

        std::cout << "Reacting to ContractPrepared event." << std::endl;

        // Send ready message
        context<CBStateMachine>().sendMessage()(new wire::Ready(e.anchor()));

        // Set anchor in payor
        context<Buying>()._payor.setAnchor(e.anchor());

        // Now ready to request first piece
        return transit<ReadyToRequestPiece>();
    }
}
}
}
