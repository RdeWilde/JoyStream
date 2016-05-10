/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 29 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_PREPARINGCONTRACT_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_PREPARINGCONTRACT_HPP

#include <protocol_statemachine/SellerHasJoined.hpp>
#include <protocol_statemachine/event/ContractPrepared.hpp>

namespace joystream {
namespace protocol_statemachine {

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

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_PREPARINGCONTRACT_HPP

