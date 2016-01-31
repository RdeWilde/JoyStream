/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 30 2016
 */

#include <extension/SellerPeerPluginPeerState.hpp>

namespace joystream {
namespace extension {

    SellerPeerPluginPeerState::SellerPeerPluginPeerState()
        : //_lastAction(LastValidAction::no_bitswapr_message_sent)
        _failureMode(FailureMode::not_failed) {
    }

    SellerPeerPluginPeerState::SellerPeerPluginPeerState(//LastValidAction lastAction,
                                           FailureMode failureMode,
                                           const joystream::protocol::Buy & lastBuyReceived,
                                           const joystream::protocol::SignRefund & lastSignRefundReceived,
                                           const joystream::protocol::Payment & lastPaymentReceived,
                                           const joystream::protocol::RequestFullPiece & lastRequestFullPieceReceived)
        : //_lastAction(lastAction)
          _failureMode(failureMode)
        , _lastBuyReceived(lastBuyReceived)
        , _lastSignRefundReceived(lastSignRefundReceived)
        , _lastPaymentReceived(lastPaymentReceived)
        , _lastRequestFullPieceReceived(lastRequestFullPieceReceived){
    }

    /**
    SellerPeerPlugin::PeerState::LastValidAction SellerPeerPlugin::PeerState::lastAction() const {
        return _lastAction;
    }

    void SellerPeerPlugin::PeerState::setLastAction(LastValidAction lastAction) {
        _lastAction = lastAction;
    }
    */

    SellerPeerPluginPeerState::FailureMode SellerPeerPluginPeerState::failureMode() const {
        return _failureMode;
    }

    void SellerPeerPluginPeerState::setFailureMode(FailureMode failureMode) {
        _failureMode = failureMode;
    }

    joystream::protocol::Payment SellerPeerPluginPeerState::lastPaymentReceived() const {
        return _lastPaymentReceived;
    }

    void SellerPeerPluginPeerState::setLastPaymentReceived(const joystream::protocol::Payment & lastPaymentReceived) {
        _lastPaymentReceived = lastPaymentReceived;
    }

    joystream::protocol::SignRefund SellerPeerPluginPeerState::lastSignRefundReceived() const {
        return _lastSignRefundReceived;
    }

    void SellerPeerPluginPeerState::setLastSignRefundReceived(const joystream::protocol::SignRefund & lastSignRefundReceived) {
        _lastSignRefundReceived = lastSignRefundReceived;
    }

    joystream::protocol::Buy SellerPeerPluginPeerState::lastBuyReceived() const {
        return _lastBuyReceived;
    }

    void SellerPeerPluginPeerState::setLastBuyReceived(const joystream::protocol::Buy & lastBuyReceived){
        _lastBuyReceived = lastBuyReceived;
    }

    joystream::protocol::RequestFullPiece SellerPeerPluginPeerState::lastRequestFullPieceReceived() const {
        return _lastRequestFullPieceReceived;
    }

    void SellerPeerPluginPeerState::setLastRequestFullPieceReceived(const joystream::protocol::RequestFullPiece & lastRequestFullPieceReceived) {
        _lastRequestFullPieceReceived = lastRequestFullPieceReceived;
    }

}
}
