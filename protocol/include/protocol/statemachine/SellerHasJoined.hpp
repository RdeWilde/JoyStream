/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 28 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATE_MACHINE_SELLERJOINED_HPP
#define JOYSTREAM_PROTOCOL_STATE_MACHINE_SELLERJOINED_HPP

#include <protocol/statemachine/Buying.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class PreparingContract;

    class SellerHasJoined : public sc::simple_state<SellerHasJoined, Buying, PreparingContract> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::Recv<wire::Observe>>,
                                sc::custom_reaction<event::Recv<wire::Buy>>,
                                sc::custom_reaction<event::Recv<wire::Sell>>
                                > reactions;

        SellerHasJoined();

        // Event handlers
        sc::result react(const event::Recv<wire::Observe> &);
        sc::result react(const event::Recv<wire::Buy> &);
        sc::result react(const event::Recv<wire::Sell> &);
    };

}
}
}

// Required to make SellerHasJoined complete when included throught his header file,
// as PreparingContract is initial state and thus part of parent state_machine definition
#include <protocol/statemachine/PreparingContract.hpp>

#endif // JOYSTREAM_PROTOCOL_STATE_MACHINE_SELLERJOINED_HPP
