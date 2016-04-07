/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 13 2016
 */

#include <protocol/statemachine/Active.hpp>

#include <boost/statechart/deep_history.hpp>
#include <iostream>

namespace joystream {
namespace protocol {
namespace statemachine {

    Active::Active() {
        std::cout << "Entering Active state." << std::endl;
    }

    sc::result Active::react(const event::Recv<wire::Observe> & e) {

        std::cout << "Reacting to Recv<wire::Observe> event." << std::endl;

        // Switch peer state
        context<CBStateMachine>().peerToObserveMode();

        // Transition to deep history
        return transit<sc::deep_history<Observing>>();
    }

    sc::result Active::react(const event::Recv<wire::Buy> & e) {

        std::cout << "Reacting to Recv<wire::Buy> event." << std::endl;

        // Switch peer state
        context<CBStateMachine>().peerToBuyMode(e.message()->terms());

        // Transition to deep history
        return transit<sc::deep_history<Observing>>();
    }

    sc::result Active::react(const event::Recv<wire::Sell> & e) {

        std::cout << "Reacting to Recv<wire::Sell> event." << std::endl;

        // Switch peer state
        wire::Sell const * m = e.message();
        context<CBStateMachine>().peerToSellMode(m->terms(), m->index());

        // Transition to deep history
        return transit<sc::deep_history<Observing>>();
    }

}
}
}
