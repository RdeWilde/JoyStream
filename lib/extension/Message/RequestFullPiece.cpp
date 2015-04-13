#include "RequestFullPiece.hpp"

#include "MessageType.hpp"

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
