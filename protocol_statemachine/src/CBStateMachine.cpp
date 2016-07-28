/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#include <protocol_statemachine/CBStateMachine.hpp>
#include <protocol_statemachine/Observing.hpp>
#include <protocol_statemachine/Buying.hpp>
#include <protocol_statemachine/Selling.hpp>
#include <protocol_statemachine/exception/exception.hpp>

namespace joystream {
namespace protocol_statemachine {

    const common::MajorMinorSoftwareVersion CBStateMachine::protocolVersion(MAJOR_PROTOCOL_VERSION, MINOR_PROTOCOL_VERSION);

    CBStateMachine::CBStateMachine(const PeerAnnouncedMode & peerAnnouncedMode,
                                   const InvitedToOutdatedContract & invitedToOutdatedContract,
                                   const InvitedToJoinContract & invitedToJoinContract,
                                   const Send & send,
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
        : _currentlyProcessingCallbacks(false)
        , _peerAnnouncedMode(_queuedCallbacks, peerAnnouncedMode)
        , _invitedToOutdatedContract(_queuedCallbacks, invitedToOutdatedContract)
        , _invitedToJoinContract(_queuedCallbacks, invitedToJoinContract)
        , _sendMessage(_queuedCallbacks, send)
        , _contractIsReady(_queuedCallbacks, contractIsReady)
        , _pieceRequested(_queuedCallbacks, pieceRequested)
        , _invalidPieceRequested(_queuedCallbacks, invalidPieceRequested)
        , _peerInterruptedPayment(_queuedCallbacks, peerInterruptedPayment)
        , _validPayment(_queuedCallbacks, validPayment)
        , _invalidPayment(_queuedCallbacks, invalidPayment)
        , _sellerJoined(_queuedCallbacks, sellerJoined)
        , _sellerInterruptedContract(_queuedCallbacks, sellerInterruptedContract)
        , _receivedFullPiece(_queuedCallbacks, receivedFullPiece)
        , _MAX_PIECE_INDEX(MAX_PIECE_INDEX)
        , _lastRequestedPiece(0) {
    }

    void CBStateMachine::processEvent(const sc::event_base & e) {

        // Have state machine process event
        state_machine::process_event(e);

        // If this event was posted from a callback,
        // then we do all queued callback processing in that
        if(_currentlyProcessingCallbacks)
            return;

        //// Process queued callbacks

        // Mark us as processing callbacks
        _currentlyProcessingCallbacks = true;

        // ::empty() must be used, rather than iterator,as
        // queue may mutate while iterating, due to
        // events posted in the callbacks initiated here.
        while(!_queuedCallbacks.empty()) {

            // Get call
            NoPayloadNotification f = _queuedCallbacks.front();

            // Remove
            _queuedCallbacks.pop_front();

            // Initiate
            try {
                f();
            } catch (const exception::StateMachineDeletedException &) {
                // If we come here, then we just exit, as
                // this statemachine has been deleted in callback,
                // Hence no part of state machine object should be used.
                // Note on deleting this: https://isocpp.org/wiki/faq/freestore-mgmt#delete-this
                return;
            }
        }

        // Mark as done processing callacks: we don't do this if
        // exception is thrown, as *(this) is dead.
        _currentlyProcessingCallbacks = false;
    }

    void CBStateMachine::unconsumed_event(const sc::event_base &) {
        std::cout << "unconsumed_event" << std::endl;
        //throw exception::StateIncompatibleEvent("");
    }

    void CBStateMachine::process_event(const sc::event_base &) {
        assert(false);
    }

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

    void CBStateMachine::peerToObserveMode() {
        _announcedModeAndTermsFromPeer.toObserve();
    }

    void CBStateMachine::peerToSellMode(const joystream::protocol_wire::SellerTerms & t, uint32_t index) {

        _announcedModeAndTermsFromPeer.toSell(t, index);

        // Update payor: even though machine need not be in Buying state!
        _payor.setPrice(t.minPrice());
        _payor.setRefundLockTime(t.minLock());
        _payor.setSettlementFee(t.settlementFee());
    }

    void CBStateMachine::peerToBuyMode(const joystream::protocol_wire::BuyerTerms & t) {

        _announcedModeAndTermsFromPeer.toBuy(t);

        // Update payee: even though machine need not be in Selling state!
        _payee.setRefundFee(t.refundFee());
    }

    void CBStateMachine::clientToObserveMode() {
        _sendMessage(new protocol_wire::Observe());
    }

    void CBStateMachine::clientToSellMode(const protocol_wire::SellerTerms & t, uint32_t index) {

        _index = index;

        // Update payee based on client terms
        _payee.setLockTime(t.minLock());
        _payee.setPrice(t.minPrice());
        _payee.setSettlementFee(t.settlementFee());

        // Send mode message
        _sendMessage(new protocol_wire::Sell(t, _index));
    }

    void CBStateMachine::clientToBuyMode(const protocol_wire::BuyerTerms & t) {

        // Update payor based on client terms
        _payor.setRefundFee(t.refundFee());

        // Send mode message
        _sendMessage(new protocol_wire::Buy(t));
    }

    void CBStateMachine::peerAnnouncedMode() {
        _peerAnnouncedMode(_announcedModeAndTermsFromPeer);
    }

    int CBStateMachine::lastRequestedPiece() const {
        return _lastRequestedPiece;
    }

    paymentchannel::Payee CBStateMachine::payee() const {
        return _payee;
    }

    paymentchannel::Payor CBStateMachine::payor() const {
        return _payor;
    }

    ModeAnnounced CBStateMachine::clientMode() const {

        if(this->inState<Buying>())
            return ModeAnnounced::buy;
        else if(this->inState<Selling>())
            return ModeAnnounced::sell;
        else if(this->inState<Observing>())
            return ModeAnnounced::observe;
        else
            throw ModeAnnounced::none;
    }

    protocol_statemachine::AnnouncedModeAndTerms CBStateMachine::announcedModeAndTermsFromPeer() const {
        return _announcedModeAndTermsFromPeer;
    }

    int CBStateMachine::MAX_PIECE_INDEX() const {
        return _MAX_PIECE_INDEX;
    }

    void CBStateMachine::setMAX_PIECE_INDEX(int MAX_PIECE_INDEX) {
        _MAX_PIECE_INDEX = MAX_PIECE_INDEX;
    }
}
}
