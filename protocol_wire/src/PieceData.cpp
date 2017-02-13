/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#include <protocol_wire/PieceData.hpp>

#include <cstring>
#include <sstream>
#include <iostream>     // std::cout, std::dec, std::hex, std::oct

namespace joystream {
namespace protocol_wire {

    PieceData::PieceData()
        : _length(0) {
    }

    PieceData::PieceData(const boost::shared_array<char> & piece, unsigned int length)
        : _piece(piece)
        , _length(length) {
    }

    PieceData PieceData::fromHex(const char * src) {

        // Get length of string encoded data
        unsigned int length = strlen(src);

        // Allocate managed array
        boost::shared_array<char> piece(new char[length]);

        // Decode hex string, and copy into array
        std::stringstream s;
        s << std::hex << src;

        for(uint i = 0; i < length;i++)
            s >> piece.get()[i];

        return PieceData(piece, length);
    }

    bool PieceData::operator==(const PieceData & rhs) const {

        // Compare lengths
        if(_length != rhs.length())
            return false;

        // Compare raw data
        const boost::shared_array<char> & rhsPiece = rhs.piece();
        for(unsigned int i = 0;i < _length;i++) {

            if(_piece[i] != rhsPiece[i])
                return false;
        }

        return true;
    }

    boost::shared_array<char> PieceData::piece() const {
        return _piece;
    }

    unsigned int PieceData::length() const {
        return _length;
    }
}
}
