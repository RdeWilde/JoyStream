/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 11 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_ACTIVE_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_ACTIVE_HPP

#include <protocol_statemachine/CBStateMachine.hpp>
#include <protocol_statemachine/event/Recv.hpp>
#include <protocol_wire/Observe.hpp>
#include <protocol_wire/Buy.hpp>
#include <protocol_wire/Sell.hpp>

#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list.hpp>

namespace joystream {
namespace protocol_statemachine {

    class Observing; // for some reason, we *must* have initial state for composite state
    class Active : public sc::simple_state<Active, CBStateMachine, Observing, sc::has_deep_history> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::Recv<protocol_wire::Observe>>,
                                sc::custom_reaction<event::Recv<protocol_wire::Buy>>,
                                sc::custom_reaction<event::Recv<protocol_wire::Sell>>
                                > reactions;

        Active();

        // Event handlers
        sc::result react(const event::Recv<protocol_wire::Observe> &);
        sc::result react(const event::Recv<protocol_wire::Buy> &);
        sc::result react(const event::Recv<protocol_wire::Sell> &);

    private:

    };

}
}

// Required to make Active complete when included throught his header file,
// as Observe is initial state and thus part of parent state_machine definition
#include <protocol_statemachine/Observing.hpp>

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_ACTIVE_HPP
