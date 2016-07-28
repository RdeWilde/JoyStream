/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#ifndef JOYSTREAM_CORE_CONNECTION_HPP
#define JOYSTREAM_CORE_CONNECTION_HPP

#include <libtorrent/socket_io.hpp>
#include <protocol_session/protocol_session.hpp>

#include <QObject>

namespace joystream {
namespace core {

class Connection : public QObject {

    Q_OBJECT

private:

    Connection(const protocol_session::status::Connection<libtorrent::tcp::endpoint> &);

public:

    libtorrent::tcp::endpoint connectionId() const noexcept;

    protocol_statemachine::AnnouncedModeAndTerms announcedModeAndTermsFromPeer() const noexcept;

signals:

    void announcedModeAndTermsFromPeerChanged(const protocol_statemachine::AnnouncedModeAndTerms &);

private:

    friend class Session;

    void update(const protocol_session::status::Connection<libtorrent::tcp::endpoint> &);

    // Connection id
    libtorrent::tcp::endpoint _connectionId;

    //// Peer state

    protocol_statemachine::AnnouncedModeAndTerms _announcedModeAndTermsFromPeer;

    //// Buyer Client state

    // Payor side of payment channel interaction
    //paymentchannel::Payor _payor;

    //// Seller Client state

    // Payee side of payment channel interaction
    //paymentchannel::Payee _payee;

    //// Buyer

    // Indexes of valid piecesm, in the order they were downloaded
    // NB: The reason this is not in Seller, is because
    // any peer can potentially provide valid pieces
    //std::queue<uint32_t> _downloadedValidPieces;

    //// Selling

};

}
}

#endif // JOYSTREAM_CORE_CONNECTION_HPP
