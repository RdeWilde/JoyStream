#include "BuyMessage.hpp"
#include "MessageType.hpp"

#include <QDataStream>

BuyMessage::BuyMessage(quint32 maxPrice, QDateTime maxLock)
    : _maxPrice(maxPrice)
    , _maxLock(maxLock) {
}

BuyMessage::BuyMessage(QDataStream & stream) {

    uint maxLockTime_t;

    stream >> _maxPrice >> maxLockTime_t;

    _maxLock.fromTime_t(maxLockTime_t);
}

MessageType BuyMessage::messageType() const {
    return MessageType::buy;
}

quint32 BuyMessage::length() const {
    return sizeof(_maxPrice) + sizeof(uint);
}

void BuyMessage::write(QDataStream & stream) const {
    stream << _maxPrice << _maxLock.toTime_t();
}

quint32 BuyMessage::maxPrice() const {
    return _maxPrice;
}

QDateTime BuyMessage::maxLock() const {
    return _maxLock;
}
