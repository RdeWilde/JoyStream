/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_CONNECTION_HPP
#define JOYSTREAM_PROTOCOL_CONNECTION_HPP

#include <protocol/Mode.hpp>
#include <protocol/ModeAnnounced.hpp>
#include <protocol/BuyModeConnectionState.hpp>
#include <protocol/SellModeConnectionState.hpp>
#include <paymentchannel/Payee.hpp>

#include <set>

namespace joystream {
namespace protocol {

    class Connection {

    public:

        //Connection();

    private:

        // Mode of client, i.e. last mode message sent
        Mode _clientMode;

        // Mode announced by peer, i.e. last mode message received
        ModeAnnounced _peerMode;

        // Write message callback

        //////////////////
        /// BUYER MODE ///
        //////////////////

        // State under buy mode
        BuyModeConnectionState _buyModeState;

        // Index of a piece assigned to this peer, only valid if
        // _clientState == ClientState::waiting_for_full_piece or ClientState::waiting_for_libtorrent_to_validate_piece
        //int _indexOfAssignedPiece;

        // Indexes of valid piecesm, in the order they were downloaded
        std::list<uint32_t> _downloadedValidPieces;

        ///////////////////
        /// SELLER MODE ///
        ///////////////////

        // State under sell mode
        SellModeConnectionState _sellModeState;

        // Payee side of payment channel
        joystream::paymentchannel::Payee _payee;

        // Index of each piece sent, in the order that it was sent
        std::list<uint32_t> _fullPiecesSent;
    };
}
}

#endif // JOYSTREAM_PROTOCOL_CONNECTION_HPP
