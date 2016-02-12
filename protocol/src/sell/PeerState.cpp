/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 11 2016
 */

#include <protocol/sell/PeerState.hpp>

namespace joystream {
namespace protocol {
namespace sell {

    PeerState::PeerState()
        : //_lastAction(LastValidAction::no_bitswapr_message_sent)
        _failureMode(FailureMode::not_failed) {
    }

    PeerState::PeerState(//LastValidAction lastAction,
                                           FailureMode failureMode,
                                           const joystream::protocol::wire::SignRefund & lastSignRefundReceived,
                                           const joystream::protocol::wire::Payment & lastPaymentReceived,
                                           const joystream::protocol::wire::RequestFullPiece & lastRequestFullPieceReceived)
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

    PeerState::FailureMode PeerState::failureMode() const {
        return _failureMode;
    }

    void PeerState::setFailureMode(FailureMode failureMode) {
        _failureMode = failureMode;
    }

    joystream::protocol::wire::Payment PeerState::lastPaymentReceived() const {
        return _lastPaymentReceived;
    }

    void PeerState::setLastPaymentReceived(const joystream::protocol::wire::Payment & lastPaymentReceived) {
        _lastPaymentReceived = lastPaymentReceived;
    }

    joystream::protocol::wire::SignRefund PeerState::lastSignRefundReceived() const {
        return _lastSignRefundReceived;
    }

    void PeerState::setLastSignRefundReceived(const joystream::protocol::wire::SignRefund & lastSignRefundReceived) {
        _lastSignRefundReceived = lastSignRefundReceived;
    }

    joystream::protocol::wire::RequestFullPiece PeerState::lastRequestFullPieceReceived() const {
        return _lastRequestFullPieceReceived;
    }

    void PeerState::setLastRequestFullPieceReceived(const joystream::protocol::wire::RequestFullPiece & lastRequestFullPieceReceived) {
        _lastRequestFullPieceReceived = lastRequestFullPieceReceived;
    }

}
}
}
