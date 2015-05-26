#include "Piece.hpp"

Piece::Piece(int index, int length, const boost::shared_array<char> & data)
    : _index(index)
    , _length(length)
    , _data(data) {
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

boost::shared_array<char> Piece::data() const {
    return _data;
}

void Piece::setData(const boost::shared_array<char> &data) {
    _data = data;
}
