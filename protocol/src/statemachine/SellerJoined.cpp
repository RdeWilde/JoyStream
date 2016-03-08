/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#include <protocol/statemachine/SellerJoined.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    SellerJoined::SellerJoined(Buy * context)
        : _context(context)
        , _state(State::waiting_for_contract){

    }

}
}
}
