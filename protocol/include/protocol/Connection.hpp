/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_CONNECTION_HPP
#define JOYSTREAM_PROTOCOL_CONNECTION_HPP

#include <protocol/statemachine/CBStateMachine.hpp>

#include <queue>

namespace joystream {
namespace wire {
    class ExtendedMessagePayload;
}
namespace protocol {

    template <class ConnectionIdType>
    // ConnectionIdType: type for identifying connections, must
    // be possible to use as key in std::map, and also have
    // std::string ConnectionIdType::toString() const
    class Connection {

    public:

        Connection(const ConnectionIdType &,
                   const statemachine::CBStateMachine::InvitedToOutdatedContract &,
                   const statemachine::CBStateMachine::InvitedToJoinContract &,
                   const statemachine::CBStateMachine::Send &,
                   const statemachine::CBStateMachine::ContractIsReady &,
                   const statemachine::CBStateMachine::PieceRequested &,
                   const statemachine::CBStateMachine::InvalidPieceRequested &,
                   const statemachine::CBStateMachine::PeerInterruptedPayment &,
                   const statemachine::CBStateMachine::ValidPayment &,
                   const statemachine::CBStateMachine::InvalidPayment &,
                   const statemachine::CBStateMachine::SellerJoined &,
                   const statemachine::CBStateMachine::SellerInterruptedContract &,
                   const statemachine::CBStateMachine::ReceivedFullPiece &);

        // Processes given message
        void process(const wire::ExtendedMessagePayload *);

    private:

        // Connection id
        ConnectionIdType _connectionId;

        // State machine for this connection
        statemachine::CBStateMachine _machine;

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

#endif // JOYSTREAM_PROTOCOL_CONNECTION_HPP
