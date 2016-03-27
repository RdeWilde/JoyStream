/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 27 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_EXCEPTION_CANNOTINVITESELLER__HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_EXCEPTION_CANNOTINVITESELLER__HPP

#include <protocol/PeerModeAnnounced.hpp>
#include <stdexcept>

namespace joystream {
namespace protocol {
namespace statemachine {
namespace exception {

    class CannotInviteNonSeller : public std::runtime_error {
    };

}
}
}
}


#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_EXCEPTION_CANNOTINVITESELLER__HPP
