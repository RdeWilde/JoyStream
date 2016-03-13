/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#include <protocol/statemachine/CBStateMachine.hpp>
#include <protocol/SellerTerms.hpp>

#include <iostream>

namespace joystream {
namespace protocol {
namespace statemachine {

    CBStateMachine::CBStateMachine(const PeerModeAnnounced & peerAnnouncedMode)
        : _peerAnnouncedMode(peerAnnouncedMode) {
    }

    joystream::protocol::PeerModeAnnounced CBStateMachine::peerAnnouncedMode() const {
        return _peerAnnouncedMode;
    }

    void CBStateMachine::setPeerAnnouncedMode(const joystream::protocol::PeerModeAnnounced & peerAnnouncedMode) {
        _peerAnnouncedMode = peerAnnouncedMode;
    }

    void CBStateMachine::clientToObserveMode() {
        // send observe mode message
        std::cout << "Sending observe message." << std::endl;
    }

    void CBStateMachine::clientToSellMode(const joystream::protocol::SellerTerms & t) {
        // send sell mode message
        std::cout << "Sending sell message with terms: ." << std::endl;
    }

    void CBStateMachine::clientToBuyMode(const joystream::protocol::BuyerTerms & t) {
        // send buy mode message
        std::cout << "Sending buy message with terms: ." << std::endl;
    }

    void CBStateMachine::peerToObserveMode() {
        _peerAnnouncedMode.toObserve();
    }

    void CBStateMachine::peerToSellMode(const joystream::protocol::SellerTerms & t) {
        _peerAnnouncedMode.toSellMode(t);
    }

    void CBStateMachine::peerToBuyMode(const joystream::protocol::BuyerTerms & t) {
        _peerAnnouncedMode.toBuyMode(t);
    }

}
}
}
