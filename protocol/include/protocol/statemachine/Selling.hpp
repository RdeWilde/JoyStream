/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATE_MACHINE_SELL_HPP
#define JOYSTREAM_PROTOCOL_STATE_MACHINE_SELL_HPP

#include <protocol/statemachine/Active.hpp>

#include <functional>
#include <iostream>

namespace Coin {
    class typesafeOutPoint;
    class PublicKey;
}

namespace joystream {
namespace protocol {
namespace statemachine {

    class Selling : public sc::simple_state<Selling,Active> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::ObserveModeStarted>,
                                sc::custom_reaction<event::BuyModeStarted>
                                > reactions;

        Selling();


        // Event handlers
        sc::result react(const event::ObserveModeStarted &);
        sc::result react(const event::BuyModeStarted &);
    };
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATE_MACHINE_SELL_HPP
