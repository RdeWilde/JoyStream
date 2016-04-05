/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#ifndef CBSTATEMACHINECALLBACKSPY
#define CBSTATEMACHINECALLBACKSPY

#include <protocol/statemachine/CBStateMachine.hpp>
#include <wire/MessageType.hpp>
#include <wire/ContractInvitation.hpp>
#include <wire/PieceData.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/Signature.hpp>

namespace joystream {
namespace protocol {

    class BuyerTerms;
    class SellerTerms;

    namespace wire {
        class ExtendedMessagePayload;
    }
}
}

using namespace joystream::protocol::statemachine;

class CBStateMachineCallbackSpy {

public:

    CBStateMachineCallbackSpy();

    // Creates the machine, with given number of pieces, to spy on
    CBStateMachine * createMonitoredMachine(int = 1);

    // Reset all callback indicators
    void reset();

    // Getters
    bool hasBeenInvitedToOutdatedContract() const;

    bool hasBeenInvitedToJoinContract() const;
    joystream::wire::ContractInvitation invitation() const;

    bool messageSent() const;
    const joystream::wire::ExtendedMessagePayload *message() const;

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

    joystream::wire::PieceData pieceData() const;

private:

    // ***
    // Variables for encoding result of most recent callback
    // ***

    // InvitedToOutdatedContract
    bool _hasBeenInvitedToOutdatedContract;

    // InvitedToJoinContract
    bool _hasBeenInvitedToJoinContract;
    joystream::wire::ContractInvitation _invitation;

    // Send
    CBStateMachine::Send _send;
    bool _messageSent;
    const joystream::wire::ExtendedMessagePayload * _message;

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
    joystream::wire::PieceData _pieceData;
};

#endif // CBSTATEMACHINECALLBACKSPY
