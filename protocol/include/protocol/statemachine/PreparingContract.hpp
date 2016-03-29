/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_PREPARINGCONTRACT_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_PREPARINGCONTRACT_HPP

#include <protocol/statemachine/SellerHasJoined.hpp>
#include <protocol/statemachine/event/ContractPrepared.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class PreparingContract : public sc::simple_state<PreparingContract, SellerHasJoined > {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::ContractPrepared>
                                > reactions;

        PreparingContract();

        sc::result react(const event::ContractPrepared &);
    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_PREPARINGCONTRACT_HPP

