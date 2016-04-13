/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 28 2016
 */

#include <protocol_statemachine/SellerHasJoined.hpp>

namespace joystream {
namespace protocol_statemachine {

    SellerHasJoined::SellerHasJoined() {
        std::cout << "Entering SellerJoined state." << std::endl;
    }

    sc::result SellerHasJoined::react(const event::Recv<protocol_wire::Observe> &) {

        std::cout << "Reacting to Recv<protocol_wire::Observe> event." << std::endl;

        // Send client notification about this interruption,
        context<CBStateMachine>()._sellerInterruptedContract();

        // and update new peer mode
        context<CBStateMachine>().peerToObserveMode();

        // Go to initial buying state
        return transit<Buying>();
    }

    sc::result SellerHasJoined::react(const event::Recv<protocol_wire::Buy> & e) {

        std::cout << "Reacting to Recv<protocol_wire::Buy> event." << std::endl;

        // Send client notification about this interruption,
        context<CBStateMachine>()._sellerInterruptedContract();

        // and update new peer mode
        context<CBStateMachine>().peerToBuyMode(e.message().terms());

        // Go to initial buying state
        return transit<Buying>();
    }

    sc::result SellerHasJoined::react(const event::Recv<protocol_wire::Sell> & e) {

        std::cout << "Reacting to Recv<protocol_wire::Sell> event." << std::endl;

        // Send client notification about this interruption,
        context<CBStateMachine>()._sellerInterruptedContract();

        // and update new peer mode
        context<CBStateMachine>().peerToSellMode(e.message().terms(), e.message().index());

        // Go to initial buying state
        return transit<Buying>();
    }

}
}
