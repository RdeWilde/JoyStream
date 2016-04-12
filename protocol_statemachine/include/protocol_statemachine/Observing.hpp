/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#include <protocol_statemachine/Active.hpp>
#include <protocol_statemachine/detail/InitializeObserving.hpp>

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_OBSERVE_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_OBSERVE_HPP

namespace sc = boost::statechart;

namespace joystream {
namespace protocol_statemachine {

    class Observing : public sc::simple_state<Observing, Active> {

    public:

        typedef boost::mpl::list<
                                sc::custom_reaction<detail::InitializeObserving>
                                > reactions;

        Observing();

        // Event handlers
        sc::result react(const detail::InitializeObserving &);

    private:

        // Whether state has been initialized with detail::InitializeBuying
        bool _initialized;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_OBSERVE_HPP

