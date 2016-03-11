/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#include <protocol/statemachine/Observe.hpp>

#include <iostream>

namespace joystream {
namespace protocol {
namespace statemachine {

    Observe::Observe() {
        std::cout << "Entering Observe state." << std::endl;
    }
}
}
}
