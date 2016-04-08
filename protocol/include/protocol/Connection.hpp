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
    class Session;

    template <class ConnectionIdType>
    // ConnectionIdType: type for identifying connections, must
    // be possible to use as key in std::map, and also have
    // std::string ConnectionIdType::toString() const
    class Connection {

    public:

        // Client requires a message to be sent
        typedef std::function<void(const wire::ExtendedMessagePayload *)> SendMessageOnConnection;

        Connection(Session<ConnectionIdType> *,
                   const ConnectionIdType &,
                   const SendMessageOnConnection &,
                   int MAX_PIECE_INDEX);

        // Processes given message
        void process(const wire::ExtendedMessagePayload *);

    private:

        // Session to which this connection corresponds
        Session<ConnectionIdType> * _session;

        // Connection id
        ConnectionIdType _connectionId;

        // Hook for sending message on connection
        SendMessageOnConnection _sendMessageOnConnection;

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
