/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_WIRE_REQUEST_FULL_PIECE_HPP
#define JOYSTREAM_WIRE_REQUEST_FULL_PIECE_HPP

namespace joystream {
namespace protocol_wire {

    class RequestFullPiece  {

    public:

        // Default constructor
        RequestFullPiece();

        // Constructor based on members
        RequestFullPiece(int pieceIndex);

        bool operator==(const RequestFullPiece &) const;

        // Getters and setters
        int pieceIndex() const;
        void setPieceIndex(int pieceIndex);

    private:

        // Index of piece being requested
        int _pieceIndex;
    };

}
}

#endif // JOYSTREAM_WIRE_REQUEST_FULL_PIECE_HPP
