/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol_wire/FullPiece.hpp>
#include <protocol_wire/MessageType.hpp>

namespace joystream {
namespace protocol_wire {

    FullPiece::FullPiece() {
    }

    FullPiece::FullPiece(const PieceData & pieceData)
        : _pieceData(pieceData) {
    }

    FullPiece::FullPiece(std::istream & stream, int length)
        : _pieceData(stream, length) {
    }

    bool FullPiece::operator==(const FullPiece & rhs) const {
        return _pieceData == rhs.pieceData();
    }

    MessageType FullPiece::messageType() const {
        return MessageType::full_piece;
    }

    quint32 FullPiece::length() const {
        return _pieceData.length();
    }

    void FullPiece::write(std::ostream & stream) const {
        _pieceData.write(stream);
    }

    PieceData FullPiece::pieceData() const {
        return _pieceData;
    }
}
}
