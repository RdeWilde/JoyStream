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

void Connection::registerMetaTypes() {
    CBStateMachine::registerMetaTypes();
}

Connection::Connection(const libtorrent::tcp::endpoint & connectionId,
                       CBStateMachine * machine)
    : _connectionId(connectionId)
    , _machine(machine) {
}

Connection * Connection::create(const protocol_session::status::Connection<libtorrent::tcp::endpoint> & status) {
    return new Connection(status.connectionId, CBStateMachine::create(status.machine));
}

Connection::~Connection() {

}

libtorrent::tcp::endpoint Connection::connectionId() const noexcept {
    return _connectionId;
}

CBStateMachine * Connection::machine() const noexcept {
    return _machine.get();
}

void Connection::update(const protocol_session::status::Connection<libtorrent::tcp::endpoint> & status) {
    _machine->update(status.machine);
}

}
}
