/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#include <controller/Peer.hpp>
#include <core/core.hpp>

namespace joystream {
namespace classic {
namespace controller {

Peer::Peer(core::Peer * peer)
    : _peer(peer) {
}

boost::optional<gui::PeerTreeViewRow> Peer::peerTreeViewRow() const {
    return _peerTreeViewRow;
}

void Peer::setPeerTreeViewRow(const gui::PeerTreeViewRow & row) {
    _peerTreeViewRow = row;
}

void Peer::setHost(const libtorrent::tcp::endpoint & endPoint) {

    if(_peerTreeViewRow.is_initialized())
        _peerTreeViewRow.get().setHost(endPoint);
}

void Peer::setClientName(std::string & client) {

    if(_peerTreeViewRow.is_initialized())
        _peerTreeViewRow.get().setClientName(client);
}


void Peer::setBEPSupport(const extension::BEPSupportStatus & status) {

    if(_peerTreeViewRow.is_initialized())
        _peerTreeViewRow.get().setBEPSupport(status);
}

}
}
}
