/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#include <core/PeerPlugin.hpp>
#include <core/Exception.hpp>

Q_DECLARE_METATYPE(joystream::extension::BEPSupportStatus)
Q_DECLARE_METATYPE(joystream::extension::status::PeerPlugin)
Q_DECLARE_METATYPE(joystream::protocol_session::status::Connection<libtorrent::tcp::endpoint>)

namespace joystream {
namespace core {

void PeerPlugin::registerMetaTypes() {
    //qRegisterMetaType<extension::status::PeerPlugin>();
    //qRegisterMetaType<protocol_session::status::Connection<libtorrent::tcp::endpoint>>();
}

PeerPlugin::PeerPlugin(const extension::status::PeerPlugin & status)
    : _status(status) {
}

extension::status::PeerPlugin PeerPlugin::status() const noexcept {
    return _status;
}

void PeerPlugin::update(const extension::status::PeerPlugin & status) {

    _status = status;

    emit statusUpdated(status);
}

}
}
