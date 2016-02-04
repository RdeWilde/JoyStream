/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PROTOCOL_REQUEST_FULL_PIECE_HPP
#define PROTOCOL_REQUEST_FULL_PIECE_HPP

#include <protocol/wire/ExtendedMessagePayload.hpp>

namespace joystream {
namespace protocol {
namespace wire {

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
}

#endif // PROTOCOL_REQUEST_FULL_PIECE_HPP
