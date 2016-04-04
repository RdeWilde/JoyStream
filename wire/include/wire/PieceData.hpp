/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#ifndef JOYSTREAM_WIRE_PIECEDATA_HPP
#define JOYSTREAM_WIRE_PIECEDATA_HPP

#include <boost/shared_array.hpp>

class QDataStream;

namespace joystream {
namespace wire {

    class PieceData {

    public:

        PieceData();

        PieceData(const boost::shared_array<char> &, int);

        PieceData(QDataStream &, int);

        int write(QDataStream &) const;

        boost::shared_array<char> piece() const;

        int length() const;

    private:

        // Piece data;
        boost::shared_array<char> _piece;

        // Byte length of piece
        int _length;
    };

}
}

#endif // JOYSTREAM_WIRE_PIECEDATA_HPP
