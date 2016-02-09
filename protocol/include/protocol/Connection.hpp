/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_CONNECTION_HPP
#define JOYSTREAM_PROTOCOL_CONNECTION_HPP

#include <protocol/PeerModeAnnounced.hpp>


#include <functional>

namespace joystream {
namespace protocol {

    namespace wire {
        class ExtendedMessagePayload;
    }

    class Connection {

    public:

        Connection();

        // Callback handler for sending a message to the peer
        typedef std::function<bool(const wire::ExtendedMessagePayload *)> SendMessageCallbackHandler;

        Connection(const std::string & peerName, PeerModeAnnounced lastModeAnnouncedByPeer, const SendMessageCallbackHandler & sendMessageCallbackHandler);

        // Getters
        std::string peerName() const;

        PeerModeAnnounced lastModeAnnouncedByPeer() const;

        SendMessageCallbackHandler sendMessageCallbackHandler() const;

    protected:

        // Name of peer: may or may not actually be needed
        std::string _peerName;

        // Mode announced by peer, i.e. last mode message received
        PeerModeAnnounced _lastModeAnnouncedByPeer;

        // Write message callback
        SendMessageCallbackHandler _sendMessageCallbackHandler;
    };
}
}

#endif // JOYSTREAM_PROTOCOL_CONNECTION_HPP
