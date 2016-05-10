/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 27 2016
 */

#include <protocol_statemachine/exception/CannotInviteNonSeller.hpp>

namespace joystream {
namespace protocol_statemachine {
namespace exception {

    CannotInviteNonSeller::CannotInviteNonSeller()
        : std::runtime_error("Cannot invite peer not in seller mode.") {
    }

}
}
}
