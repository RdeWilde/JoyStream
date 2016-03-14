/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 11 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_ACTIVE_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_ACTIVE_HPP

#include <protocol/statemachine/CBStateMachine.hpp>
#include <protocol/statemachine/event/Recv.hpp>
#include <protocol/wire/Observe.hpp>
#include <protocol/wire/Buy.hpp>
#include <protocol/wire/Sell.hpp>

#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class Observing; // for some reason, we *must* have initial state for composite state
    class Active : public sc::simple_state<Active, CBStateMachine, Observing, sc::has_deep_history> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::Recv<wire::Observe>>,
                                sc::custom_reaction<event::Recv<wire::Buy>>,
                                sc::custom_reaction<event::Recv<wire::Sell>>
                                > reactions;

        Active();

        // Event handlers
        sc::result react(const event::Recv<wire::Observe> &);
        sc::result react(const event::Recv<wire::Buy> &);
        sc::result react(const event::Recv<wire::Sell> &);

    private:


    };

}
}
}

// Required to make Active complete when included throught his header file,
// as Observe is initial state and thus part of parent state_machine definition
#include <protocol/statemachine/Observing.hpp>

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_ACTIVE_HPP
