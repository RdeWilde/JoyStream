/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/Connection.hpp>

namespace joystream {
namespace protocol {

    Connection::Connection() {
    }

    Connection::Connection(const std::string & peerName, PeerModeAnnounced lastModeAnnouncedByPeer, const SendMessageCallbackHandler & sendMessageCallbackHandler)
        : _peerName(peerName)
        , _lastModeAnnouncedByPeer(lastModeAnnouncedByPeer)
        , _sendMessageCallbackHandler(sendMessageCallbackHandler) {
    }

    std::string Connection::peerName() const {
        return _peerName;
    }

    PeerModeAnnounced Connection::lastModeAnnouncedByPeer() const {
        return _lastModeAnnouncedByPeer;
    }

    SendMessageCallbackHandler Connection::sendMessageCallbackHandler() const {
        return _sendMessageCallbackHandler;
    }


}
}
