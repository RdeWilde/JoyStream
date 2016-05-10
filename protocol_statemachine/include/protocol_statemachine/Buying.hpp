/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_BUY_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_BUY_HPP

#include <protocol_statemachine/Active.hpp>
#include <protocol_statemachine/event/ObserveModeStarted.hpp>
#include <protocol_statemachine/event/SellModeStarted.hpp>
#include <protocol_statemachine/event/UpdateTerms.hpp>

namespace joystream {
namespace protocol_statemachine {

    class ReadyToInviteSeller;

    class Buying : public sc::simple_state<Buying, Active, ReadyToInviteSeller> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<event::ObserveModeStarted>,
                                sc::custom_reaction<event::SellModeStarted>,
                                sc::custom_reaction<event::UpdateTerms<protocol_wire::BuyerTerms>>
                                > reactions;

        Buying();

        // Event handlers
        sc::result react(const event::ObserveModeStarted &);
        sc::result react(const event::SellModeStarted &);
        sc::result react(const event::UpdateTerms<protocol_wire::BuyerTerms> &);

    private:
    };
}
}

// Required to make Buying complete when included throught his header file,
// as ReadyToInvite is initial state and thus part of parent state_machine definition
#include <protocol_statemachine/ReadyToInviteSeller.hpp>

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_BUY_HPP
