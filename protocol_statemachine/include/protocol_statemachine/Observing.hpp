/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#include <protocol_statemachine/Active.hpp>
#include <protocol_statemachine/event/BuyModeStarted.hpp>
#include <protocol_statemachine/event/SellModeStarted.hpp>

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_OBSERVE_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_OBSERVE_HPP

namespace sc = boost::statechart;

namespace joystream {
namespace protocol_statemachine {

    class Observing : public sc::simple_state<Observing, Active> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::BuyModeStarted>,
                                sc::custom_reaction<event::SellModeStarted>
                                > reactions;

        Observing();

        // Event handlers
        sc::result react(const event::BuyModeStarted &);
        sc::result react(const event::SellModeStarted &);
    };

}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_OBSERVE_HPP

