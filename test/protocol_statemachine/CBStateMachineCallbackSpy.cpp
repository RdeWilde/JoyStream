/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#include <CBStateMachineCallbackSpy.hpp>
#include <protocol_wire/ExtendedMessagePayload.hpp>

CBStateMachineCallbackSpy::CBStateMachineCallbackSpy()
    : _message(nullptr) {

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
    [this]() {
        _hasBeenInvitedToJoinContract = true;
    },
    [this](const protocol_wire::ExtendedMessagePayload * m) {

        std::cout << "Sending message: " <<  protocol_wire::messageName(m->messageType()) << std::endl;

        _messageSent = true;

        if(_message != nullptr)
            delete _message;

        _message = m;
    },
    [this](quint64 value, const Coin::typesafeOutPoint & anchor, const Coin::PublicKey & contractPk, const Coin::PubKeyHash & finalPkHash) {
        _contractHasBeenPrepared = true;
        _anchor = anchor;
        _value = value;
        _contractPk = contractPk;
        _finalPkHash = finalPkHash;
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

    // Send
    _messageSent = false;

    if(_message != nullptr) {
        delete _message;
        _message = nullptr;
    }

    // ContractIsReady
    _contractHasBeenPrepared = false;
    _anchor = Coin::typesafeOutPoint();
    _value = 0;
    _contractPk = Coin::PublicKey();
    _finalPkHash = Coin::PubKeyHash();

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

quint64 CBStateMachineCallbackSpy::value() const {
    return _value;
}

Coin::PublicKey CBStateMachineCallbackSpy::contractPk() const {
    return _contractPk;
}

Coin::PubKeyHash CBStateMachineCallbackSpy::finalPkHash() const {
    return _finalPkHash;
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

protocol_wire::MessageType CBStateMachineCallbackSpy::messageType() const {
    return _message->messageType();
}

protocol_wire::Buy CBStateMachineCallbackSpy::buyMessage() const {
    return *dynamic_cast<const protocol_wire::Buy *>(_message);
}

protocol_wire::FullPiece CBStateMachineCallbackSpy::fullPieceMessage() const {
    return *dynamic_cast<const protocol_wire::FullPiece *>(_message);
}

protocol_wire::JoinContract CBStateMachineCallbackSpy::joinContractMessage() const {
    return *dynamic_cast<const protocol_wire::JoinContract *>(_message);
}

protocol_wire::JoiningContract CBStateMachineCallbackSpy::joiningContractMessage() const {
    return *dynamic_cast<const protocol_wire::JoiningContract *>(_message);
}

protocol_wire::Observe CBStateMachineCallbackSpy::observeMessage() const {
    return *dynamic_cast<const protocol_wire::Observe *>(_message);
}

protocol_wire::Payment CBStateMachineCallbackSpy::paymentMessage() const {
    return *dynamic_cast<const protocol_wire::Payment *>(_message);
}

protocol_wire::Ready CBStateMachineCallbackSpy::readyMessage() const {
    return *dynamic_cast<const protocol_wire::Ready *>(_message);
}

protocol_wire::RequestFullPiece CBStateMachineCallbackSpy::requestFullPieceMessage() const {
    return *dynamic_cast<const protocol_wire::RequestFullPiece *>(_message);
}

protocol_wire::Sell CBStateMachineCallbackSpy::sellMessage() const {
    return *dynamic_cast<const protocol_wire::Sell *>(_message);
}
