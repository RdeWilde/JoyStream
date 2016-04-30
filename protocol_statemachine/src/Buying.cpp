/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#include <protocol_statemachine/Buying.hpp>
#include <protocol_statemachine/Selling.hpp>

#include <iostream>

namespace joystream {
namespace protocol_statemachine {

    Buying::Buying() {
        std::cout << "Entering Buying state." << std::endl;
    }

    sc::result Buying::react(const event::ObserveModeStarted &) {

        std::cout << "Reacting to ObserveModeStarted." << std::endl;

        // Client to observe mode
        context<CBStateMachine>().clientToObserveMode();

        // Transiti
        return transit<Observing>();
    }

    sc::result Buying::react(const event::SellModeStarted & e) {

        std::cout << "Reacting to SellModeStarted." << std::endl;

        // Client to Selling mode
        context<CBStateMachine>().clientToSellMode(e.terms(), 0);

        // Transition to Selling state
        return transit<Selling>();
    }

    sc::result Buying::react(const event::UpdateTerms<joystream::protocol_wire::BuyerTerms> & e) {

        std::cout << "Reacting to UpdateTerms<BuyerTerms>." << std::endl;

        // Client to Buying mode
        context<CBStateMachine>().clientToBuyMode(e.terms());

        // Transition back to initial Buying state
        return transit<Buying>();
    }
}

protocol_wire::BuyerTerms Buying::getTerms() const
{
    return _terms;
}

}
