/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#include <controller/Peer.hpp>
#include <gui/gui.hpp>
#include <core/core.hpp>

namespace joystream {
namespace classic {
namespace controller {

Peer::Peer(core::Peer * peer,
           gui::PeerTableRowModel * row)
    : _peer(peer)
    , _row(row) {

    QObject::connect(peer,
                     &core::Peer::clientChanged,
                     this,
                     &Peer::setClientName);

    setHost(peer->endPoint()); // no signal
    setClientName(peer->client());

}

void Peer::setHost(const boost::asio::ip::tcp::endpoint &endPoint) {
    _row->setHost(endPoint);
}

void Peer::setClientName(const std::string &client) {
    _row->setClientName(client);
}

void Peer::setBEPSupport(const extension::BEPSupportStatus & status) {
    _row->setBEPSupport(status);
}

}
}
}
