/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 18 2016
 */

#include <core/Seller.hpp>

namespace joystream {
namespace core {

Seller::Seller(const protocol_session::status::Seller<libtorrent::tcp::endpoint> & status)
    : _state(status.state)
    , _connectionId(status.connection) {
}

protocol_session::SellerState Seller::state() const noexcept {
    return _state;
}

libtorrent::tcp::endpoint Seller::connectionId() const noexcept {
    return _connectionId;
}

void Seller::update(const protocol_session::status::Seller<libtorrent::tcp::endpoint> & status) {

    assert(status.connection == _connectionId);

    if(_state != status.state) {

        _state = status.state;
        emit stateChanged(_state);
    }
}

}
}
