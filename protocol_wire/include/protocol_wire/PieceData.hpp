/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#ifndef JOYSTREAM_WIRE_PIECEDATA_HPP
#define JOYSTREAM_WIRE_PIECEDATA_HPP

#include <iostream>
#include <boost/shared_array.hpp>

namespace joystream {
namespace protocol_wire {

    class PieceData {

    public:

        PieceData();

        PieceData(const boost::shared_array<char> &, unsigned int);

        static PieceData fromHex(const char *);

        bool operator==(const PieceData &) const;

        boost::shared_array<char> piece() const;

        unsigned int length() const;

    private:

        // Piece data;
        boost::shared_array<char> _piece;

        // Byte length of piece
        unsigned int _length;
    };

}
}

#endif // JOYSTREAM_WIRE_PIECEDATA_HPP
