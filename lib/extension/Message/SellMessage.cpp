#include "SellMessage.hpp"
#include "MessageType.hpp"

#include <QDataStream>

SellMessage::SellMessage(quint32 minPrice, QDateTime minLock)
    : _minPrice(minPrice)
    , _minLock(minLock) {
}

SellMessage::SellMessage(QDataStream & stream) {

    uint minLockTime_t;

    stream >> _minPrice >> minLockTime_t;

    _minLock.fromTime_t(minLockTime_t);
}

MessageType SellMessage::messageType() const {
    return MessageType::sell;
}

quint32 SellMessage::length() const {
    return sizeof(_minPrice) + sizeof(uint);
}

void SellMessage::write(QDataStream & stream) const {
    stream << _minPrice << _minLock.toTime_t();
}

quint32 SellMessage::minPrice() const {
    return _minPrice;
}

QDateTime SellMessage::minLock() const {
    return _minLock;
}
