/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol_wire/FullPiece.hpp>
#include <protocol_wire/MessageType.hpp>

#include <QDataStream>

namespace joystream {
namespace protocol_wire {

    FullPiece::FullPiece(const PieceData & pieceData)
        : _pieceData(pieceData) {
    }

    FullPiece::FullPiece(QDataStream & stream, int length)
        : _pieceData(stream, length) {
    }

    MessageType FullPiece::messageType() const {
        return MessageType::full_piece;
    }

    quint32 FullPiece::length() const {
        return _pieceData.length();
    }

    void FullPiece::write(QDataStream & stream) const {
        _pieceData.write(stream);
    }

    PieceData FullPiece::pieceData() const {
        return _pieceData;
    }
}
}
