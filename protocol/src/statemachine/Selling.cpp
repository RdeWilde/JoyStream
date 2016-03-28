/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#include <protocol/statemachine/Selling.hpp>
#include <protocol/statemachine/Buying.hpp>

#include <iostream>
#include <cassert>

namespace joystream {
namespace protocol {
namespace statemachine {

    Selling::Selling()
        : _initialized(false)
        , _index(0) {
        std::cout << "Entering Selling state." << std::endl;
    }

    sc::result Selling::react(const detail::InitializeSelling & e) {

        if(_initialized)
            throw std::runtime_error("Selling state already initialized.");
        else
           _initialized = true;

        std::cout << "Reacting to detail::InitializeSelling." << std::endl;

        assert(_index == 0);

        // Update payee based on terms
        updatePayeeTerms(e.terms());

        // Send message
        context<CBStateMachine>().sendMessage()(new wire::Sell(e.terms(), _index));

        // No transition
        return discard_event();
    }

    sc::result Selling::react(const event::ObserveModeStarted & e) {

        if(!_initialized)
            throw std::runtime_error("Selling state not initialized.");

        std::cout << "Reacting to event::ObserveModeStarted." << std::endl;

        // Trigger initialization event for when we are in Observing state
        post_event(detail::InitializeObserving());

        // Transition to Observe state
        return transit<Observing>();
    }

    sc::result Selling::react(const event::BuyModeStarted & e) {

        if(!_initialized)
            throw std::runtime_error("Selling state not initialized.");

        std::cout << "Reacting to event::BuyModeStarted." << std::endl;

        // Trigger initialization event for when we are in Selling state
        post_event(detail::InitializeBuying(e.terms()));

        // Transition to Buy state
        return transit<Buying>();
    }

    sc::result Selling::react(const event::UpdateTerms<SellerTerms> & e) {

        if(!_initialized)
            throw std::runtime_error("Selling state not initialized.");

        std::cout << "Reacting to event::UpdateTerms<SellerTerms>." << std::endl;

        // Increment term index
        _index++;

        // Update payee based on terms
        updatePayeeTerms(e.terms());

        // Create sell message and send
        context<CBStateMachine>().sendMessage()(new wire::Sell(e.terms(), _index));

        // Transition back to initial selling state
        return transit<Selling>();
    }

    uint32_t Selling::index() const {
        return _index;
    }

    void Selling::updatePayeeTerms(const SellerTerms & t) {
        _payee.setLockTime(t.minLock());
        _payee.setPrice(t.minPrice());
        _payee.setSettlementFee(t.settlementFee());
    }

}
}
}
