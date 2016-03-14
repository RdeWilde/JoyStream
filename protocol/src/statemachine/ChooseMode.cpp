/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 11 2016
 */

#include <protocol/statemachine/ChooseMode.hpp>
#include <protocol/statemachine/Observing.hpp>
#include <protocol/statemachine/Selling.hpp>
#include <protocol/statemachine/Buying.hpp>

#include <iostream>

namespace joystream {
namespace protocol {
namespace statemachine {

    ChooseMode::ChooseMode() {
        std::cout << "Entering ChooseMode state." << std::endl;
    }

    sc::result ChooseMode::ChooseMode::react(const event::ObserveModeStarted & e) {

        std::cout << "Reacting to event::ObserveModeStarted." << std::endl;

        // Switch client state
        context<CBStateMachine>().clientToObserveMode();

        // Transition to Observe state
        return transit<Observing>();
    }

    sc::result ChooseMode::react(const event::SellModeStarted & e) {

        std::cout << "Reacting to event::SellModeStarted." << std::endl;

        // Switch client state
        context<CBStateMachine>().clientToSellMode(e.terms());

        // Transition to Sell state
        return transit<Selling>();
    }

    sc::result ChooseMode::react(const event::BuyModeStarted & e) {

        std::cout << "Reacting to event::BuyModeStarted." << std::endl;

        // Switch client state
        context<CBStateMachine>().clientToBuyMode(e.terms());

        // Transition to Buy state
        return transit<Buying>();
    }
}
}
}
