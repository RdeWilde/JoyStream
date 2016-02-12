/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SELLER_CONNECTION_HPP
#define JOYSTREAM_PROTOCOL_SELLER_CONNECTION_HPP

#include <protocol/Connection.hpp>
#include <protocol/SellerClientState.hpp>
#include <protocol/SellerPeerState.hpp>
#include <paymentchannel/Payee.hpp>

#include <queue>

namespace joystream {
namespace protocol {

    class SellerConnection : public Connection {

    public:

        SellerConnection();

        SellerConnection(const std::string & peerName,
                   PeerModeAnnounced lastModeAnnouncedByPeer,
                   const SendMessageCallbackHandler & sendMessageCallbackHandler,
                   SellerClientState sellClientState,
                   SellerPeerState sellPeerState,
                   const joystream::paymentchannel::Payee & payee,
                   const std::queue<uint32_t> & fullPiecesSent);

        // Create a (seller) connection which is fresh, i.e. has never had any message transmitted except seller mode message
        static SellerConnection sellMessageJustSent(const Connection & c,
                                                    quint32 lockTime,
                                                    quint64 price,
                                                    const Coin::KeyPair & payeeContractKeys,
                                                    const Coin::KeyPair & payeePaymentKeys);

        // Getters
        SellerClientState clientState() const;

        SellerPeerState peerState() const;

        joystream::paymentchannel::Payee payee() const;

        std::queue<uint32_t> fullPiecesSent() const;

    private:

        // State of client (us) on this connection
        SellerClientState _clientState;

        // State of peer on this connection
        SellerPeerState _peerState;

        // Payee side of payment channel
        joystream::paymentchannel::Payee _payee;

        // Index of each piece sent, in the order that it was sent
        std::queue<uint32_t> _fullPiecesSent;

    };

}
}

#endif // JOYSTREAM_PROTOCOL_SELLER_CONNECTION_HPP
