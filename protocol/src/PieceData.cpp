/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#include <protocol/PieceData.hpp>

namespace joystream {
namespace protocol {

    PieceData::PieceData(const boost::shared_array<char> & piece, int length)
        : _piece(piece)
        , _length(length) {
    }

    boost::shared_array<char> PieceData::piece() const {
        return _piece;
    }

    int PieceData::length() const {
        return _length;
    }
}
}
