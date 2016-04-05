/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 27 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_WAITINGFORSELLERTOJOIN_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_WAITINGFORSELLERTOJOIN_HPP

#include <protocol/statemachine/Buying.hpp>
#include <protocol/statemachine/event/Recv.hpp>
#include <wire/JoiningContract.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class WaitingForSellerToJoin : public sc::simple_state<WaitingForSellerToJoin, Buying> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::Recv<joystream::wire::JoiningContract>>
                                > reactions;

        WaitingForSellerToJoin();

        // Event handlers
        sc::result react(const event::Recv<joystream::wire::JoiningContract> &);

    };

}
}
}


#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_WAITINGFORSELLERTOJOIN_HPP
