#include "Sell.hpp"
#include "MessageType.hpp"

#include <QDataStream>

Sell::Sell(quint32 minPrice, QDateTime minLock)
    : _minPrice(minPrice)
    , _minLock(minLock) {
}

Sell::Sell(QDataStream & stream) {

    uint minLockTime_t;

    stream >> _minPrice >> minLockTime_t;

    _minLock.fromTime_t(minLockTime_t);
}

MessageType Sell::messageType() const {
    return MessageType::sell;
}

quint32 Sell::length() const {
    return sizeof(_minPrice) + sizeof(uint);
}

void Sell::write(QDataStream & stream) const {
    stream << _minPrice << _minLock.toTime_t();
}

quint32 Sell::minPrice() const {
    return _minPrice;
}

QDateTime Sell::minLock() const {
    return _minLock;
}
