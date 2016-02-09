/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/buy/Connection.hpp>

namespace joystream {
namespace protocol {
namespace buy {

    Connection::Connection(const std::string & peerName,
                           PeerModeAnnounced lastModeAnnouncedByPeer,
                           const SendMessageCallbackHandler & sendMessageCallbackHandler,
                           ClientState clientState,
                           PeerState peerState,
                           const std::list<uint32_t> & downloadedValidPieces)
        : joystream::protocol::Connection(peerName, lastModeAnnouncedByPeer, sendMessageCallbackHandler)
        , _clientState(clientState)
        , _peerState(peerState)
        , _downloadedValidPieces(downloadedValidPieces) {
    }

    ClientState Connection::clientState() const {
        return _clientState;
    }

    PeerState Connection::peerState() const {
        return _peerState;
    }

    std::list<uint32_t> Connection::downloadedValidPieces() const {
        return _downloadedValidPieces;
    }



}
}
}
