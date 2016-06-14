/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#ifndef CBSTATEMACHINECALLBACKSPY
#define CBSTATEMACHINECALLBACKSPY

#include <protocol_statemachine/protocol_statemachine.hpp>
#include <protocol_wire/protocol_wire.hpp>
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
using namespace joystream::protocol_statemachine;

class CBStateMachineCallbackSpy {

public:

    CBStateMachineCallbackSpy();

    // Creates the machine, with given number of pieces, to spy on
    CBStateMachine * createMonitoredMachine(int = 1);

    // Reset all callback indicators
    void reset();

    // Getters
    bool peerHasAnnouncedMode() const;
    AnnouncedModeAndTerms announcedModeAndTerms() const;

    bool hasBeenInvitedToOutdatedContract() const;

    bool hasBeenInvitedToJoinContract() const;

    bool messageSent() const;

    protocol_wire::PieceData pieceData() const;

    protocol_wire::MessageType messageType() const;

    protocol_wire::Buy buyMessage() const;

    protocol_wire::FullPiece fullPieceMessage() const;

    protocol_wire::JoinContract joinContractMessage() const;

    protocol_wire::JoiningContract joiningContractMessage() const;

    protocol_wire::Observe observeMessage() const;

    protocol_wire::Payment paymentMessage() const;

    protocol_wire::Ready readyMessage() const;

    protocol_wire::RefundSigned refundSignedMessage() const;

    protocol_wire::RequestFullPiece requestFullPieceMessage() const;

    protocol_wire::Sell sellMessage() const;

    protocol_wire::SignRefund signRefundMessage() const;

    bool contractHasBeenPrepared() const;
    Coin::typesafeOutPoint anchor() const;
    quint64 value() const;
    Coin::PublicKey contractPk() const;
    Coin::RedeemScriptHash finalScriptHash() const;

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

private:

    // ***
    // Variables for encoding result of most recent callback
    // ***

    bool _peerHasAnnouncedMode;
    AnnouncedModeAndTerms _announcedModeAndTerms;

    // InvitedToOutdatedContract
    bool _hasBeenInvitedToOutdatedContract;

    // InvitedToJoinContract
    bool _hasBeenInvitedToJoinContract;

    // Send
    bool _messageSent;
    const protocol_wire::ExtendedMessagePayload * _message;
    /**
    protocol_wire::MessageType _messageType;
    protocol_wire::Buy _buyMessage;
    protocol_wire::FullPiece _fullPieceMessage;
    protocol_wire::JoinContract _joinContractMessage;
    protocol_wire::JoiningContract _joiningContractMessage;
    protocol_wire::Observe _observeMessage;
    protocol_wire::Payment _paymentMessage;
    protocol_wire::Ready _readyMessage;
    protocol_wire::RefundSigned _refundSignedMessage;
    protocol_wire::RequestFullPiece _requestFullPieceMessage;
    protocol_wire::Sell _sellMessage;
    protocol_wire::SignRefund _signRefundMessage;
    */

    // ContractIsReady
    bool _contractHasBeenPrepared;
    Coin::typesafeOutPoint _anchor;
    quint64 _value;
    Coin::PublicKey _contractPk;
    Coin::RedeemScriptHash _finalScriptHash;

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
