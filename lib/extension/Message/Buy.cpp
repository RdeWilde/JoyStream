#include "Buy.hpp"
#include "MessageType.hpp"

#include <QDataStream>

Buy::Buy(quint32 maxPrice, QDateTime maxLock)
    : _maxPrice(maxPrice)
    , _maxLock(maxLock) {
}

Buy::Buy(QDataStream & stream) {

    uint maxLockTime_t;

    stream >> _maxPrice >> maxLockTime_t;

    _maxLock.fromTime_t(maxLockTime_t);
}

MessageType Buy::messageType() const {
    return MessageType::buy;
}

quint32 Buy::length() const {
    return sizeof(_maxPrice) + sizeof(uint);
}

void Buy::write(QDataStream & stream) const {
    stream << _maxPrice << _maxLock.toTime_t();
}

quint32 Buy::maxPrice() const {
    return _maxPrice;
}

QDateTime Buy::maxLock() const {
    return _maxLock;
}
