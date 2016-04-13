/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 11 2016
 */

#include <protocol_statemachine/ChooseMode.hpp>
#include <protocol_statemachine/Observing.hpp>
#include <protocol_statemachine/Selling.hpp>
#include <protocol_statemachine/Buying.hpp>

#include <iostream>

namespace joystream {
namespace protocol_statemachine {

    ChooseMode::ChooseMode() {
        std::cout << "Entering ChooseMode state." << std::endl;
    }

    sc::result ChooseMode::react(const event::ObserveModeStarted &) {

        std::cout << "Reacting to ObserveModeStarted." << std::endl;

        // Client to observe mode
        context<CBStateMachine>().clientToObserveMode();

        // Transition to Observe state
        return transit<Observing>();
    }

    sc::result ChooseMode::react(const event::SellModeStarted & e) {

        std::cout << "Reacting to SellModeStarted." << std::endl;

        // Client to sell mode
        context<CBStateMachine>().clientToSellMode(e.terms(), 0);

        // Transition to Sell state
        return transit<Selling>();
    }

    sc::result ChooseMode::react(const event::BuyModeStarted & e) {

        std::cout << "Reacting to BuyModeStarted." << std::endl;

        // Client to buy mode
        context<CBStateMachine>().clientToBuyMode(e.terms());

        // Transition to Buy state
        return transit<Buying>();
    }
}
}
