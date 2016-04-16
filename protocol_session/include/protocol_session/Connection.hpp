/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_CONNECTION_HPP
#define JOYSTREAM_PROTOCOLSESSION_CONNECTION_HPP

#include <protocol_statemachine/protocol_statemachine.hpp>

#include <queue>

namespace joystream {
namespace protocol_wire {
    class ExtendedMessagePayload;
}
namespace protocol_session {

    template <class ConnectionIdType>
    // ConnectionIdType: type for identifying connections, must
    // be possible to use as key in std::map, and also have
    // std::string ConnectionIdType::toString() const
    class Connection {

    public:

        Connection(const ConnectionIdType &,
                   const protocol_statemachine::CBStateMachine::PeerAnnouncedMode &,
                   const protocol_statemachine::CBStateMachine::InvitedToOutdatedContract &,
                   const protocol_statemachine::CBStateMachine::InvitedToJoinContract &,
                   const protocol_statemachine::CBStateMachine::Send &,
                   const protocol_statemachine::CBStateMachine::ContractIsReady &,
                   const protocol_statemachine::CBStateMachine::PieceRequested &,
                   const protocol_statemachine::CBStateMachine::InvalidPieceRequested &,
                   const protocol_statemachine::CBStateMachine::PeerInterruptedPayment &,
                   const protocol_statemachine::CBStateMachine::ValidPayment &,
                   const protocol_statemachine::CBStateMachine::InvalidPayment &,
                   const protocol_statemachine::CBStateMachine::SellerJoined &,
                   const protocol_statemachine::CBStateMachine::SellerInterruptedContract &,
                   const protocol_statemachine::CBStateMachine::ReceivedFullPiece &);

        // Processes given message
        void process(const protocol_wire::ExtendedMessagePayload &);

        // Commitment in this interaction
        paymentchannel::Commitment commitment() const;

    private:

        // Connection id
        ConnectionIdType _connectionId;

        // State machine for this connection
        protocol_statemachine::CBStateMachine _machine;

        //// Buyer

        // Point in time when last invite sent
        time_t _whenLastInviteSent;

        // Indexes of valid piecesm, in the order they were downloaded
        // NB: The reason this is not in Seller, is because
        // any peer can potentially provide valid pieces
        std::queue<uint32_t> _downloadedValidPieces;
    };

}
}

#endif // JOYSTREAM_PROTOCOLSESSION_CONNECTION_HPP
