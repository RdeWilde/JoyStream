/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 17 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_SELLER_HPP
#define JOYSTREAM_PROTOCOLSESSION_SELLER_HPP

#include <string>
#include <cstdlib>

namespace joystream {
namespace protocol_session {
namespace detail {

    template <class ConnectionIdType>
    class Connection;

    template <class ConnectionIdType>
    class Seller {

        enum class State {

            // Not been assigned piece
            waiting_to_be_assigned_piece,

            // We have requested a full piece, and we are waiting for it to arrive
            waiting_for_full_piece,

            // We are waiting for libtorrent to fire on_piece_pass() or on_piece_failed()
            // on a full piece which was recently received
            waiting_for_piece_validation_and_storage,

            // Peer is no longer connected
            gone
        };

    public:

        Seller();

        Seller(State , Connection<ConnectionIdType> * , uint32_t);

        // When seller is not assigned a piece, this routine
        // can be used to assing piece to seller and send request to peer
        void requestPiece(int i);

        // Getters
        State state() const;

        Connection<ConnectionIdType> * connection() const;

        int indexOfAssignedPiece() const;

    private:

        // State of this seller
        State _state;

        // Connection identifier for seller
        Connection<ConnectionIdType> * _connection;

        // When _state == State::waiting_for_full_piece,
        // waiting_for_piece_validation_and_storage
        int _indexOfAssignedPiece;

        // When last piece was assigned to this seller.
        // Is used to identify slow seller.
        time_t _whenLastPieceAssigned;
    };

}
}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol_session/../../src/detail/Seller.cpp>

#endif // JOYSTREAM_PROTOCOLSESSION_SELLER_HPP
