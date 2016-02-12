/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 11 2016
 */

#include <protocol/SellerPeerState.hpp>

namespace joystream {
namespace protocol {

    SellerPeerState::SellerPeerState()
        : //_lastAction(LastValidAction::no_bitswapr_message_sent)
        _failureMode(FailureMode::not_failed) {
    }

    SellerPeerState::SellerPeerState(//LastValidAction lastAction,
                                           FailureMode failureMode,
                                           const wire::SignRefund & lastSignRefundReceived,
                                           const wire::Payment & lastPaymentReceived,
                                           const wire::RequestFullPiece & lastRequestFullPieceReceived)
        : //_lastAction(lastAction)
          _failureMode(failureMode)
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

    SellerPeerState::FailureMode SellerPeerState::failureMode() const {
        return _failureMode;
    }

    void SellerPeerState::setFailureMode(FailureMode failureMode) {
        _failureMode = failureMode;
    }

    wire::Payment SellerPeerState::lastPaymentReceived() const {
        return _lastPaymentReceived;
    }

    void SellerPeerState::setLastPaymentReceived(const wire::Payment & lastPaymentReceived) {
        _lastPaymentReceived = lastPaymentReceived;
    }

    wire::SignRefund SellerPeerState::lastSignRefundReceived() const {
        return _lastSignRefundReceived;
    }

    void SellerPeerState::setLastSignRefundReceived(const wire::SignRefund & lastSignRefundReceived) {
        _lastSignRefundReceived = lastSignRefundReceived;
    }

    joystream::protocol::wire::RequestFullPiece SellerPeerState::lastRequestFullPieceReceived() const {
        return _lastRequestFullPieceReceived;
    }

    void SellerPeerState::setLastRequestFullPieceReceived(const wire::RequestFullPiece & lastRequestFullPieceReceived) {
        _lastRequestFullPieceReceived = lastRequestFullPieceReceived;
    }

}
}
