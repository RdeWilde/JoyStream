/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#include <protocol/statemachine/CBStateMachine.hpp>
#include <protocol/statemachine/exception/StateIncompatibleEvent.hpp>


namespace joystream {
namespace protocol {
namespace statemachine {

    CBStateMachine::CBStateMachine(const InvitedToOutdatedContract & invitedToOutdatedContract,
                                   const InvitedToJoinContract & invitedToJoinContract,
                                   const Send & sendMessage,
                                   const ContractIsReady & contractIsReady,
                                   const PieceRequested & pieceRequested,
                                   const PeerInterruptedPayment & peerInterruptedPayment,
                                   const ValidPayment & validPayment,
                                   const InvalidPayment & invalidPayment)
        : _invitedToOutdatedContract(invitedToOutdatedContract)
        , _invitedToJoinContract(invitedToJoinContract)
        , _sendMessage(sendMessage)
        , _contractIsReady(contractIsReady)
        , _pieceRequested(pieceRequested)
        , _peerInterruptedPayment(peerInterruptedPayment)
        , _validPayment(validPayment)
        , _invalidPayment(invalidPayment) {
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

    void CBStateMachine::peerToSellMode(const joystream::protocol::SellerTerms & t, uint32_t index) {
        _peerAnnouncedMode.toSell(t, index);
    }

    void CBStateMachine::peerToBuyMode(const joystream::protocol::BuyerTerms & t) {
        _peerAnnouncedMode.toBuy(t);
    }

    CBStateMachine::InvitedToOutdatedContract CBStateMachine::invitedToOutdatedContract() const {
        return _invitedToOutdatedContract;
    }

    CBStateMachine::InvitedToJoinContract CBStateMachine::invitedToJoinContract() const {
        return _invitedToJoinContract;
    }

    CBStateMachine::Send CBStateMachine::sendMessage() const {
        return _sendMessage;
    }

    CBStateMachine::ContractIsReady CBStateMachine::contractIsReady() const {
        return _contractIsReady;
    }

    CBStateMachine::PieceRequested CBStateMachine::pieceRequested() const {
        return _pieceRequested;
    }

    CBStateMachine::PeerInterruptedPayment CBStateMachine::getPeerInterruptedPayment() const {
        return _peerInterruptedPayment;
    }

    CBStateMachine::ValidPayment CBStateMachine::getValidPayment() const {
        return _validPayment;
    }

    CBStateMachine::InvalidPayment CBStateMachine::getInvalidPayment() const {
        return _invalidPayment;
    }

    joystream::protocol::PeerModeAnnounced CBStateMachine::peerAnnouncedMode() const {
        return _peerAnnouncedMode;
    }
}
}
}
