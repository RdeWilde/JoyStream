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

    //// Setup all callback handlers

    // InvitedToOutdatedContract
    _invitedToOutdatedContract = [this](void) {
        _hasBeenInvitedToOutdatedContract = true;
    };

    // InvitedToJoinContract
    _invitedToJoinContract = [this](const joystream::protocol::ContractInvitation & invitation) {
        _hasBeenInvitedToJoinContract = true;
        _invitation = invitation;
    };

    // Send
    _send = [this](const joystream::protocol::wire::ExtendedMessagePayload * m) {

        std::cout << "Sending message: " <<  joystream::protocol::wire::MessageTypeToString(m->messageType()) << std::endl;

        _messageSent = true;
        _message = m;
    };

    // ContractIsReady
    _contractIsReady = [this](const Coin::typesafeOutPoint & o) {
        _contractHasBeenPrepared = true;
        _anchor = o;
    };

    // PieceRequested
    _pieceRequested = [this](int i) {
        _pieceHasBeenRequested = true;
        _piece = i;
    };

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
    CBStateMachine * machine = new CBStateMachine(_invitedToOutdatedContract, _invitedToJoinContract, _send, _contractIsReady, _pieceRequested);

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
}

bool StateMachineCallbackSpy::hasBeenInvitedToOutdatedContract() const {
    return _hasBeenInvitedToOutdatedContract;
}

bool StateMachineCallbackSpy::hasBeenInvitedToJoinContract() const {
    return _hasBeenInvitedToJoinContract;
}

joystream::protocol::ContractInvitation StateMachineCallbackSpy::invitation() const {
    return _invitation;
}

bool StateMachineCallbackSpy::messageSent() const {
    return _messageSent;
}

const joystream::protocol::wire::ExtendedMessagePayload *StateMachineCallbackSpy::message() const {
    return _message;
}

bool StateMachineCallbackSpy::contractHasBeenPrepared() const {
    return _contractHasBeenPrepared;
}

Coin::typesafeOutPoint StateMachineCallbackSpy::anchor() const {
    return _anchor;
}

bool StateMachineCallbackSpy::pieceHasBeenRequested() const {
    return _pieceHasBeenRequested;
}

int StateMachineCallbackSpy::piece() const {
    return _piece;
}
