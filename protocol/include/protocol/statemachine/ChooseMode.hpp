/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_CHOOSEMODE_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_CHOOSEMODE_HPP

#include <protocol/statemachine/CBStateMachine.hpp>
#include <protocol/statemachine/event/ObserveModeStarted.hpp>
#include <protocol/statemachine/event/SellModeStarted.hpp>
#include <protocol/statemachine/event/BuyModeStarted.hpp>

#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol {
namespace statemachine {

    class ChooseMode : public sc::simple_state<ChooseMode, CBStateMachine> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::ObserveModeStarted>,
                                sc::custom_reaction<event::SellModeStarted>,
                                sc::custom_reaction<event::BuyModeStarted>
                                > reactions;

        ChooseMode();

        // Event handlers
        sc::result react(const event::ObserveModeStarted &);
        sc::result react(const event::SellModeStarted &);
        sc::result react(const event::BuyModeStarted &);

    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_CHOOSEMODE_HPP
