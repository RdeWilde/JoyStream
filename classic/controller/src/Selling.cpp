/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 24 2016
 */

#include <controller/Selling.hpp>

namespace joystream {
namespace classic {
namespace controller {

Selling::Selling(core::Selling * selling)
    : _selling(selling) {

}

void Selling::setPolicy(const protocol_session::SellingPolicy &) {

}

void Selling::setTerms(const protocol_wire::SellerTerms &) {

}

}
}
}
