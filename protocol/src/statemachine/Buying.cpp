/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#include <protocol/statemachine/Buying.hpp>
#include <protocol/statemachine/Selling.hpp>

#include <iostream>

namespace joystream {
namespace protocol {
namespace statemachine {

    Buying::Buying() {

    }

    sc::result Buying::react(const event::ObserveModeStarted & e) {

        std::cout << "Reacting to event::ObserveModeStarted." << std::endl;

        // Switch client state
        context<CBStateMachine>().clientToObserveMode();

        // Transition to Observe state
        return transit<Observing>();
    }

    sc::result Buying::react(const event::SellModeStarted & e) {

        std::cout << "Reacting to event::SellModeStarted." << std::endl;

        // Switch client state
        context<CBStateMachine>().clientToSellMode(e.terms());

        // Transition to Sell state
        return transit<Selling>();
    }

}
}
}
