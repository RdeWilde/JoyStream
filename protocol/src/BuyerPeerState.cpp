/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 12 2016
 */

#include <protocol/BuyerPeerState.hpp>

namespace joystream {
namespace protocol {

    BuyerPeerState::BuyerPeerState()
        : //_lastAction(LastValidAction::no_bitswapr_message_sent)
         _failureMode(FailureMode::not_failed) {
    }

    BuyerPeerState::BuyerPeerState(//LastValidAction lastAction,
                                            FailureMode failureMode,
                                            const wire::JoiningContract & lastJoiningContractReceived)
        : //_lastAction(lastAction)
          _failureMode(failureMode)
        , _lastJoiningContractReceived(lastJoiningContractReceived) {
    }

    /*
    BuyerPeerPlugin::PeerState::LastValidAction BuyerPeerPlugin::PeerState::lastAction() const {
        return _lastAction;
    }
    void BuyerPeerPlugin::PeerState::setLastAction(LastValidAction lastAction) {
        _lastAction = lastAction;
    }
    */

    BuyerPeerState::FailureMode BuyerPeerState::failureMode() const {
        return _failureMode;
    }

    void BuyerPeerState::setFailureMode(FailureMode failureMode) {
        _failureMode = failureMode;
    }

    wire::JoiningContract BuyerPeerState::lastJoiningContractReceived() const {
        return _lastJoiningContractReceived;
    }

    void BuyerPeerState::setLastJoiningContractReceived(const wire::JoiningContract & lastJoiningContractReceived) {
        _lastJoiningContractReceived = lastJoiningContractReceived;
    }

}
}
