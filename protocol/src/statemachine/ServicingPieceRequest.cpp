/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#include <protocol/statemachine/ServicingPieceRequest.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    ServicingPieceRequest::ServicingPieceRequest(Sell * context)
        : _context(context)
        , _state(State::piece_requested) {

    }

}
}
}
