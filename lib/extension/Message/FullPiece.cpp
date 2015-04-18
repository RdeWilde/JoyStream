#include "FullPiece.hpp"
#include "MessageType.hpp"

#include <QDataStream>

FullPiece::FullPiece() {
}

FullPiece::FullPiece(const QVector<char> & piece)
    : _piece(piece) {
}

FullPiece::FullPiece(QDataStream & stream, int lengthOfPiece)
    : _piece(lengthOfPiece) {

    // Try to fill vector buffer
    int result = stream.readRawData(_piece.data(), lengthOfPiece);

    // Check that we were able to read full piece
    if(result != lengthOfPiece)
        throw std::exception("Was unable to read full piece from stream.");
}

MessageType FullPiece::messageType() const {
    return MessageType::full_piece;
}

quint32 FullPiece::length() const {
    return _piece.size();
}

void FullPiece::write(QDataStream & stream) const {
    stream << _piece;
}

QVector<char> FullPiece::piece() const {
    return _piece;
}

void FullPiece::setPiece(const QVector<char> & piece) {
    _piece = piece;
}
