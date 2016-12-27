/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol_wire/Buy.hpp>
#include <protocol_wire/MessageType.hpp>

namespace joystream {
namespace protocol_wire {

    Buy::Buy() {
    }

    Buy::Buy(const BuyerTerms & terms)
        : _terms(terms) {
    }

    bool Buy::operator==(const Buy & o) const {
        return _terms == o.terms();
    }

    Buy::Buy(std::istream & stream) {
        stream >> _terms;
    }

    MessageType Buy::messageType() const {
        return MessageType::buy;
    }

    uint32_t Buy::length() const {
        return _terms.length();
    }

    void Buy::write(std::ostream & stream) const {
        stream << _terms;
    }

    BuyerTerms Buy::terms() const {
        return _terms;
    }

    void Buy::setTerms(const BuyerTerms & terms) {
        _terms = terms;
    }

}
}
