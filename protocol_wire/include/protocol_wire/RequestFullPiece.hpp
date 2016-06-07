/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_WIRE_REQUEST_FULL_PIECE_HPP
#define JOYSTREAM_WIRE_REQUEST_FULL_PIECE_HPP

#include <protocol_wire/ExtendedMessagePayload.hpp>

namespace joystream {
namespace protocol_wire {

    class RequestFullPiece : public ExtendedMessagePayload {

    public:

        // Default constructor
        RequestFullPiece();

        // Constructor based on members
        RequestFullPiece(int pieceIndex);

        // Constructor based on raw payload
        // NB: Substitute with static factory in future, so that you cannot create stale
        // payload objects if there is an error in the reading from stream
        RequestFullPiece(QDataStream & stream);

        virtual ~RequestFullPiece() {}

        bool operator==(const RequestFullPiece &) const;

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;
        virtual quint32 length() const;
        virtual void write(QDataStream & stream) const;

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
