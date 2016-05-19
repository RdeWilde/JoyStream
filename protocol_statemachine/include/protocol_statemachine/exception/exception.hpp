/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 13 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_EXCEPTION_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_EXCEPTION_HPP

#include <protocol_statemachine/exception/CannotInviteNonSeller.hpp>
#include <protocol_statemachine/exception/InvitedToJoinContractByNonBuyer.hpp>
#include <protocol_statemachine/exception/StateIncompatibleEvent.hpp>

namespace joystream {
namespace protocol_statemachine {
namespace exception {

// Exception that a callback should throw if it deletes the state machine
class StateMachineDeletedException : public std::runtime_error {

public:

    StateMachineDeletedException() : std::runtime_error("State machine deleted in callback.") {}
};

}
}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_EXCEPTION_HPP
