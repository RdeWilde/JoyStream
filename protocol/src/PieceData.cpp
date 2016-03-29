/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#include <protocol/PieceData.hpp>

#include <QDataStream>

namespace joystream {
namespace protocol {

    PieceData::PieceData()
        : _length(0) {
    }

    PieceData::PieceData(const boost::shared_array<char> & piece, int length)
        : _piece(piece)
        , _length(length) {
    }

    PieceData::PieceData(QDataStream & stream, int length)
        : _piece(new char[length])
        , _length(length) {

        // Try to fill buffer
        int result = stream.readRawData(_piece.get(), _length);

        // Check that we were able to read full piece
        if(result != _length)
            throw std::runtime_error("Was unable to read full piece from stream.");
    }

    int PieceData::write(QDataStream & stream) const {
        return stream.writeRawData(_piece.get(), _length);
    }

    boost::shared_array<char> PieceData::piece() const {
        return _piece;
    }

    int PieceData::length() const {
        return _length;
    }
}
}
