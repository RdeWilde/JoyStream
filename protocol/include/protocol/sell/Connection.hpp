/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SELL_CONNECTION_HPP
#define JOYSTREAM_PROTOCOL_SELL_CONNECTION_HPP

#include <protocol/Connection.hpp>
#include <protocol/sell/ClientState.hpp>
#include <protocol/sell/PeerState.hpp>
#include <paymentchannel/Payee.hpp>

#include <list>

namespace joystream {
namespace protocol {
namespace sell {

    class Connection : public joystream::protocol::Connection {

    public:

        Connection(const std::string & peerName,
                   PeerModeAnnounced lastModeAnnouncedByPeer,
                   const SendMessageCallbackHandler & sendMessageCallbackHandler,
                   ClientState sellClientState,
                   PeerState sellPeerState,
                   const joystream::paymentchannel::Payee & payee,
                   const std::list<uint32_t> & fullPiecesSent);

        // Getters
        ClientState clientState() const;

        PeerState peerState() const;

        joystream::paymentchannel::Payee payee() const;

        std::list<uint32_t> fullPiecesSent() const;

    private:

        // State of client (us) on this connection
        ClientState _clientState;

        // State of peer on this connection
        PeerState _peerState;

        // Payee side of payment channel
        joystream::paymentchannel::Payee _payee;

        // Index of each piece sent, in the order that it was sent
        std::list<uint32_t> _fullPiecesSent;

    };
}
}
}
#endif // JOYSTREAM_PROTOCOL_SELL_CONNECTION_HPP
