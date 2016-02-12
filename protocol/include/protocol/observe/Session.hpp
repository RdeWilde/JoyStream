/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_OBSERVE_SESSION_HPP
#define JOYSTREAM_PROTOCOL_OBSERVE_SESSION_HPP

#include <protocol/Session.hpp>
#include <protocol/Connection.hpp>

#include <map>

namespace joystream {
namespace protocol {

namespace sell {
    class Session;
    class Terms;
}

namespace buy {
    class Session;
    class Terms;
}

namespace observe {

    class Session : public joystream::protocol::Session {

    public:

        Session(const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler, const std::map<std::string, Connection> & connections);

        // Returns session for corresponding new mode, after sending appropriate messages to all active peers,
        // and returned object is owned by callee.
        joystream::protocol::sell::Session * switchToSellMode(const joystream::protocol::sell::Terms & terms, uint32_t numberOfPiecesInTorrent);
        joystream::protocol::buy::Session * switchToSellMode(const joystream::protocol::buy::Terms & terms);

        virtual void addConnection(const Connection & connection);

        virtual void removeConnection(const std::string & name);

        virtual void processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload & message);

    private:

        // Mapping peer name to corresponding connection with peer
        std::map<std::string, Connection> _connections;
    };
}
}
}

#endif // JOYSTREAM_PROTOCOL_OBSERVE_SESSION_HPP

