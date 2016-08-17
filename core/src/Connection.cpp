/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 18 2016
 */

#include <core/Connection.hpp>
#include <core/CBStateMachine.hpp>
#include <protocol_session/protocol_session.hpp>

namespace joystream {
namespace core {

Connection::Connection(const protocol_session::status::Connection<libtorrent::tcp::endpoint> & status)
    : _connectionId(status.connectionId)
    , _machine(new CBStateMachine(status.machine)) {
}

libtorrent::tcp::endpoint Connection::connectionId() const noexcept {
    return _connectionId;
}

std::shared_ptr<CBStateMachine> Connection::machine() const noexcept {
    return _machine;
}

void Connection::update(const protocol_session::status::Connection<libtorrent::tcp::endpoint> & status) {
    _machine->update(status.machine);
}

}
}
