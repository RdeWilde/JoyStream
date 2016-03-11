/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_BUY_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_BUY_HPP

#include <protocol/statemachine/Active.hpp>
//#include <protocol/statemachine/SellerJoined.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class Buy : public sc::simple_state<Buy, Active> {

    };

    /**
    class Buy {

    public:

        enum class State {

            // simple state (initial state):
            ready_to_invite_seller,

            // simple state:
            waiting_for_seller_to_join,

            // complex state:
            seller_joined
        };

        Buy(CBStateMachine * context);

    private:

        CBStateMachine * _context;

        State _state;

        // Composite state
        SellerJoined _sellerJoined;

    };*/

}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_BUY_HPP
