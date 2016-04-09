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
#include <paymentchannel/Payor.hpp>
#include <paymentchannel/Payee.hpp>

#include <boost/statechart/state_machine.hpp>

namespace sc = boost::statechart;

namespace Coin {
    class typesafeOutPoint;
    class PublicKey;
    class Signature;
}

namespace joystream {
namespace wire {
    class ExtendedMessagePayload;
    class SellerTerms;
    class BuyerTerms;
    class ContractInvitation;
    class ContractRSVP;
    class PieceData;
}

namespace protocol {
namespace statemachine {

    class ChooseMode; // Default state

    class CBStateMachine : public sc::state_machine<CBStateMachine, ChooseMode> {

    public:

        //// General Notifications

        // Client requires a message to be sent
        typedef std::function<void(const wire::ExtendedMessagePayload *)> Send;

        //// Selling Notifications

        // Client was invited to expired contract, as indicated by bad index
        typedef std::function<void()> InvitedToOutdatedContract;

        // Client was invited to join given contract, should terms be included? they are available in _peerAnnounced
        typedef std::function<void(const joystream::wire::ContractInvitation)> InvitedToJoinContract;

        // Peer announced that contract is now ready, should contract be be included? it was available
        typedef std::function<void(const Coin::typesafeOutPoint &)> ContractIsReady;

        // Peer requested piece
        typedef std::function<void(int)> PieceRequested;

        // Peer invalid piece requested
        typedef std::function<void()> InvalidPieceRequested;

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
        typedef std::function<void(const joystream::wire::PieceData &)> ReceivedFullPiece;

        CBStateMachine(const InvitedToOutdatedContract &,
                       const InvitedToJoinContract &,
                       const Send &,
                       const ContractIsReady &,
                       const PieceRequested &,
                       const InvalidPieceRequested &,
                       const PeerInterruptedPayment &,
                       const ValidPayment &,
                       const InvalidPayment &,
                       const SellerJoined &,
                       const SellerInterruptedContract &,
                       const ReceivedFullPiece &,
                       int);

        void unconsumed_event(const sc::event_base &);

        // Deprecated: internals should not be directly visible for client or tester
        // Get name of current state: ***Varies from compiler to compiler***
        //const char * getInnerStateName() const;

        // Getters
        PeerModeAnnounced peerAnnouncedMode() const;

        int MAX_PIECE_INDEX() const;
        void setMAX_PIECE_INDEX(int);

    private:

        //// States require access to private machine state

        friend class ChooseMode;
        friend class Active;

        // Observing states
        friend class Observing;

        // Selling states
        friend class Selling;
        friend class ReadyForInvitation;
        friend class Invited;
        friend class WaitingToStart;
        friend class ServicingPieceRequest;
        friend class WaitingForPayment;
        friend class ReadyForPieceRequest;
        friend class LoadingPiece;

        // Buying states
        friend class Buying;
        friend class ReadyToInviteSeller;
        friend class WaitingForSellerToJoin;
        friend class PreparingContract;
        friend class ProcessingPiece;
        friend class SellerHasJoined;
        friend class PreparingContract;
        friend class ProcessingPiece;
        friend class ReadyToRequestPiece;
        friend class WaitingForFullPiece;

        //// State modifiers

        // Update peer mode state
        void peerToObserveMode();
        void peerToSellMode(const joystream::wire::SellerTerms &, uint32_t);
        void peerToBuyMode(const joystream::wire::BuyerTerms &);

        // Update mode state and send mode message and return corresponding transition
        void updateAndAnnounceClientMode();
        void updateAndAnnounceClientMode(const joystream::wire::SellerTerms &, uint32_t = 0);
        void updateAndAnnounceClientMode(const joystream::wire::BuyerTerms &);

        // Update payment channel based on terms
        void updatePayeeTerms(const joystream::wire::SellerTerms &);
        void updatePayorTerms(const joystream::wire::BuyerTerms &);

        //// Callbacks

        // Callbacks for classifier routines
        InvitedToOutdatedContract _invitedToOutdatedContract;
        InvitedToJoinContract _invitedToJoinContract;
        Send _sendMessage;
        ContractIsReady _contractIsReady;
        PieceRequested _pieceRequested;
        InvalidPieceRequested _invalidPieceRequested;
        PeerInterruptedPayment _peerInterruptedPayment;
        ValidPayment _validPayment;
        InvalidPayment _invalidPayment;
        SellerJoined _sellerJoined;
        SellerInterruptedContract _sellerInterruptedContract;
        ReceivedFullPiece _receivedFullPiece;

        // Greatest valid piece index
        int _MAX_PIECE_INDEX;

        //// Peer state

        joystream::protocol::PeerModeAnnounced _peerAnnouncedMode;

        //// Buyer Client state

        // Payor side of payment channel interaction
        paymentchannel::Payor _payor;

        //// Seller Client state

        // Index for most recent terms broadcasted
        uint32_t _index;

        // Payee side of payment channel interaction
        paymentchannel::Payee _payee;

    };
}
}
}

// Required to make CBStateMachine complete when included throught his header file,
// as ChooseMode is initial state and thus part of parent state_machine definition
#include <protocol/statemachine/ChooseMode.hpp>

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP
