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

    class Piece {

    public:

        enum class State {

            // We do not have piece, and it has given assignment status
            unassigned,
            assigned_to_peer_for_download,

            // We do have piece
            fully_downloaded_and_valid,
        };

        // Default constructor
        Piece();

        // Constructors based on members
        //Piece(int index, int length, int numberOfBlocks, State state, BuyerPeerPlugin * peerPlugin);

        // Constructors based on members
        Piece(int index, State state, const std::string & nameOfConnectionAssignedThisPiece);

        // Getters and setters
        int index() const;
        void setIndex(int index);

        State state() const;
        void setState(State state);

        std::string nameOfConnectionAssignedThisPiece() const;
        void setNameOfConnectionAssignedThisPiece(const std::string & nameOfConnectionAssignedThisPiece);

    private:

        // Index of piece
        int _index;

        // Byte length of piece (should be the same for all but last piece)
        int _length;

        // Piece state
        State _state;

        // Peer plugin assigned to this piece: Do we actually need this
        std::string _nameOfConnectionAssignedThisPiece;
    };

}
}

#endif // JOYSTREAM_PROTOCOLSESSION_PIECE_HPP
