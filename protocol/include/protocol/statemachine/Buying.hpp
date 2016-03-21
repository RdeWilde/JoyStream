/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_BUY_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_BUY_HPP

#include <protocol/statemachine/Active.hpp>
#include <protocol/statemachine/detail/InitializeBuying.hpp>
#include <protocol/statemachine/event/ObserveModeStarted.hpp>
#include <protocol/statemachine/event/SellModeStarted.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {

    class Buying : public sc::simple_state<Buying, Active> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<detail::InitializeBuying>,
                                sc::custom_reaction<event::ObserveModeStarted>,
                                sc::custom_reaction<event::SellModeStarted>
                                > reactions;

        Buying();

        // Event handlers
        sc::result react(const detail::InitializeBuying &);
        sc::result react(const event::ObserveModeStarted &);
        sc::result react(const event::SellModeStarted &);

    private:

        // Whether state has been initialized with detail::InitializeBuying
        bool _initialized;

        //// Client state

        // Most recent terms broadcasted
        BuyerTerms _terms;
    };
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_BUY_HPP
