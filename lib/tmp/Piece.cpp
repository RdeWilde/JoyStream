#include "Piece.hpp"

Piece::Piece()
    : _hasValidData(false) {
}

Piece::Piece(int index, int length, bool hasValidData, const boost::shared_array<char> & data)
    : _index(index)
    , _length(length)
    , _hasValidData(hasValidData)
    , _data(data) {
}

Piece::Piece(const Piece & piece)
    : Piece(piece.index(), piece.length(), piece.hasValidData(), piece.data()) {
}

int Piece::index() const {
    return _index;
}

void Piece::setIndex(int index) {
    _index = index;
}

int Piece::length() const {
    return _length;
}

void Piece::setLength(int length) {
    _length = length;
}

bool Piece::hasValidData() const {
    return _hasValidData;
}

void Piece::setHasValidData(bool hasValidData) {
    _hasValidData = hasValidData;
}

boost::shared_array<char> Piece::data() const {
    return _data;
}

void Piece::setData(const boost::shared_array<char> &data) {
    _data = data;
}
