#include "Buy.hpp"
#include "MessageType.hpp"

#include <QDataStream>

Buy::Buy(quint32 maxPrice, quint32 maxLock, quint32 minSellers)
    : _maxPrice(maxPrice)
    , _maxLock(maxLock)
    , _minSellers(minSellers){
}

Buy::Buy(QDataStream & stream) {

    stream >> _maxPrice >> _maxLock >> _minSellers;
}

MessageType Buy::messageType() const {
    return MessageType::buy;
}

quint32 Buy::length() const {
    return sizeof(_maxPrice) + sizeof(uint) + sizeof(quint32);
}

void Buy::write(QDataStream & stream) const {
    stream << _maxPrice << _maxLock << _minSellers;
}

quint32 Buy::maxPrice() const {
    return _maxPrice;
}

quint32 Buy::maxLock() const {
    return _maxLock;
}

quint32 Buy::minSellers() const {
    return _minSellers;
}
