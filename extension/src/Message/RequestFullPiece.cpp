/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Message/RequestFullPiece.hpp>
#include <core/extension/Message/MessageType.hpp>

#include <QDataStream>

RequestFullPiece::RequestFullPiece() {
}

RequestFullPiece::RequestFullPiece(int pieceIndex)
    : _pieceIndex(pieceIndex) {
}

RequestFullPiece::RequestFullPiece(QDataStream & stream) {
    stream >> _pieceIndex;
}

MessageType RequestFullPiece::messageType() const {
    return MessageType::request_full_piece;
}

quint32 RequestFullPiece::length() const {
    return sizeof(_pieceIndex);
}

void RequestFullPiece::write(QDataStream & stream) const {
    stream << _pieceIndex;
}

int RequestFullPiece::pieceIndex() const {
    return _pieceIndex;
}

void RequestFullPiece::setPieceIndex(int pieceIndex) {
    _pieceIndex = pieceIndex;
}
