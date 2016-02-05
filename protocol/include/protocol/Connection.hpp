/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_CONNECTION_HPP
#define JOYSTREAM_PROTOCOL_CONNECTION_HPP

#include <protocol/Mode.hpp>
#include <protocol/PeerModeAnnounced.hpp>
#include <paymentchannel/Payee.hpp>

#include <protocol/sell/ClientState.hpp>
#include <protocol/sell/PeerState.hpp>
#include <protocol/buy/ClientState.hpp>
#include <protocol/buy/PeerState.hpp>

#include <set>
#include <functional>

namespace joystream {
namespace protocol {

    class Connection {

    public:

        typedef std::function<void(const wire::ExtendedMessagePayload *)> SendMessageCallbackHandler;
        typedef std::function<void(int)> ReadPieceCallbackHandler;

        // Name constructors corresponding to mode



        // Callbacks
        void pieceRead(int index);

    private:

        // Mode of client, i.e. last mode message sent
        Mode _clientMode;

        // Mode announced by peer, i.e. last mode message received
        PeerModeAnnounced _lastModeAnnouncedByPeer;

        // Write message callback
        //

        //////////////////
        /// BUYER MODE ///
        //////////////////

        // All state in this section is only relevant when in buy mode (_clientMode == Mode::buy)

        // State of client (us) on this connection
        buy::ClientState _buyClientState;

        // State of peer on this connection
        buy::PeerState _buyPeerState;

        // Index of a piece assigned to this peer, only valid if
        // _clientState == ClientState::waiting_for_full_piece or
        // ClientState::waiting_for_libtorrent_to_validate_piece
        //int _indexOfAssignedPiece;

        // Indexes of valid piecesm, in the order they were downloaded
        std::list<uint32_t> _downloadedValidPieces;

        ///////////////////
        /// SELLER MODE ///
        ///////////////////

        // All state in this section is only relevant when in sell mode (_clientMode == Mode::sell)

        // State of client (us) on this connection
        sell::ClientState _sellClientState;

        // State of peer on this connection
        sell::PeerState _sellPeerState;

        // Payee side of payment channel
        joystream::paymentchannel::Payee _payee;

        // Index of each piece sent, in the order that it was sent
        std::list<uint32_t> _fullPiecesSent;
    };
}
}

#endif // JOYSTREAM_PROTOCOL_CONNECTION_HPP
