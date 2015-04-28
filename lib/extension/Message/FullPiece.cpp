#include "FullPiece.hpp"
#include "MessageType.hpp"

#include <QDataStream>

FullPiece::FullPiece()
    : _piece(0)
    , _length(0) {
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
        throw std::exception("Was unable to read full piece from stream.");
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
