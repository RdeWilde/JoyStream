#include "Sell.hpp"
#include "MessageType.hpp"

#include <QDataStream>

Sell::Sell(quint32 minPrice, quint32 minLock)
    : _minPrice(minPrice)
    , _minLock(minLock) {
}

Sell::Sell(QDataStream & stream) {
    stream >> _minPrice >> _minLock;
}

MessageType Sell::messageType() const {
    return MessageType::sell;
}

quint32 Sell::length() const {
    return sizeof(_minPrice) + sizeof(quint32);
}

void Sell::write(QDataStream & stream) const {
    stream << _minPrice << _minLock;
}

quint32 Sell::minPrice() const {
    return _minPrice;
}

quint32 Sell::minLock() const {
    return _minLock;
}
