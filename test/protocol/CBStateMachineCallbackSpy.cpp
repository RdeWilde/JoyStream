/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#include <CBStateMachineCallbackSpy.hpp>
#include <protocol/wire/ExtendedMessagePayload.hpp>

CBStateMachineCallbackSpy::CBStateMachineCallbackSpy() {

    // Set all indicators to neutral
    reset();
}

CBStateMachine * CBStateMachineCallbackSpy::createMonitoredMachine(int MAX_PIECE_INDEX) {

    CBStateMachine * machine = new CBStateMachine(
    [this](void) {
        _hasBeenInvitedToOutdatedContract = true;
    },
    [this](const joystream::protocol::ContractInvitation & invitation) {
        _hasBeenInvitedToJoinContract = true;
        _invitation = invitation;
    },
    [this](const joystream::protocol::wire::ExtendedMessagePayload * m) {
        std::cout << "Sending message: " <<  joystream::protocol::wire::MessageTypeToString(m->messageType()) << std::endl;
        _messageSent = true;
        _message = m;
    },
    [this](const Coin::typesafeOutPoint & o) {
        _contractHasBeenPrepared = true;
        _anchor = o;
    },
    [this](int i) {
        _pieceHasBeenRequested = true;
        _piece = i;
    },
    [this]() {
        _invalidPieceHasBeenRequested = true;
    },
    [this]() {
        _paymentInterrupted = true;
    },
    [this](const Coin::Signature & s) {
        _receivedInvalidPayment = true;
        _invalidPaymentSignature = s;
    },
    [this](const Coin::Signature & s) {
        _receivedValidPayment = true;
        _validPaymentSignature = s;
    },
    [this]() {
        _sellerHasJoined = true;
    },
    [this]() {
        _sellerHasInterruptedContract = true;
    },
    [this](const joystream::protocol::PieceData & p) {
        _hasReceivedFullPiece = true;
        _pieceData = p;
    },
    MAX_PIECE_INDEX);

    // Initiate machine
    machine->initiate();

    return machine;
}

void CBStateMachineCallbackSpy::reset() {

    // InvitedToOutdatedContract
    _hasBeenInvitedToOutdatedContract = false;

    // InvitedToJoinContract
    _hasBeenInvitedToJoinContract = false;
    _invitation = joystream::protocol::ContractInvitation();

    // Send
    _messageSent = false;
    if(_message != NULL) {
        delete _message;
        _message = NULL;
    }

    // ContractIsReady
    _contractHasBeenPrepared = false;
    _anchor = Coin::typesafeOutPoint();

    // PieceRequested
    _pieceHasBeenRequested = false;
    _piece = 0;

    // InvalidPieceRequested
    _invalidPieceHasBeenRequested = false;

    // PeerInterruptedPayment
    _paymentInterrupted = false;

    // InvalidPayment
    _receivedInvalidPayment = false;
    _invalidPaymentSignature = Coin::Signature();

    // ValidPayment
    _receivedValidPayment = false;
    _validPaymentSignature = Coin::Signature();

    // SellerJoined
    _sellerHasJoined = false;

    // SellerInterruptedContract
    _sellerHasInterruptedContract = false;

    // ReceivedFullPiece
    _hasReceivedFullPiece = false;
    _pieceData = joystream::protocol::PieceData();
}

bool CBStateMachineCallbackSpy::hasBeenInvitedToOutdatedContract() const {
    return _hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_invalidPieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

bool CBStateMachineCallbackSpy::hasBeenInvitedToJoinContract() const {
    return !_hasBeenInvitedToOutdatedContract &&
           _hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_invalidPieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

joystream::protocol::ContractInvitation CBStateMachineCallbackSpy::invitation() const {
    return _invitation;
}

bool CBStateMachineCallbackSpy::messageSent() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           _messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_invalidPieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

const joystream::protocol::wire::ExtendedMessagePayload *CBStateMachineCallbackSpy::message() const {
    return _message;
}

bool CBStateMachineCallbackSpy::contractHasBeenPrepared() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           _contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_invalidPieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

Coin::typesafeOutPoint CBStateMachineCallbackSpy::anchor() const {
    return _anchor;
}

bool CBStateMachineCallbackSpy::pieceHasBeenRequested() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           _pieceHasBeenRequested &&
           !_invalidPieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

int CBStateMachineCallbackSpy::piece() const {
    return _piece;
}

bool CBStateMachineCallbackSpy::invalidPieceHasBeenRequested() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           _invalidPieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

bool CBStateMachineCallbackSpy::paymentInterrupted() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_invalidPieceHasBeenRequested &&
           _paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

bool CBStateMachineCallbackSpy::receivedInvalidPayment() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_invalidPieceHasBeenRequested &&
           !_paymentInterrupted &&
           _receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

Coin::Signature CBStateMachineCallbackSpy::invalidPaymentSignature() const {
    return _invalidPaymentSignature;
}

bool CBStateMachineCallbackSpy::receivedValidPayment() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_invalidPieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           _receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

Coin::Signature CBStateMachineCallbackSpy::validPaymentSignature() const {
    return _validPaymentSignature;
}

bool CBStateMachineCallbackSpy::sellerHasJoined() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_invalidPieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           _sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

bool CBStateMachineCallbackSpy::sellerHasInterruptedContract() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_invalidPieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           _sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

bool CBStateMachineCallbackSpy::hasReceivedFullPiece() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_invalidPieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           _hasReceivedFullPiece;
}

joystream::protocol::PieceData CBStateMachineCallbackSpy::pieceData() const {
    return _pieceData;
}
