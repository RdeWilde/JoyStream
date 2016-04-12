/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 21 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_EXCEPTION_STATEINCOMPATIBLEEVENT_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_EXCEPTION_STATEINCOMPATIBLEEVENT_HPP

#include <boost/statechart/event_base.hpp>
#include <stdexcept>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol_statemachine {
namespace exception {

    class StateIncompatibleEvent : public std::runtime_error {

    public:

        StateIncompatibleEvent();

    /**
    public:

        StateIncompatibleEvent(const sc::event_base & event);

        sc::event_base event() const;

    private:

        sc::event_base _event;
        */
    };

}
}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_EXCEPTION_STATEINCOMPATIBLEEVENT_HPP

