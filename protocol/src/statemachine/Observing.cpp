/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#include <protocol/statemachine/Observing.hpp>

#include <iostream>

namespace joystream {
namespace protocol {
namespace statemachine {

    Observing::Observing()
        : _initialized(false) {
        std::cout << "Entering Observing state." << std::endl;
    }

    sc::result Observing::react(const detail::InitializeObserving &) {

        if(_initialized)
            throw std::runtime_error("Observing state already initialized.");
        else
           _initialized = true;

        std::cout << "Reacting to detail::InitializeObserving." << std::endl;

        // Send mode message.
        context<CBStateMachine>()._sendMessage(new wire::Observe());

        // No transition
        return discard_event();
    }

}
}
}
