/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#include <StateMachineCallbackSpy.hpp>
#include <protocol/wire/ExtendedMessagePayload.hpp>

StateMachineCallbackSpy::StateMachineCallbackSpy() {

    // Set all indicators to neutral
    reset();
}

CBStateMachine * StateMachineCallbackSpy::createFreshMachineInObserveMode() {

    // Create and initiate state machine
    CBStateMachine * machine = createFreshMachine();

    // Switch to observe mode
    machine->process_event(event::ObserveModeStarted());

    return machine;
}

CBStateMachine * StateMachineCallbackSpy::createFreshMachineInBuyMode(const joystream::protocol::BuyerTerms & terms) {

    // Create and initiate state machine
    CBStateMachine * machine = createFreshMachine();

    // Switch to buy mode
    machine->process_event(event::BuyModeStarted(terms));

    return machine;
}

CBStateMachine * StateMachineCallbackSpy::createFreshMachineInSellMode(const joystream::protocol::SellerTerms & terms) {

    // Create and initiate state machine
    CBStateMachine * machine = createFreshMachine();

    // Switch to sell mode
    machine->process_event(event::SellModeStarted(terms));

    return machine;
}

CBStateMachine * StateMachineCallbackSpy::createFreshMachine() {

    // Create
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
    });

    // Initiate machine
    machine->initiate();

    return machine;
}

void StateMachineCallbackSpy::reset() {

    // InvitedToOutdatedContract
    _hasBeenInvitedToOutdatedContract = false;

    // InvitedToJoinContract
    _hasBeenInvitedToJoinContract = false;
    _invitation = joystream::protocol::ContractInvitation();

    // Send
    _messageSent = false;
    _message = NULL;

    // ContractIsReady
    _contractHasBeenPrepared = false;
    _anchor = Coin::typesafeOutPoint();

    // PieceRequested
    _pieceHasBeenRequested = false;
    _piece = 0;

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

bool StateMachineCallbackSpy::hasBeenInvitedToOutdatedContract() const {
    return _hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

bool StateMachineCallbackSpy::hasBeenInvitedToJoinContract() const {
    return !_hasBeenInvitedToOutdatedContract &&
           _hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

joystream::protocol::ContractInvitation StateMachineCallbackSpy::invitation() const {
    return _invitation;
}

bool StateMachineCallbackSpy::messageSent() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           _messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

const joystream::protocol::wire::ExtendedMessagePayload *StateMachineCallbackSpy::message() const {
    return _message;
}

bool StateMachineCallbackSpy::contractHasBeenPrepared() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           _contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

Coin::typesafeOutPoint StateMachineCallbackSpy::anchor() const {
    return _anchor;
}

bool StateMachineCallbackSpy::pieceHasBeenRequested() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           _pieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

int StateMachineCallbackSpy::piece() const {
    return _piece;
}

bool StateMachineCallbackSpy::paymentInterrupted() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           _paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

bool StateMachineCallbackSpy::receivedInvalidPayment() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_paymentInterrupted &&
           _receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

Coin::Signature StateMachineCallbackSpy::invalidPaymentSignature() const {
    return _invalidPaymentSignature;
}

bool StateMachineCallbackSpy::receivedValidPayment() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           _receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

Coin::Signature StateMachineCallbackSpy::validPaymentSignature() const {
    return _validPaymentSignature;
}

bool StateMachineCallbackSpy::sellerHasJoined() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           _sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

bool StateMachineCallbackSpy::sellerHasInterruptedContract() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           _sellerHasInterruptedContract &&
           !_hasReceivedFullPiece;
}

bool StateMachineCallbackSpy::hasReceivedFullPiece() const {
    return !_hasBeenInvitedToOutdatedContract &&
           !_hasBeenInvitedToJoinContract &&
           !_messageSent &&
           !_contractHasBeenPrepared &&
           !_pieceHasBeenRequested &&
           !_paymentInterrupted &&
           !_receivedInvalidPayment &&
           !_receivedValidPayment &&
           !_sellerHasJoined &&
           !_sellerHasInterruptedContract &&
           _hasReceivedFullPiece;
}

joystream::protocol::PieceData StateMachineCallbackSpy::pieceData() const {
    return _pieceData;
}
