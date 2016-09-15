/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 18 2016
 */

#include <core/Seller.hpp>

Q_DECLARE_METATYPE(joystream::protocol_session::SellerState)

namespace joystream {
namespace core {

void Seller::registerMetaTypes() {

    qRegisterMetaType<protocol_session::SellerState>();
}

Seller::Seller(const protocol_session::SellerState & state,
               const libtorrent::tcp::endpoint & connectionId)
    : _state(state)
    , _connectionId(connectionId) {
}

Seller * Seller::create(const protocol_session::status::Seller<libtorrent::tcp::endpoint> & status) {
    return new Seller(status.state, status.connection);
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
