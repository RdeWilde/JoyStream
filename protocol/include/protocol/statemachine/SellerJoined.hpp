/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_SELLERJOINED_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_SELLERJOINED_HPP

namespace joystream {
namespace protocol {
namespace statemachine {

    class Buy;

    class SellerJoined {

    public:

        enum class State {

            // simple state (inital state):
            waiting_for_contract,

            // simple state:
            ready_to_request_piece,

            // simple state:
            piece_requested,

            // simple state:
            validating_piece,

            // pseudo state: termination
            terminated
        };

        SellerJoined(Buy * context);

    private:

        State _state;

        Buy * _context;
    };
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_SELLERJOINED_HPP
