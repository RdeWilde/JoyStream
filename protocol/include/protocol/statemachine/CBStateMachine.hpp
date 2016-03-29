/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP

#include <protocol/PeerModeAnnounced.hpp>
#include <protocol/statemachine/event/Recv.hpp>

#include <boost/statechart/state_machine.hpp>

namespace sc = boost::statechart;

namespace Coin {
    class typesafeOutPoint;
    class PublicKey;
    class Signature;
}

namespace joystream {
namespace protocol {

    namespace wire {
        class ExtendedMessagePayload;
    }

    class ContractInvitation;
    class ContractRSVP;
    class PieceData;

    namespace statemachine {

        class ChooseMode; // Default state

        // Friend classes that need to modify CBStateMachine::_peerAnnouncedMode
        class Active;
        class ServicingPieceRequest;
        class SellerHasJoined;
        class WaitingForFullPiece;

        class CBStateMachine : public sc::state_machine<CBStateMachine, ChooseMode> {

        public:

            //// General Notifications

            // Client requires a message to be sent
            typedef std::function<void(const wire::ExtendedMessagePayload *)> Send;

            //// Selling Notifications

            // Client was invited to expired contract, as indicated by bad index
            typedef std::function<void()> InvitedToOutdatedContract;

            // Client was invited to join given contract, should terms be included? they are available in _peerAnnounced
            typedef std::function<void(const ContractInvitation)> InvitedToJoinContract;

            // Peer announced that contract is now ready, should contract be be included? it was available
            typedef std::function<void(const Coin::typesafeOutPoint &)> ContractIsReady;

            // Peer requested piece
            typedef std::function<void(int)> PieceRequested;

            // Peer sent mode message when payment was expected/required
            typedef std::function<void()> PeerInterruptedPayment;

            // Peer sent valid payment signature
            typedef std::function<void(const Coin::Signature &)> ValidPayment;

            // Peer sent an invalid payment signature
            typedef std::function<void(const Coin::Signature &)> InvalidPayment;

            //// Buying Notifications

            // Peer, in seller mode, joined the most recent invitation
            typedef std::function<void()> SellerJoined;

            // Peer, in seller mode, left - by sending new mode message (which may also be sell) - after requesting a piece
            typedef std::function<void()> SellerInterruptedContract;

            // Peer, in seller mode, responded with full piece
            typedef std::function<void(const PieceData &)> ReceivedFullPiece;

            CBStateMachine(const InvitedToOutdatedContract &,
                           const InvitedToJoinContract &,
                           const Send &,
                           const ContractIsReady &,
                           const PieceRequested &,
                           const PeerInterruptedPayment &,
                           const ValidPayment &,
                           const InvalidPayment &,
                           const SellerJoined &,
                           const SellerInterruptedContract &,
                           const ReceivedFullPiece &);

            void unconsumed_event(const sc::event_base &);

            // Deprecated: internals should not be directly visible for client or tester
            // Get name of current state: ***Varies from compiler to compiler***
            //const char * getInnerStateName() const;

            // Getters and setters
            InvitedToOutdatedContract invitedToOutdatedContract() const;

            InvitedToJoinContract invitedToJoinContract() const;

            Send sendMessage() const;

            ContractIsReady contractIsReady() const;

            PieceRequested pieceRequested() const;

            PeerInterruptedPayment getPeerInterruptedPayment() const;

            ValidPayment getValidPayment() const;

            InvalidPayment getInvalidPayment() const;

            SellerJoined getSellerJoined() const;

            SellerInterruptedContract getSellerInterruptedContract() const;

            ReceivedFullPiece getReceivedFullPiece() const;

            PeerModeAnnounced peerAnnouncedMode() const;

        private:

            friend class Active;
            friend class ServicingPieceRequest;
            friend class SellerHasJoined;
            friend class WaitingForFullPiece; //

            // Context actions
            void peerToObserveMode();
            void peerToSellMode(const SellerTerms &, uint32_t);
            void peerToBuyMode(const BuyerTerms &);

            // Callbacks for classifier routines
            InvitedToOutdatedContract _invitedToOutdatedContract;
            InvitedToJoinContract _invitedToJoinContract;
            Send _sendMessage;
            ContractIsReady _contractIsReady;
            PieceRequested _pieceRequested;
            PeerInterruptedPayment _peerInterruptedPayment;
            ValidPayment _validPayment;
            InvalidPayment _invalidPayment;
            SellerJoined _sellerJoined;
            SellerInterruptedContract _sellerInterruptedContract;
            ReceivedFullPiece _receivedFullPiece;

            //// Peer state
            //*** Just factor out modeannounced and index, dont save actual terms? ***
            joystream::protocol::PeerModeAnnounced _peerAnnouncedMode;
        };
    }
}
}

// Required to make CBStateMachine complete when included throught his header file,
// as ChooseMode is initial state and thus part of parent state_machine definition
#include <protocol/statemachine/ChooseMode.hpp>

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP
