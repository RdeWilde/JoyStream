/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_PROTOCOL_WIRE_FULLPIECE_HPP
#define JOYSTREAM_PROTOCOL_WIRE_FULLPIECE_HPP

#include <protocol_wire/ExtendedMessagePayload.hpp>
#include <protocol_wire/PieceData.hpp>

namespace joystream {
namespace protocol_wire {

    class FullPiece : public ExtendedMessagePayload {

    public:

        FullPiece();
        FullPiece(const PieceData &);

        virtual ~FullPiece() {}

        bool operator==(const FullPiece &) const;

        // Constructor based on raw payload
        FullPiece(std::istream & stream, int length);

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;
        virtual quint32 length() const;
        virtual void write(std::ostream & stream) const;

        // Getters
        PieceData pieceData() const;

    private:

        // Piece data;
        PieceData _pieceData;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_WIRE_FULLPIECE_HPP
