/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol/wire/Sell.hpp>
#include <protocol/wire/MessageType.hpp>

#include <QDataStream>

namespace joystream {
namespace protocol {
namespace wire {

    Sell::Sell() {
    }

    Sell::Sell(const joystream::protocol::sell::Terms & terms)
        : _terms(terms) {
    }

    Sell::Sell(QDataStream & stream) {
        stream >> _terms._minPrice >> _terms._minLock >> _terms._maxSellers;
    }

    MessageType Sell::messageType() const {
        return MessageType::sell;
    }

    quint32 Sell::length() const {
        return sizeof(_terms._minPrice) + sizeof(_terms._minLock) + sizeof(_terms._maxSellers);
    }

    void Sell::write(QDataStream & stream) const {
        stream << _terms._minPrice << _terms._minLock << _terms._maxSellers;
    }

    joystream::protocol::sell::Terms Sell::terms() const  {
        return _terms;
    }

    void Sell::setTerms(const joystream::protocol::sell::Terms & terms) {
        _terms = terms;
    }
}
}
}
