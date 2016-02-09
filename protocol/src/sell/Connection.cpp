/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/sell/Connection.hpp>

namespace joystream {
namespace protocol {
namespace sell {

    Connection::Connection(const std::string & peerName,
                           PeerModeAnnounced lastModeAnnouncedByPeer,
                           const SendMessageCallbackHandler & sendMessageCallbackHandler,
                           ClientState clientState,
                           PeerState peerState,
                           const joystream::paymentchannel::Payee & payee,
                           const std::list<uint32_t> & fullPiecesSent)
        : joystream::protocol::Connection(peerName, lastModeAnnouncedByPeer, sendMessageCallbackHandler)
        , _clientState(clientState)
        , _peerState(peerState)
        , _fullPiecesSent(downloadedValidPieces) {

    }

    ClientState Connection::clientState() const {
        return _clientState;
    }

    PeerState Connection::peerState() const {
        return _peerState;
    }

    joystream::paymentchannel::Payee Connection::payee() const {
        return _payee;
    }

    std::list<uint32_t> Connection::fullPiecesSent() const {
        return _fullPiecesSent;
    }
}
}
}
