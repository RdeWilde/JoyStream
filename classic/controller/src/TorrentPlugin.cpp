/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 18 2016
 */

#include <controller/TorrentPlugin.hpp>
#include <controller/PeerPlugin.hpp>
#include <core/core.hpp>

namespace joystream {
namespace classic {
namespace controller {

TorrentPlugin::TorrentPlugin(core::TorrentPlugin * torrentPlugin)
    : _torrentPlugin(torrentPlugin)
    , _session(torrentPlugin->session()){

    QObject::connect(torrentPlugin,
                     &core::TorrentPlugin::peerPluginAdded,
                     this,
                     &TorrentPlugin::addPeerPlugin);

    QObject::connect(torrentPlugin,
                     &core::TorrentPlugin::peerPluginRemoved,
                     this,
                     &TorrentPlugin::removePeerPlugin);

}

TorrentPlugin::~TorrentPlugin() {
}

Session & TorrentPlugin::session() noexcept {
    return _session;
}

void TorrentPlugin::addPeerPlugin(core::PeerPlugin * plugin) {

    if(_peerPlugins.count(plugin->endPoint()) > 0)
        throw std::runtime_error("Peer plugin already present");

    // what about link to peer tree view row

    _peerPlugins[plugin->endPoint()] = std::unique_ptr<PeerPlugin>(new PeerPlugin(plugin,
                                                                                  nullptr));
    // setPeerTreeViewRow
}

void TorrentPlugin::removePeerPlugin(const libtorrent::tcp::endpoint & endPoint) {

    //remove

    //update some views?
}

}
}
}
