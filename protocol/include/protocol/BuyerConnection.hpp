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

        BuyerConnection(const Connection & connection,
                        BuyerClientState clientState,
                        BuyerPeerState peerState,
                        const std::queue<uint32_t> & downloadedValidPieces);

        // Create a (buyer) connection which is fresh, i.e. has never had any message transmitted except buyer mode message
        static BuyerConnection buyMessageJustSent(const Connection & connection);

        // Getters
        BuyerClientState clientState() const;

        BuyerPeerState peerState() const;

        std::queue<uint32_t> downloadedValidPieces() const;

    private:

        // State of client (us) on this connection
        BuyerClientState _clientState;

        // Point in time when last invite sent
        time_t _whenLastInviteSent;

        // State of peer on this connection
        BuyerPeerState _peerState;

        // Indexes of valid piecesm, in the order they were downloaded
        // NB: The reason this is not in Seller, is because
        // any peer can potentially provide valid pieces
        std::queue<uint32_t> _downloadedValidPieces;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_BUYER_CONNECTION_HPP
