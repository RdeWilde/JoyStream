#include "Buy.hpp"
#include "MessageType.hpp"

#include <QDataStream>

Buy::Buy(quint32 maxPrice, quint32 maxLock)
    : _maxPrice(maxPrice)
    , _maxLock(maxLock) {
}

Buy::Buy(QDataStream & stream) {

    stream >> _maxPrice >> _maxLock;
}

MessageType Buy::messageType() const {
    return MessageType::buy;
}

quint32 Buy::length() const {
    return sizeof(_maxPrice) + sizeof(uint);
}

void Buy::write(QDataStream & stream) const {
    stream << _maxPrice << _maxLock;
}

quint32 Buy::maxPrice() const {
    return _maxPrice;
}

quint32 Buy::maxLock() const {
    return _maxLock;
}
