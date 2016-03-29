/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_PROTOCOL_WIRE_FULL_PIECE_HPP
#define JOYSTREAM_PROTOCOL_WIRE_FULL_PIECE_HPP

#include <protocol/wire/ExtendedMessagePayload.hpp>
#include <protocol/PieceData.hpp>

namespace joystream {
namespace protocol {
namespace wire {

    class FullPiece : public ExtendedMessagePayload {

    public:

        // Constructor from members
        FullPiece(const PieceData &);

        // Constructor based on raw payload
        FullPiece(QDataStream & stream, int length);

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;
        virtual quint32 length() const;
        virtual void write(QDataStream & stream) const;

        // Getters
        PieceData pieceData() const;

    private:

        // Piece data;
        PieceData _pieceData;
    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_WIRE_FULL_PIECE_HPP
