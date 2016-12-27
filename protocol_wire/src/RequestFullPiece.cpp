/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol_wire/RequestFullPiece.hpp>
#include <protocol_wire/MessageType.hpp>

namespace joystream {
namespace protocol_wire {

    RequestFullPiece::RequestFullPiece()
        : RequestFullPiece(0) {
    }

    RequestFullPiece::RequestFullPiece(int pieceIndex)
        : _pieceIndex(pieceIndex) {
    }

    RequestFullPiece::RequestFullPiece(std::istream & stream) {
        stream >> _pieceIndex;
    }

    bool RequestFullPiece::operator==(const RequestFullPiece & rhs) const {
        return _pieceIndex == rhs.pieceIndex();
    }

    MessageType RequestFullPiece::messageType() const {
        return MessageType::request_full_piece;
    }

    uint32_t RequestFullPiece::length() const {
        return sizeof(_pieceIndex);
    }

    void RequestFullPiece::write(std::ostream & stream) const {
        stream << _pieceIndex;
    }

    int RequestFullPiece::pieceIndex() const {
        return _pieceIndex;
    }

    void RequestFullPiece::setPieceIndex(int pieceIndex) {
        _pieceIndex = pieceIndex;
    }
}
}
