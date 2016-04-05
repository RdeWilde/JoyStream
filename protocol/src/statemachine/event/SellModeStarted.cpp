/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#include <protocol/statemachine/event/SellModeStarted.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {
namespace event {

    SellModeStarted::SellModeStarted(const joystream::wire::SellerTerms & terms)
        : _terms(terms) {
    }

    joystream::wire::SellerTerms SellModeStarted::terms() const {
        return _terms;
    }

    void SellModeStarted::setTerms(const joystream::wire::SellerTerms & terms) {
        _terms = terms;
    }

}
}
}
}
