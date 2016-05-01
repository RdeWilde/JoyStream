/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol_session/detail/Connection.hpp>
#include <protocol_wire/protocol_wire.hpp>

namespace joystream {
namespace protocol_session {
namespace detail {

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
    void Connection<ConnectionIdType>::processMessage(const protocol_wire::ExtendedMessagePayload & message) {

        // Get message type
        protocol_wire::MessageType messageType = message.messageType();

        // Call relevant message handler
        switch(messageType) {

            case protocol_wire::MessageType::observe:
                processEvent(protocol_statemachine::event::Recv<protocol_wire::Observe>(static_cast<const protocol_wire::Observe &>(message)));
                break;
            case protocol_wire::MessageType::buy:
                processEvent(protocol_statemachine::event::Recv<protocol_wire::Buy>(static_cast<const protocol_wire::Buy &>(message)));
                break;
            case protocol_wire::MessageType::sell:
                processEvent(protocol_statemachine::event::Recv<protocol_wire::Sell>(static_cast<const protocol_wire::Sell &>(message)));
                break;
            case protocol_wire::MessageType::join_contract:
                processEvent(protocol_statemachine::event::Recv<protocol_wire::JoinContract>(static_cast<const protocol_wire::JoinContract &>(message)));
                break;
            case protocol_wire::MessageType::joining_contract:
                processEvent(protocol_statemachine::event::Recv<protocol_wire::JoiningContract>(static_cast<const protocol_wire::JoiningContract &>(message)));
                break;
            case protocol_wire::MessageType::ready:
                processEvent(protocol_statemachine::event::Recv<protocol_wire::Ready>(static_cast<const protocol_wire::Ready &>(message)));
                break;
            case protocol_wire::MessageType::request_full_piece:
                processEvent(protocol_statemachine::event::Recv<protocol_wire::RequestFullPiece>(static_cast<const protocol_wire::RequestFullPiece &>(message)));
                break;
            case protocol_wire::MessageType::full_piece:
                processEvent(protocol_statemachine::event::Recv<protocol_wire::FullPiece>(static_cast<const protocol_wire::FullPiece &>(message)));
                break;
            case protocol_wire::MessageType::payment:
                processEvent(protocol_statemachine::event::Recv<protocol_wire::Payment>(static_cast<const protocol_wire::Payment &>(message)));
                break;

            default:
                assert(false);
        }
    }

    template <class ConnectionIdType>
    void Connection<ConnectionIdType>::processEvent(const boost::statechart::event_base & e) {
        _machine.processEvent(e);
    }

    template <class ConnectionIdType>
    ConnectionIdType Connection<ConnectionIdType>::connectionId() const {
        return _connectionId;
    }

    template <class ConnectionIdType>
    const protocol_statemachine::CBStateMachine & Connection<ConnectionIdType>::machine() const {
        return _machine;
    }

}
}
}
