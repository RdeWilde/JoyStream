#include "OutPoint.hpp"

#include <QJsonObject>

OutPoint::OutPoint() {
}

OutPoint::OutPoint(const OutPoint& outputPoint) {
    _hash = outputPoint.hash();
    _index = outputPoint.index();
}

OutPoint & OutPoint::operator=(const OutPoint& outputPoint) {
    _hash = outputPoint.hash();
    _index = outputPoint.index();

    return *this;
}

OutPoint::OutPoint(const Hash & hash, quint32 index)
    : _hash(hash)
    , _index(index) {
}

QJsonObject OutPoint::json() const {
    return QJsonObject {
        {"hash", _hash.toString()},
        {"index", static_cast<qint64>(_index)}
    };
}

Hash OutPoint::hash() const {
    return _hash;
}

void OutPoint::setHash(const Hash& hash){
    _hash = hash;
}

quint32 OutPoint::index() const {
    return _index;
}

void OutPoint::setIndex(const quint32& index) {
    _index = index;
}


