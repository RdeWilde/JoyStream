/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Message/FullPiece.hpp>
#include <core/extension/Message/MessageType.hpp>

#include <QDataStream>

FullPiece::FullPiece()
    :  _length(0) { //_piece(0)
}

FullPiece::FullPiece(const boost::shared_array<char> & piece, int length)
    : _piece(piece)
    , _length(length) {
}

FullPiece::FullPiece(QDataStream & stream, int length)
    : _piece(new char[length])
    , _length(length) {

    // Try to fill buffer
    int result = stream.readRawData(_piece.get(), _length);

    // Check that we were able to read full piece
    if(result != _length)
        throw std::runtime_error("Was unable to read full piece from stream.");
}

MessageType FullPiece::messageType() const {
    return MessageType::full_piece;
}

quint32 FullPiece::length() const {
    return _length;
}

void FullPiece::write(QDataStream & stream) const {
    stream.writeRawData(_piece.get(), _length);
}

boost::shared_array<char> FullPiece::piece() const {
    return _piece;
}

void FullPiece::setPiece(const boost::shared_array<char> & piece) {
    _piece = piece;
}

void FullPiece::setLength(int length) {
    _length = length;
}
