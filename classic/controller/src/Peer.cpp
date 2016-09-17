/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#include <controller/Peer.hpp>
#include <gui/PeersDialog/PeerTreeViewRow.hpp>
#include <core/core.hpp>

namespace joystream {
namespace classic {
namespace controller {

Peer::Peer(core::Peer * peer,
           gui::PeerTreeViewRow * peerTreeViewRow)
    : _peer(peer)
    , _peerTreeViewRow(peerTreeViewRow) {

    QObject::connect(peer,
                     &core::Peer::clientChanged,
                     this,
                     &Peer::setClientName);
}

gui::PeerTreeViewRow * Peer::peerTreeViewRow() const noexcept {
    return _peerTreeViewRow.get();
}

void Peer::setPeerTreeViewRow(gui::PeerTreeViewRow * peerTreeViewRow) {
    _peerTreeViewRow.reset(peerTreeViewRow);
}

void Peer::dropPeerTreeViewRow() {
    _peerTreeViewRow.release();
}

bool Peer::peerTreeViewRowSet() const noexcept {
    return _peerTreeViewRow.get() != nullptr;
}

void Peer::setHost(const libtorrent::tcp::endpoint & endPoint) {

    if(peerTreeViewRowSet())
        _peerTreeViewRow->setHost(endPoint);
}

void Peer::setClientName(std::string & client) {

    if(peerTreeViewRowSet())
        _peerTreeViewRow->setClientName(client);
}

void Peer::setBEPSupport(const extension::BEPSupportStatus & status) {

    if(peerTreeViewRowSet())
        _peerTreeViewRow->setBEPSupport(status);
}

}
}
}
