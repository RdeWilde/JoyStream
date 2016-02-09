/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/Connection.hpp>
#include <protocol/buy/ClientState.hpp>
#include <protocol/buy/PeerState.hpp>

#include <list>

#ifndef JOYSTREAM_PROTOCOL_BUY_CONNECTION_HPP
#define JOYSTREAM_PROTOCOL_BUY_CONNECTION_HPP

namespace joystream {
namespace protocol {
namespace buy {

    class Connection : public joystream::protocol::Connection {

    public:

        Connection(const std::string & peerName,
                   PeerModeAnnounced lastModeAnnouncedByPeer,
                   const SendMessageCallbackHandler & sendMessageCallbackHandler,
                   ClientState clientState,
                   PeerState peerState,
                   const std::list<uint32_t> & downloadedValidPieces);

        // Getters
        ClientState clientState() const;

        PeerState peerState() const;

        std::list<uint32_t> downloadedValidPieces() const;

    private:

        // State of client (us) on this connection
        ClientState _clientState;

        // State of peer on this connection
        PeerState _peerState;

        // Index of a piece assigned to this peer, only valid if
        // _clientState == ClientState::waiting_for_full_piece or
        // ClientState::waiting_for_libtorrent_to_validate_piece
        //int _indexOfAssignedPiece;

        // Indexes of valid piecesm, in the order they were downloaded
        std::list<uint32_t> _downloadedValidPieces;

    };
}
}
}

#endif // JOYSTREAM_PROTOCOL_BUY_CONNECTION_HPP
