/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#include <protocol_statemachine/event/SellModeStarted.hpp>

namespace joystream {
namespace protocol_statemachine {
namespace event {

    SellModeStarted::SellModeStarted(const joystream::protocol_wire::SellerTerms & terms)
        : _terms(terms) {
    }

    joystream::protocol_wire::SellerTerms SellModeStarted::terms() const {
        return _terms;
    }

    void SellModeStarted::setTerms(const joystream::protocol_wire::SellerTerms & terms) {
        _terms = terms;
    }

}
}
}
