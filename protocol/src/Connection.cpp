/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/Connection.hpp>
#include <protocol/Session.hpp>
#include <wire/MessageType.hpp>
#include <wire/Observe.hpp>
#include <wire/Buy.hpp>
#include <wire/Sell.hpp>
#include <wire/JoinContract.hpp>
#include <wire/JoiningContract.hpp>
#include <wire/Ready.hpp>
#include <wire/RequestFullPiece.hpp>
#include <wire/FullPiece.hpp>
#include <wire/Payment.hpp>
#include <protocol/statemachine/event/Recv.hpp>

namespace joystream {
namespace protocol {

    template <class ConnectionIdType>
    Connection<ConnectionIdType>::Connection(Session<ConnectionIdType> * session,
                                             const ConnectionIdType & connectionId,
                                             const SendMessageOnConnection & sendMessageOnConnection)
        : _session(session)
        , _connectionId(connectionId)
        , _sendMessageOnConnection(sendMessageOnConnection)
        , _machine(
                  [this](void) {
                      _session->invitedToOutdatedContract(_connectionId);
                  },
                  [this](const joystream::wire::ContractInvitation & invitation) {
                      _session->invitedToJoinContract(_connectionId,invitation);
                  },
                  [this](const joystream::wire::ExtendedMessagePayload * m) {
                      _sendMessageOnConnection(_connectionId, m);
                  },
                  [this](const Coin::typesafeOutPoint & o) {
                      _session->contractPrepared(_connectionId, o);
                  },
                  [this](int i) {
                      _session->pieceRequested(_connectionId, i);
                  },
                  [this]() {
                      _session->invalidPieceRequested(_connectionId);
                  },
                  [this]() {
                      _session->paymentInterrupted(_connectionId);
                  },
                  [this](const Coin::Signature & s) {
                      _session->receivedValidPayment(_connectionId, s);
                  },
                  [this](const Coin::Signature & s) {
                      _session->receivedInvalidPayment(_connectionId, s);
                  },
                  [this]() {
                      _session->sellerHasJoined(_connectionId);
                  },
                  [this]() {
                      _session->sellerHasInterruptedContract(_connectionId);
                  },
                  [this](const joystream::wire::PieceData & p) {
                      _session->receivedFullPiece(_connectionId, p);
                  },
                  0) {
    }

    template <class ConnectionIdType>
    void Connection<ConnectionIdType>::process(const wire::ExtendedMessagePayload * message) {

        // Get message type
        joystream::wire::MessageType messageType = message->messageType();

        // Call relevant message handler
        switch(messageType) {

            case joystream::wire::MessageType::observe:
                _machine.process_event(statemachine::event::Recv<joystream::wire::Observe>(dynamic_cast<const joystream::wire::Observe *>(message)));
                break;
            case joystream::wire::MessageType::buy:
                _machine.process_event(statemachine::event::Recv<joystream::wire::Buy>(dynamic_cast<const joystream::wire::Buy *>(message)));
                break;
            case joystream::wire::MessageType::sell:
                _machine.process_event(statemachine::event::Recv<joystream::wire::Sell>(dynamic_cast<const joystream::wire::Sell *>(message)));
                break;
            case joystream::wire::MessageType::join_contract:
                _machine.process_event(statemachine::event::Recv<joystream::wire::JoinContract>(dynamic_cast<const joystream::wire::JoinContract *>(message)));
                break;
            case joystream::wire::MessageType::joining_contract:
                _machine.process_event(statemachine::event::Recv<joystream::wire::JoiningContract>(dynamic_cast<const joystream::wire::JoiningContract *>(message)));
                break;
            case joystream::wire::MessageType::ready:
                _machine.process_event(statemachine::event::Recv<joystream::wire::Ready>(dynamic_cast<const joystream::wire::Ready *>(message)));
                break;
            case joystream::wire::MessageType::request_full_piece:
                _machine.process_event(statemachine::event::Recv<joystream::wire::RequestFullPiece>(dynamic_cast<const joystream::wire::RequestFullPiece *>(message)));
                break;
            case joystream::wire::MessageType::full_piece:
                _machine.process_event(statemachine::event::Recv<joystream::wire::FullPiece>(dynamic_cast<const joystream::wire::FullPiece *>(message)));
                break;
            case joystream::wire::MessageType::payment:
                _machine.process_event(statemachine::event::Recv<joystream::wire::Payment>(dynamic_cast<const joystream::wire::Payment *>(message)));
                break;

            default:
                assert(false);
        }
    }
}
}
