/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 21 2016
 */

#include <protocol_statemachine/detail/InitializeSelling.hpp>

using namespace joystream::protocol_statemachine::detail;

InitializeSelling::InitializeSelling(const joystream::protocol_wire::SellerTerms & terms)
    : _terms(terms) {
}

joystream::protocol_wire::SellerTerms InitializeSelling::terms() const {
    return _terms;
}
