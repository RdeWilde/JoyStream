/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#ifndef CBSTATEMACHINECALLBACKSPY
#define CBSTATEMACHINECALLBACKSPY

#include <protocol_statemachine/CBStateMachine.hpp>
#include <protocol_wire/MessageType.hpp>
#include <protocol_wire/ContractInvitation.hpp>
#include <protocol_wire/PieceData.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/Signature.hpp>

namespace joystream {
namespace protocol_wire {

    class BuyerTerms;
    class SellerTerms;
    class ExtendedMessagePayload;

}
}

using namespace joystream;

class CBStateMachineCallbackSpy {

public:

    CBStateMachineCallbackSpy();

    // Creates the machine, with given number of pieces, to spy on
    protocol_statemachine::CBStateMachine * createMonitoredMachine(int = 1);

    // Reset all callback indicators
    void reset();

    // Getters
    bool peerHasAnnouncedMode() const;
    protocol_statemachine::AnnouncedModeAndTerms announcedModeAndTerms() const;

    bool hasBeenInvitedToOutdatedContract() const;

    bool hasBeenInvitedToJoinContract() const;
    protocol_wire::ContractInvitation invitation() const;

    bool messageSent() const;
    std::shared_ptr<const protocol_wire::ExtendedMessagePayload> message() const;

    bool contractHasBeenPrepared() const;
    Coin::typesafeOutPoint anchor() const;

    bool pieceHasBeenRequested() const;
    int piece() const;

    bool invalidPieceHasBeenRequested() const;

    bool paymentInterrupted() const;

    bool receivedInvalidPayment() const;
    Coin::Signature invalidPaymentSignature() const;

    bool receivedValidPayment() const;
    Coin::Signature validPaymentSignature() const;

    bool sellerHasJoined() const;

    bool sellerHasInterruptedContract() const;

    bool hasReceivedFullPiece() const;

    protocol_wire::PieceData pieceData() const;

private:

    // ***
    // Variables for encoding result of most recent callback
    // ***

    bool _peerHasAnnouncedMode;
    protocol_statemachine::AnnouncedModeAndTerms _announcedModeAndTerms;

    // InvitedToOutdatedContract
    bool _hasBeenInvitedToOutdatedContract;

    // InvitedToJoinContract
    bool _hasBeenInvitedToJoinContract;
    protocol_wire::ContractInvitation _invitation;

    // Send
    protocol_statemachine::CBStateMachine::Send _send;
    bool _messageSent;
    std::shared_ptr<const protocol_wire::ExtendedMessagePayload> _message;

    // ContractIsReady
    bool _contractHasBeenPrepared;
    Coin::typesafeOutPoint _anchor;

    // PieceRequested
    bool _pieceHasBeenRequested;
    int _piece;

    // InvalidPieceRequested
    bool _invalidPieceHasBeenRequested;

    // PeerInterruptedPayment
    bool _paymentInterrupted;

    // InvalidPayment
    bool _receivedInvalidPayment;
    Coin::Signature _invalidPaymentSignature;

    // ValidPayment
    bool _receivedValidPayment;
    Coin::Signature _validPaymentSignature;

    // SellerJoined
    bool _sellerHasJoined;

    // SellerInterruptedContract
    bool _sellerHasInterruptedContract;

    // ReceivedFullPiece
    bool _hasReceivedFullPiece;
    protocol_wire::PieceData _pieceData;
};

#endif // CBSTATEMACHINECALLBACKSPY
