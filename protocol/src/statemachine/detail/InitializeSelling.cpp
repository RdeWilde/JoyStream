/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 21 2016
 */

#include <protocol/statemachine/detail/InitializeSelling.hpp>

using namespace joystream::protocol::statemachine::detail;

InitializeSelling::InitializeSelling(const SellerTerms & terms)
    : _terms(terms) {
}

joystream::protocol::SellerTerms InitializeSelling::terms() const {
    return _terms;
}
