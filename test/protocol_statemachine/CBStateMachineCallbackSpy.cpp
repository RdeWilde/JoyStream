/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#include <CBStateMachineCallbackSpy.hpp>
#include <protocol_wire/ExtendedMessagePayload.hpp>

CBStateMachineCallbackSpy::CBStateMachineCallbackSpy() {

    // Set all indicators to neutral
    reset();
}

protocol_statemachine::CBStateMachine * CBStateMachineCallbackSpy::createMonitoredMachine(int MAX_PIECE_INDEX) {

    protocol_statemachine::CBStateMachine * machine = new protocol_statemachine::CBStateMachine(
    [this](const protocol_statemachine::AnnouncedModeAndTerms & a) {
        _peerHasAnnouncedMode = true;
        _announcedModeAndTerms = a;
    },
    [this](void) {
        _hasBeenInvitedToOutdatedContract = true;
    },
    [this](const protocol_wire::ContractInvitation & invitation) {
        _hasBeenInvitedToJoinContract = true;
        _invitation = invitation;
    },
    [this](const protocol_wire::ExtendedMessagePayload * m) {
        std::cout << "Sending message: " <<  protocol_wire::MessageTypeToString(m->messageType()) << std::endl;
        _messageSent = true;
        _message.reset(m);
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
        _receivedValidPayment = true;
        _validPaymentSignature = s;
    },
    [this](const Coin::Signature & s) {
        _receivedInvalidPayment = true;
        _invalidPaymentSignature = s;
    },
    [this]() {
        _sellerHasJoined = true;
    },
    [this]() {
        _sellerHasInterruptedContract = true;
    },
    [this](const protocol_wire::PieceData & p) {
        _hasReceivedFullPiece = true;
        _pieceData = p;
    },
    MAX_PIECE_INDEX);

    // Initiate machine
    machine->initiate();

    return machine;
}

void CBStateMachineCallbackSpy::reset() {

    // PeerAnnouncedMode
    _peerHasAnnouncedMode = false;
    _announcedModeAndTerms = protocol_statemachine::AnnouncedModeAndTerms();

    // InvitedToOutdatedContract
    _hasBeenInvitedToOutdatedContract = false;

    // InvitedToJoinContract
    _hasBeenInvitedToJoinContract = false;
    _invitation = protocol_wire::ContractInvitation();

    // Send
    _messageSent = false;
    _message.reset();

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
    _pieceData = protocol_wire::PieceData();
}

bool CBStateMachineCallbackSpy::peerHasAnnouncedMode() const {
    return _peerHasAnnouncedMode &&
           !_hasBeenInvitedToOutdatedContract &&
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

protocol_statemachine::AnnouncedModeAndTerms CBStateMachineCallbackSpy::announcedModeAndTerms() const {
    return _announcedModeAndTerms;
}

bool CBStateMachineCallbackSpy::hasBeenInvitedToOutdatedContract() const {
    return !_peerHasAnnouncedMode &&
           _hasBeenInvitedToOutdatedContract &&
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
    return !_peerHasAnnouncedMode &&
           !_hasBeenInvitedToOutdatedContract &&
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

protocol_wire::ContractInvitation CBStateMachineCallbackSpy::invitation() const {
    return _invitation;
}

bool CBStateMachineCallbackSpy::messageSent() const {
    return !_peerHasAnnouncedMode &&
           !_hasBeenInvitedToOutdatedContract &&
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

std::shared_ptr<const protocol_wire::ExtendedMessagePayload> CBStateMachineCallbackSpy::message() const {
    return _message;
}

bool CBStateMachineCallbackSpy::contractHasBeenPrepared() const {
    return !_peerHasAnnouncedMode &&
           !_hasBeenInvitedToOutdatedContract &&
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
    return !_peerHasAnnouncedMode &&
           !_hasBeenInvitedToOutdatedContract &&
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
    return !_peerHasAnnouncedMode &&
           !_hasBeenInvitedToOutdatedContract &&
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
    return !_peerHasAnnouncedMode &&
           !_hasBeenInvitedToOutdatedContract &&
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
    return !_peerHasAnnouncedMode &&
           !_hasBeenInvitedToOutdatedContract &&
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
    return !_peerHasAnnouncedMode &&
           !_hasBeenInvitedToOutdatedContract &&
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
    return !_peerHasAnnouncedMode &&
           !_hasBeenInvitedToOutdatedContract &&
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
    return !_peerHasAnnouncedMode &&
           !_hasBeenInvitedToOutdatedContract &&
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
    return !_peerHasAnnouncedMode &&
           !_hasBeenInvitedToOutdatedContract &&
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

protocol_wire::PieceData CBStateMachineCallbackSpy::pieceData() const {
    return _pieceData;
}
