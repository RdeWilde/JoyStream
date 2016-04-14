/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_PIECE_HPP
#define JOYSTREAM_PROTOCOLSESSION_PIECE_HPP

#include <string>

namespace joystream {
namespace protocol_session {

    template <class ConnectionIdType>
    class Piece {

    public:

        enum class State {

            // We do not have piece, and it has given assignment status
            unassigned,
            assigned_to_peer_for_download,

            // We do have piece
            fully_downloaded_and_valid,
        };

        Piece();
        Piece(int index, State, const ConnectionIdType &, int length);

        // Getters and setters
        int index() const;
        void setIndex(int);

        State state() const;
        void setState(State);

        ConnectionIdType id() const;
        void setId(const ConnectionIdType &);

        int length() const;
        void setLength(int);

    private:

        // Index of piece
        int _index;

        // Piece state
        State _state;

        // Id of connectionto which piece is assigned, when _state == State::assigned_to_peer_for_download
        ConnectionIdType _id;

        // Byte length of piece (should be the same for all but last piece)
        int _length;
    };
}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol_session/../../src/Piece.cpp>

#endif // JOYSTREAM_PROTOCOLSESSION_PIECE_HPP
