/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 18 2016
 */

#include <controller/Connection.hpp>
#include <core/core.hpp>

namespace joystream {
namespace classic {
namespace controller {

Connection::Connection(core::Connection * connection)
    : _connection(connection)
    , _stateMachine(connection->machine()) {
}

CBStateMachine & Connection::stateMachine() noexcept {
    return _stateMachine;
}

}
}
}
