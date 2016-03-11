/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 11 2016
 */

#include <protocol/statemachine/ChooseMode.hpp>

#include <iostream>

namespace joystream {
namespace protocol {
namespace statemachine {

    ChooseMode::ChooseMode() {
        std::cout << "Intering choose mode" << std::endl;
    }

    sc::result ChooseMode::ChooseMode::react(const event::ObserveModeStarted & e) {
        std::cout << "Trying to enter Observe mode, but not transition yet";
    }
}
}
}
