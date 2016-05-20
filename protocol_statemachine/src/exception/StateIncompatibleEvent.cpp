/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 21 2016
 */

#include <protocol_statemachine/exception/StateIncompatibleEvent.hpp>

using namespace joystream::protocol_statemachine::exception;

StateIncompatibleEvent::StateIncompatibleEvent(const std::string & state)
    : std::runtime_error(std::string("Event was not compatible with currently active state: ") + state) {
}

/**
StateIncompatibleEvent::StateIncompatibleEvent(const sc::event_base & event)
    : _event(event) {
}

sc::event_base StateIncompatibleEvent::event() const {
    return _event;
}
*/
