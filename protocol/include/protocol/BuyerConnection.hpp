/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/Connection.hpp>
#include <protocol/BuyerClientState.hpp>
#include <protocol/BuyerPeerState.hpp>

#include <queue>

#ifndef JOYSTREAM_PROTOCOL_BUYER_CONNECTION_HPP
#define JOYSTREAM_PROTOCOL_BUYER_CONNECTION_HPP

namespace joystream {
namespace protocol {

    class BuyerConnection : public Connection {

    public:

        BuyerConnection();

        BuyerConnection(const std::string & peerName,
                       PeerModeAnnounced lastModeAnnouncedByPeer,
                       const SendMessageCallbackHandler & sendMessageCallbackHandler,
                       BuyerClientState clientState,
                       BuyerPeerState peerState,
                       const std::queue<uint32_t> & downloadedValidPieces);

        // Create a (buyer) connection which is fresh, i.e. has never had any message transmitted except buyer mode message
        static BuyerConnection buyMessageJustSent(const Connection & connection);

        // Getters
        BuyerClientState clientState() const;

        BuyerPeerState peerState() const;

        std::list<uint32_t> downloadedValidPieces() const;

    private:

        // State of client (us) on this connection
        BuyerClientState _clientState;

        // State of peer on this connection
        BuyerPeerState _peerState;

        // Index of a piece assigned to this peer, only valid if
        // _clientState == ClientState::waiting_for_full_piece or
        // ClientState::waiting_for_libtorrent_to_validate_piece
        //int _indexOfAssignedPiece;

        // Indexes of valid piecesm, in the order they were downloaded
        std::queue<uint32_t> _downloadedValidPieces;

    };

}
}

#endif // JOYSTREAM_PROTOCOL_BUYER_CONNECTION_HPP
