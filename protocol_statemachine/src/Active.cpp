/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 13 2016
 */

#include <protocol_statemachine/Active.hpp>

#include <boost/statechart/deep_history.hpp>
#include <iostream>

namespace joystream {
namespace protocol_statemachine {

    Active::Active() {
        std::cout << "Entering Active state." << std::endl;
    }

    sc::result Active::react(const event::Recv<protocol_wire::Observe> & e) {

        std::cout << "Reacting to Recv<protocol_wire::Observe> event." << std::endl;

        // Switch peer state
        context<CBStateMachine>().peerToObserveMode();

        // Notify client about announcement
        context<CBStateMachine>().peerAnnouncedMode();

        // Transition to deep history
        return transit<sc::deep_history<Observing>>();
    }

    sc::result Active::react(const event::Recv<protocol_wire::Buy> & e) {

        std::cout << "Reacting to Recv<protocol_wire::Buy> event." << std::endl;

        // Switch peer state
        context<CBStateMachine>().peerToBuyMode(e.message()->terms());

        // Notify client about announcement
        context<CBStateMachine>().peerAnnouncedMode();

        // Transition to deep history
        return transit<sc::deep_history<Observing>>();
    }

    sc::result Active::react(const event::Recv<protocol_wire::Sell> & e) {

        std::cout << "Reacting to Recv<protocol_wire::Sell> event." << std::endl;

        // Switch peer state
        protocol_wire::Sell const * m = e.message();
        context<CBStateMachine>().peerToSellMode(m->terms(), m->index());

        // Notify client about announcement
        context<CBStateMachine>().peerAnnouncedMode();

        // Transition to deep history
        return transit<sc::deep_history<Observing>>();
    }

}
}
