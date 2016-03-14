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

    CBStateMachine::CBStateMachine(const InvitedToOutdatedContract & invitedToOutdatedContract,
                                   const InvitedToJoinContract & invitedToJoinContract,
                                   const Send & sendMessage,
                                   const ContractIsReady & contractIsReady,
                                   const PieceRequested & pieceRequested,
                                   const PeerModeAnnounced & peerAnnouncedMode)
        : _peerAnnouncedMode(peerAnnouncedMode)
        , _invitedToOutdatedContract(invitedToOutdatedContract)
        , _invitedToJoinContract(invitedToJoinContract)
        , _sendMessage(sendMessage)
        , _contractIsReady(contractIsReady)
        , _pieceRequested(pieceRequested) {
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

    joystream::protocol::PeerModeAnnounced CBStateMachine::peerAnnouncedMode() const {
        return _peerAnnouncedMode;
    }
}
}
}
