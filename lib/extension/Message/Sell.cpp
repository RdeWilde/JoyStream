#include "Sell.hpp"
#include "MessageType.hpp"

#include <QDataStream>

Sell::Sell(quint32 minPrice, quint32 minLock, quint32 maxSellers)
    : _minPrice(minPrice)
    , _minLock(minLock)
    , _maxSellers(maxSellers) {
}

Sell::Sell(QDataStream & stream) {
    stream >> _minPrice >> _minLock >> _maxSellers;
}

MessageType Sell::messageType() const {
    return MessageType::sell;
}

quint32 Sell::length() const {
    return sizeof(_minPrice) + sizeof(_minLock) + sizeof(_maxSellers);
}

void Sell::write(QDataStream & stream) const {
    stream << _minPrice << _minLock << _maxSellers;
}

quint32 Sell::minPrice() const {
    return _minPrice;
}

void Sell::setMinPrice(quint32 minPrice) {
    _minPrice = minPrice;
}

quint32 Sell::minLock() const {
    return _minLock;
}

void Sell::setMinLock(quint32 minLock) {
    _minLock = minLock;
}

quint32 Sell::maxSellers() const {
    return _maxSellers;
}

void Sell::setMaxSellers(quint32 maxSellers) {
    _maxSellers = maxSellers;
}
