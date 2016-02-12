/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol/wire/Buy.hpp>
#include <protocol/wire/MessageType.hpp>

#include <QDataStream>

namespace joystream {
namespace protocol {
namespace wire {

    Buy::Buy() {
    }

    Buy::Buy(const joystream::protocol::buy::Terms & terms)
        : _terms(terms) {
    }

    Buy::Buy(QDataStream & stream) {
        stream >> _terms._maxPrice >> _terms._maxLock >> _terms._numberOfSellers;
    }

    MessageType Buy::messageType() const {
        return MessageType::buy;
    }

    quint32 Buy::length() const {
        return sizeof(_terms._maxPrice) + sizeof(_terms._maxLock) + sizeof(_terms._numberOfSellers);
    }

    void Buy::write(QDataStream & stream) const {
        stream << _terms._maxPrice << _terms._maxLock << _terms._numberOfSellers;
    }
    
    joystream::protocol::buy::Terms Buy::terms() const {
        return _terms;
    }
    
    void Buy::setTerms(const joystream::protocol::buy::Terms & terms) {
        _terms = terms;
    }

}
}
}
