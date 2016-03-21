/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 21 2016
 */

#include <protocol/statemachine/detail/InitializeBuying.hpp>

using namespace joystream::protocol::statemachine::detail;

InitializeBuying::InitializeBuying(const BuyerTerms & terms)
    : _terms(terms) {
}

joystream::protocol::BuyerTerms InitializeBuying::terms() const
{
    return _terms;
}
