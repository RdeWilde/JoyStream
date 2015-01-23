#include "OutputPoint.hpp"

OutputPoint::OutputPoint() {
}

OutputPoint::OutputPoint(const OutputPoint& outputPoint) {
    _hash = outputPoint.hash();
    _index = outputPoint.index();
}

OutputPoint & operator=(const OutputPoint& outputPoint) {
    _hash = outputPoint.hash();
    _index = outputPoint.index();
}

OutputPoint::OutputPoint(const Hash & hash, quint32 index)
    : _hash(hash)
    , _index(index) {
}

Hash OutputPoint::hash() const {
    return _hash;
}

void OutputPoint::setHash(const Hash& hash){
    _hash = hash;
}

quint32 OutputPoint::index() const {
    return _index;
}

void OutputPoint::setIndex(const quint32& index) {
    _index = index;
}


