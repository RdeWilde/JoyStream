/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP

#include <protocol_statemachine/AnnouncedModeAndTerms.hpp>
#include <protocol_statemachine/event/Recv.hpp>
#include <paymentchannel/paymentchannel.hpp>

#include <boost/statechart/state_machine.hpp>

namespace sc = boost::statechart;

namespace Coin {
    class typesafeOutPoint;
    class PublicKey;
    class Signature;
}

namespace joystream {
namespace protocol_wire {
    class ExtendedMessagePayload;
    class SellerTerms;
    class BuyerTerms;
    class Ready;
    class PieceData;
}

namespace protocol_statemachine {

    class ChooseMode; // Default state

    class CBStateMachine : public sc::state_machine<CBStateMachine, ChooseMode> {

    public:

        //// General Notifications

        typedef std::function<void(const protocol_statemachine::AnnouncedModeAndTerms &)> PeerAnnouncedMode;

        // Client requires a message to be sent
        typedef std::function<void(const protocol_wire::ExtendedMessagePayload &)> Send;

        //// Selling Notifications

        // Client was invited to expired contract, as indicated by bad index
        typedef std::function<void()> InvitedToOutdatedContract;

        // Client was invited to join given contract, should terms be included? they are available in _peerAnnounced
        typedef std::function<void()> InvitedToJoinContract;

        // Peer announced that contract is now ready, should contract be be included? it was available
        typedef std::function<void(quint64, const Coin::typesafeOutPoint &, const Coin::PublicKey &, const Coin::PubKeyHash &)> ContractIsReady;

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
        typedef std::function<void(const protocol_wire::PieceData &)> ReceivedFullPiece;

        CBStateMachine();

        CBStateMachine(const PeerAnnouncedMode &,
                       const InvitedToOutdatedContract &,
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

        // Whether state machine is in given (T) inner state
        template<typename T>
        bool inState() const;

        // Deprecated: internals should not be directly visible for client or tester
        // Get name of current state: ***Varies from compiler to compiler***
        //const char * getInnerStateName() const;

        // Mode of client
        ModeAnnounced clientMode() const;

        // Getters
        AnnouncedModeAndTerms announcedModeAndTermsFromPeer() const;

        int MAX_PIECE_INDEX() const;
        void setMAX_PIECE_INDEX(int);

        paymentchannel::Payor payor() const;

        paymentchannel::Payee payee() const;

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
        friend class ReadyToRequestPiece;
        friend class WaitingForFullPiece;

        //// State modifiers

        // Update peer mode state and payor/payee
        void peerToObserveMode();
        void peerToSellMode(const protocol_wire::SellerTerms &, uint32_t);
        void peerToBuyMode(const protocol_wire::BuyerTerms &);

        // Update payor/payee and send mode message
        void clientToObserveMode();
        void clientToSellMode(const protocol_wire::SellerTerms &, uint32_t = 0);
        void clientToBuyMode(const protocol_wire::BuyerTerms &);

        //// Callbacks

        // Callbacks for classifier routines
        PeerAnnouncedMode _peerAnnouncedMode;
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

        void peerAnnouncedMode();

        // Greatest valid piece index
        int _MAX_PIECE_INDEX;

        //// Peer state

        protocol_statemachine::AnnouncedModeAndTerms _announcedModeAndTermsFromPeer;

        //// Buyer Client state

        // Payor side of payment channel interaction
        paymentchannel::Payor _payor;

        //// Seller Client state

        // Index for most recent terms broadcasted
        uint32_t _index;

        // Payee side of payment channel interaction
        paymentchannel::Payee _payee;

    };

    template<typename T>
    bool CBStateMachine::inState() const {
        return this->state_cast<const T *>() != NULL;
    }
}
}

// Required to make CBStateMachine complete when included throught his header file,
// as ChooseMode is initial state and thus part of parent state_machine definition
#include <protocol_statemachine/ChooseMode.hpp>

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_CBSTATEMACHINE_HPP
