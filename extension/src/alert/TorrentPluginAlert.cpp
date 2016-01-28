/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Alert/TorrentPluginAlert.hpp>

TorrentPluginAlert::TorrentPluginAlert(const libtorrent::sha1_hash & infoHash)
    : _infoHash(infoHash) {
}

TorrentPluginAlert::TorrentPluginAlert(const TorrentPluginAlert & alert)
    : _infoHash(alert.infoHash()) {
}

libtorrent::sha1_hash TorrentPluginAlert::infoHash() const {
    return _infoHash;
}
