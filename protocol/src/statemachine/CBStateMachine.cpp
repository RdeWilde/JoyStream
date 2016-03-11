/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#include <protocol/statemachine/CBStateMachine.hpp>
#include <protocol/SellerTerms.hpp>

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
    }

    void CBStateMachine::clientToSellMode(const joystream::protocol::SellerTerms & terms) {
        // send sell mode message
    }

}
}
}
