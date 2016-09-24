/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 19 2016
 */

#include <controller/Session.hpp>
#include <controller/Connection.hpp>
#include <core/core.hpp>

namespace joystream {
namespace classic {
namespace controller {

Session::Session(core::Session * session)
    : _session(session) {

    QObject::connect(session,
                     &core::Session::modeChanged,
                     this,
                     &Session::setMode);

    QObject::connect(session,
                     &core::Session::stateChanged,
                     this,
                     &Session::setState);

    QObject::connect(session,
                     &core::Session::connectionAdded,
                     this,
                     &Session::addConnection);

    QObject::connect(session,
                     &core::Session::connectionRemoved,
                     this,
                     &Session::removeConnection);

}

Session::~Session() {

}

Session::SubState & Session::subState() noexcept {
    return _subState;
}

void Session::setTorrentTreeViewRow(gui::TorrentTreeViewRow * row) {
    _torrentTreeViewRow = row;
}

void Session::unsetTorrentTreeViewRow() {
    _torrentTreeViewRow = nullptr;
}

void Session::setPeersDialog(gui::PeersDialog * peersDialog) {
    _peersDialog = peersDialog;
}

void Session::unsetPeersDialog() {
    _peersDialog = nullptr;
}

void Session::setMode(protocol_session::SessionMode mode) {
    //_
}

void Session::setState(protocol_session::SessionState state) {

}

void Session::addConnection(const core::Connection * c) {

    if(_peerPlugins.count(c->connectionId()) > 0)
        throw std::runtime_error("Connection already added.");

    // _peers[peer->endPoint()] = std::unique_ptr<Peer>(new Peer(peer, row));
}

void Session::removeConnection(const libtorrent::tcp::endpoint & endPoint) {

    //
}



}
}
}
