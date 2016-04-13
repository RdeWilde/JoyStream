/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol_session/Connection.hpp>
#include <protocol_wire/MessageType.hpp>
#include <protocol_wire/Observe.hpp>
#include <protocol_wire/Buy.hpp>
#include <protocol_wire/Sell.hpp>
#include <protocol_wire/JoinContract.hpp>
#include <protocol_wire/JoiningContract.hpp>
#include <protocol_wire/Ready.hpp>
#include <protocol_wire/RequestFullPiece.hpp>
#include <protocol_wire/FullPiece.hpp>
#include <protocol_wire/Payment.hpp>
#include <protocol_statemachine/event/Recv.hpp>

namespace joystream {
namespace protocol_session {

    template <class ConnectionIdType>
    Connection<ConnectionIdType>::Connection(const ConnectionIdType & connectionId,
                                             const protocol_statemachine::CBStateMachine::PeerAnnouncedMode & peerAnnouncedMode,
                                             const protocol_statemachine::CBStateMachine::InvitedToOutdatedContract & invitedToOutdatedContract,
                                             const protocol_statemachine::CBStateMachine::InvitedToJoinContract & invitedToJoinContract,
                                             const protocol_statemachine::CBStateMachine::Send & send,
                                             const protocol_statemachine::CBStateMachine::ContractIsReady & contractIsReady,
                                             const protocol_statemachine::CBStateMachine::PieceRequested & pieceRequested,
                                             const protocol_statemachine::CBStateMachine::InvalidPieceRequested & invalidPieceRequested,
                                             const protocol_statemachine::CBStateMachine::PeerInterruptedPayment & peerInterruptedPayment,
                                             const protocol_statemachine::CBStateMachine::ValidPayment & validPayment,
                                             const protocol_statemachine::CBStateMachine::InvalidPayment & invalidPayment,
                                             const protocol_statemachine::CBStateMachine::SellerJoined & sellerJoined,
                                             const protocol_statemachine::CBStateMachine::SellerInterruptedContract & sellerInterruptedContract,
                                             const protocol_statemachine::CBStateMachine::ReceivedFullPiece & receivedFullPiece)
        : _connectionId(connectionId)
        , _machine(peerAnnouncedMode,
                   invitedToOutdatedContract,
                   invitedToJoinContract,
                   send,
                   contractIsReady,
                   pieceRequested,
                   invalidPieceRequested,
                   peerInterruptedPayment,
                   validPayment,
                   invalidPayment,
                   sellerJoined,
                   sellerInterruptedContract,
                   receivedFullPiece,
                   0) {
    }

    template <class ConnectionIdType>
    void Connection<ConnectionIdType>::process(const protocol_wire::ExtendedMessagePayload & message) {

        // Get message type
        protocol_wire::MessageType messageType = message.messageType();

        // Call relevant message handler
        switch(messageType) {

            case protocol_wire::MessageType::observe:
                _machine.process_event(protocol_statemachine::event::Recv<protocol_wire::Observe>(static_cast<const protocol_wire::Observe &>(message)));
                break;
            case protocol_wire::MessageType::buy:
                _machine.process_event(protocol_statemachine::event::Recv<protocol_wire::Buy>(static_cast<const protocol_wire::Buy &>(message)));
                break;
            case protocol_wire::MessageType::sell:
                _machine.process_event(protocol_statemachine::event::Recv<protocol_wire::Sell>(static_cast<const protocol_wire::Sell &>(message)));
                break;
            case protocol_wire::MessageType::join_contract:
                _machine.process_event(protocol_statemachine::event::Recv<protocol_wire::JoinContract>(static_cast<const protocol_wire::JoinContract &>(message)));
                break;
            case protocol_wire::MessageType::joining_contract:
                _machine.process_event(protocol_statemachine::event::Recv<protocol_wire::JoiningContract>(static_cast<const protocol_wire::JoiningContract &>(message)));
                break;
            case protocol_wire::MessageType::ready:
                _machine.process_event(protocol_statemachine::event::Recv<protocol_wire::Ready>(static_cast<const protocol_wire::Ready &>(message)));
                break;
            case protocol_wire::MessageType::request_full_piece:
                _machine.process_event(protocol_statemachine::event::Recv<protocol_wire::RequestFullPiece>(static_cast<const protocol_wire::RequestFullPiece &>(message)));
                break;
            case protocol_wire::MessageType::full_piece:
                _machine.process_event(protocol_statemachine::event::Recv<protocol_wire::FullPiece>(static_cast<const protocol_wire::FullPiece &>(message)));
                break;
            case protocol_wire::MessageType::payment:
                _machine.process_event(protocol_statemachine::event::Recv<protocol_wire::Payment>(static_cast<const protocol_wire::Payment &>(message)));
                break;

            default:
                assert(false);
        }
    }
}
}
