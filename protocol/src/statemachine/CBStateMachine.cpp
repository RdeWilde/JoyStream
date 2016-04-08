/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#include <protocol/statemachine/CBStateMachine.hpp>
#include <protocol/statemachine/Observing.hpp>
#include <protocol/statemachine/Buying.hpp>
#include <protocol/statemachine/Selling.hpp>
#include <protocol/statemachine/exception/StateIncompatibleEvent.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    CBStateMachine::CBStateMachine(const InvitedToOutdatedContract & invitedToOutdatedContract,
                                   const InvitedToJoinContract & invitedToJoinContract,
                                   const Send & sendMessage,
                                   const ContractIsReady & contractIsReady,
                                   const PieceRequested & pieceRequested,
                                   const InvalidPieceRequested & invalidPieceRequested,
                                   const PeerInterruptedPayment & peerInterruptedPayment,
                                   const ValidPayment & validPayment,
                                   const InvalidPayment & invalidPayment,
                                   const SellerJoined & sellerJoined,
                                   const SellerInterruptedContract & sellerInterruptedContract,
                                   const ReceivedFullPiece & receivedFullPiece,
                                   int MAX_PIECE_INDEX)
        : _invitedToOutdatedContract(invitedToOutdatedContract)
        , _invitedToJoinContract(invitedToJoinContract)
        , _sendMessage(sendMessage)
        , _contractIsReady(contractIsReady)
        , _pieceRequested(pieceRequested)
        , _invalidPieceRequested(invalidPieceRequested)
        , _peerInterruptedPayment(peerInterruptedPayment)
        , _validPayment(validPayment)
        , _invalidPayment(invalidPayment)
        , _sellerJoined(sellerJoined)
        , _sellerInterruptedContract(sellerInterruptedContract)
        , _receivedFullPiece(receivedFullPiece)
        , _MAX_PIECE_INDEX(MAX_PIECE_INDEX) {
    }

    void CBStateMachine::unconsumed_event(const sc::event_base & e) {
        throw exception::StateIncompatibleEvent();
    }

    /**
    const char * CBStateMachine::getInnerStateName() const {

        // We assume there is only one type which is active in any
        // in the state machine.
        int n = 0;
        for (CBStateMachine::state_iterator i = this->state_begin(); i != this->state_end(); ++i, ++n);

        assert(n == 1);

        // Get type info
        // The following use of typeid assumes that
        // BOOST_STATECHART_USE_NATIVE_RTTI is defined
        return typeid(*(this->state_begin())).name();
    }
    */

    void CBStateMachine::peerToObserveMode() {
        _peerAnnouncedMode.toObserve();
    }

    void CBStateMachine::peerToSellMode(const joystream::wire::SellerTerms & t, uint32_t index) {

        _peerAnnouncedMode.toSell(t, index);

        // Update payor: even though machine need not be in Buying state!
        _payor.setPrice(t.minPrice());
        _payor.setRefundLockTime(t.minLock());
        _payor.setSettlementFee(t.settlementFee());
    }

    void CBStateMachine::peerToBuyMode(const joystream::wire::BuyerTerms & t) {

        _peerAnnouncedMode.toBuy(t);

        // Update payee: even though machine need not be in Selling state!
        _payee.setRefundFee(t.refundFee());
    }

    void CBStateMachine::updateAndAnnounceClientMode() {
        _sendMessage(new wire::Observe());
    }

    void CBStateMachine::updateAndAnnounceClientMode(const joystream::wire::SellerTerms & t, uint32_t index) {

        _index = index;

        // Update payee based on client terms
        updatePayeeTerms(t);

        // Send mode message
        _sendMessage(new wire::Sell(t, _index));
    }

    void CBStateMachine::updateAndAnnounceClientMode(const joystream::wire::BuyerTerms & t) {

        // Update payor based on client terms
        updatePayorTerms(t);

        // Send mode message
        _sendMessage(new wire::Buy(t));
    }

    void CBStateMachine::updatePayeeTerms(const joystream::wire::SellerTerms & t) {
        _payee.setLockTime(t.minLock());
        _payee.setPrice(t.minPrice());
        _payee.setSettlementFee(t.settlementFee());
    }

    void CBStateMachine::updatePayorTerms(const joystream::wire::BuyerTerms & t) {
        _payor.setRefundFee(t.refundFee());
    }

    joystream::protocol::PeerModeAnnounced CBStateMachine::peerAnnouncedMode() const {
        return _peerAnnouncedMode;
    }
}
}
}
