/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 17 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SELLER_HPP
#define JOYSTREAM_PROTOCOL_SELLER_HPP

#include <string>
#include <cstdlib>

namespace joystream {
namespace protocol {

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

        Seller(State , const ConnectionIdType &, uint32_t);

        // Getters and setters
        State state() const;
        void setState(State);

        ConnectionIdType connectionId() const;
        void setConnectionId(const ConnectionIdType &connectionId);

        uint32_t indexOfAssignedPiece() const;
        void setIndexOfAssignedPiece(const uint32_t &indexOfAssignedPiece);

    private:

        // State of this seller
        State _state;

        // Connection identifier for seller
        ConnectionIdType _connectionId;

        // When _state == State::waiting_for_full_piece,
        // waiting_for_piece_validation_and_storage
        uint32_t _indexOfAssignedPiece;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_SELLER_HPP
