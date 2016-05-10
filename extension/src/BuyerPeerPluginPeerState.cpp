/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 1 2016
 */

#include <extension/BuyerPeerPluginPeerState.hpp>

namespace joystream {
namespace extension {

    BuyerPeerPluginPeerState::BuyerPeerPluginPeerState()
        : //_lastAction(LastValidAction::no_bitswapr_message_sent)
         _failureMode(FailureMode::not_failed) {
    }

    BuyerPeerPluginPeerState::BuyerPeerPluginPeerState(//LastValidAction lastAction,
                                            FailureMode failureMode,
                                            const joystream::protocol::Sell & lastSellReceived,
                                            const joystream::protocol::JoiningContract & lastJoiningContractReceived)
        : //_lastAction(lastAction)
          _failureMode(failureMode)
        , _lastSellReceived(lastSellReceived)
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

    BuyerPeerPluginPeerState::FailureMode BuyerPeerPluginPeerState::failureMode() const {
        return _failureMode;
    }

    void BuyerPeerPluginPeerState::setFailureMode(FailureMode failureMode) {
        _failureMode = failureMode;
    }

    joystream::protocol::JoiningContract BuyerPeerPluginPeerState::lastJoiningContractReceived() const {
        return _lastJoiningContractReceived;
    }

    void BuyerPeerPluginPeerState::setLastJoiningContractReceived(const joystream::protocol::JoiningContract & lastJoiningContractReceived) {
        _lastJoiningContractReceived = lastJoiningContractReceived;
    }

    joystream::protocol::Sell BuyerPeerPluginPeerState::lastSellReceived() const {
        return _lastSellReceived;
    }

    void BuyerPeerPluginPeerState::setLastSellReceived(const joystream::protocol::Sell & lastSellReceived) {
        _lastSellReceived = lastSellReceived;
    }
}
}
