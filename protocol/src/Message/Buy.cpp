/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Message/Buy.hpp>
#include <core/extension/Message/MessageType.hpp>

#include <QDataStream>

Buy::Buy() {

}

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
    return sizeof(_maxPrice) + sizeof(_maxLock) + sizeof(_minSellers);
}

void Buy::write(QDataStream & stream) const {
    stream << _maxPrice << _maxLock << _minSellers;
}

quint32 Buy::maxPrice() const {
    return _maxPrice;
}

void Buy::setMaxPrice(quint32 maxPrice) {
    _maxPrice = maxPrice;
}

quint32 Buy::maxLock() const {
    return _maxLock;
}

void Buy::setMaxLock(quint32 maxLock){
    _maxLock = maxLock;
}

quint32 Buy::minSellers() const {
    return _minSellers;
}

void Buy::setMinSellers(quint32 minSellers) {
    _minSellers = minSellers;
}
