/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_OBSERVEMODESTARTED_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_OBSERVEMODESTARTED_HPP

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol {
namespace statemachine {
namespace event {

    class ObserveModeStarted : public sc::event<ObserveModeStarted> {

    };

}
}
}
}

#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_EVENT_OBSERVEMODESTARTED_HPP

