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

    Buying::Buying()
        : _initialized(false) {
        std::cout << "Entering Buying state." << std::endl;
    }

    sc::result Buying::react(const detail::InitializeBuying & e) {

        if(_initialized)
            throw std::runtime_error("Buying state already initialized.");
        else
           _initialized = true;

        std::cout << "Reacting to detail::InitializeBuying." << std::endl;

        // Store terms
        _terms = e.terms();

        // Send message
        context<CBStateMachine>().sendMessage()(new wire::Buy(_terms));

        // No transition
        return discard_event();
    }

    sc::result Buying::react(const event::ObserveModeStarted & e) {

        if(!_initialized)
            throw std::runtime_error("Buying state not initialized.");

        std::cout << "Reacting to event::ObserveModeStarted." << std::endl;

        // Trigger initialization event for when we are in Observing state
        post_event(detail::InitializeObserving());

        // Transition to Observe state
        return transit<Observing>();
    }

    sc::result Buying::react(const event::SellModeStarted & e) {

        if(!_initialized)
            throw std::runtime_error("Buying state not initialized.");

        std::cout << "Reacting to event::SellModeStarted." << std::endl;

        // Trigger initialization event for when we are in Selling state
        post_event(detail::InitializeSelling(e.terms()));

        // Transition to Selling state
        return transit<Selling>();
    }

    sc::result Buying::react(const event::UpdateTerms<BuyerTerms> & e) {

        if(!_initialized)
            throw std::runtime_error("Selling state not initialized.");

        std::cout << "Reacting to event::UpdateTerms<BuyerTerms>." << std::endl;

        // Store terms
        _terms = e.terms();

        // Send message
        context<CBStateMachine>().sendMessage()(new wire::Buy(_terms));

        // Transition back to initial selling state
        return transit<Buying>();
    }

}
}
}
