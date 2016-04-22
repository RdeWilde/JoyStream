/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_DETAIL_PIECE_HPP
#define JOYSTREAM_PROTOCOLSESSION_DETAIL_PIECE_HPP

#include <string>

namespace joystream {
namespace protocol_session {
namespace detail {

    template <class ConnectionIdType>
    class Piece {

    public:

        enum class State {

            // We do not have piece, and it has given assignment status
            unassigned,

            assigned_to_peer_for_download,

            // We do have piece
            downloaded
        };

        Piece();
        Piece(int index, State, const ConnectionIdType &, int length);

        // Piece is assigned to connection with given id
        void assigned(const ConnectionIdType &);

        // Piece has been downloaded
        void downloaded();

        // Piece is no longer assigned to a connection
        void deAssign();

        // Getters
        int index() const;

        State state() const;

        ConnectionIdType connectionId() const;

        int length() const;

    private:

        // Index of piece
        int _index;

        // Piece state
        State _state;

        // Id of connectionto which piece is assigned, when _state == State::assigned_to_peer_for_download
        ConnectionIdType _connectionId;

        // Byte length of piece (should be the same for all but last piece)
        int _length;
    };

}
}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol_session/../../src/detail/Piece.cpp>

#endif // JOYSTREAM_PROTOCOLSESSION_DETAIL_PIECE_HPP
