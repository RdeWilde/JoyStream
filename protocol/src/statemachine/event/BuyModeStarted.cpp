/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#include <protocol/statemachine/event/BuyModeStarted.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {
namespace event {

    BuyModeStarted::BuyModeStarted(const joystream::wire::BuyerTerms & terms)
        : _terms(terms) {
    }

    joystream::wire::BuyerTerms BuyModeStarted::terms() const {
        return _terms;
    }

    void BuyModeStarted::setTerms(const joystream::wire::BuyerTerms & terms) {
        _terms = terms;
    }

}
}
}
}
