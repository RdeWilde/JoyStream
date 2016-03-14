/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#include <protocol/statemachine/Selling.hpp>
#include <protocol/statemachine/Buying.hpp>

#include <iostream>

namespace joystream {
namespace protocol {
namespace statemachine {

    Selling::Selling() {
        std::cout << "Entering Sell state." << std::endl;
    }

    sc::result Selling::react(const event::ObserveModeStarted & e) {

        std::cout << "Reacting to event::ObserveModeStarted." << std::endl;

        // Switch client state
        context<CBStateMachine>().clientToObserveMode();

        // Transition to Observe state
        return transit<Observing>();
    }

    sc::result Selling::react(const event::BuyModeStarted & e) {

        std::cout << "Reacting to event::BuyModeStarted." << std::endl;

        // Switch client state
        context<CBStateMachine>().clientToBuyMode(e.terms());

        // Transition to Buy state
        return transit<Buying>();
    }

}
}
}
