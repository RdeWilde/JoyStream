/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 28 2016
 */

#include <protocol/statemachine/SellerHasJoined.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    SellerHasJoined::SellerHasJoined() {
        std::cout << "Entering SellerJoined state." << std::endl;
    }

    sc::result SellerHasJoined::react(const event::Recv<wire::Observe> & e) {

        std::cout << "Reacting to Recv<wire::Observe> event." << std::endl;

        // Send client notification about this interruption,
        context<CBStateMachine>().sellerInterruptedContract()();

        // and update new peer mode
        context<CBStateMachine>().peerToObserveMode();

        // Go to initial buying state
        return transit<Buying>();
    }

    sc::result SellerHasJoined::react(const event::Recv<wire::Buy> & e) {

        std::cout << "Reacting to Recv<wire::Buy> event." << std::endl;

        // Send client notification about this interruption,
        context<CBStateMachine>().peerInterruptedPayment()();

        // and update new peer mode
        context<CBStateMachine>().peerToBuyMode(e.message()->terms());

        // Go to initial buying state
        return transit<Buying>();
    }

    sc::result SellerHasJoined::react(const event::Recv<wire::Sell> & e) {

        std::cout << "Reacting to Recv<wire::Sell> event." << std::endl;

        // Send client notification about this interruption,
        context<CBStateMachine>().peerInterruptedPayment()();

        // and update new peer mode
        wire::Sell const * m = e.message();
        context<CBStateMachine>().peerToSellMode(m->terms(), m->index());

        // Go to initial buying state
        return transit<Buying>();
    }

}
}
}
