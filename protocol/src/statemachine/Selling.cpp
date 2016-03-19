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

    Selling::Selling()
        : _index(0) {
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

    sc::result Selling::react(const event::UpdateTerms<SellerTerms> & e) {

        // Increment term index
        _index++;

        // Create sell message and send
        context<CBStateMachine>().sendMessage()(new wire::Sell(e.terms(), _index));

        // Transition back to initial selling state
        return transit<Selling>();
    }

    uint32_t Selling::index() const {
        return _index;
    }

}
}
}
