/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 18 2016
 */

#include <core/Connection.hpp>

namespace joystream {
namespace core {

Connection::Connection(const protocol_session::status::Connection<libtorrent::tcp::endpoint> & status)
    : _connectionId(status.connectionId)
    , _announcedModeAndTermsFromPeer(status.machine.announcedModeAndTermsFromPeer) {
}

libtorrent::tcp::endpoint Connection::connectionId() const noexcept {
    return _connectionId;
}

protocol_statemachine::AnnouncedModeAndTerms Connection::announcedModeAndTermsFromPeer() const noexcept {
    return _announcedModeAndTermsFromPeer;
}

void Connection::update(const protocol_session::status::Connection<libtorrent::tcp::endpoint> & status) {

    if(_announcedModeAndTermsFromPeer != status.machine.announcedModeAndTermsFromPeer) {

        _announcedModeAndTermsFromPeer = status.machine.announcedModeAndTermsFromPeer;
        emit announcedModeAndTermsFromPeerChanged(_announcedModeAndTermsFromPeer);
    }
}

}
}
