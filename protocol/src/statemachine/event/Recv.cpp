/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#include <protocol/statemachine/event/Recv.hpp>

namespace joystream {
namespace protocol {
namespace statemachine {
namespace event {

    Recv::Recv(joystream::protocol::wire::ExtendedMessagePayload * message)
        : _message(message) {
    }

    joystream::protocol::wire::ExtendedMessagePayload const * Recv::message() const {
        return _message;
    }

    void Recv::setMessage(joystream::protocol::wire::ExtendedMessagePayload * message) {
        _message = message;
    }

}
}
}
}

