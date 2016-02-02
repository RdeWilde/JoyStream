/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/request/StartObserverTorrentPlugin.hpp>

namespace joystream {
namespace extension {
namespace request {

    StartObserverTorrentPlugin::StartObserverTorrentPlugin() {
    }

    StartObserverTorrentPlugin::StartObserverTorrentPlugin(const libtorrent::sha1_hash & infoHash)
        : _infoHash(infoHash) {
    }

    PluginRequestType StartObserverTorrentPlugin::getPluginRequestType() const {
        return PluginRequestType::StartObserverTorrentPlugin;
    }

    libtorrent::sha1_hash StartObserverTorrentPlugin::infoHash() const {
        return _infoHash;
    }

    void StartObserverTorrentPlugin::setInfoHash(const libtorrent::sha1_hash & infoHash) {
        _infoHash = infoHash;
    }

}
}
}
