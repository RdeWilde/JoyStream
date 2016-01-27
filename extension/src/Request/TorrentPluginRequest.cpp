/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Request/TorrentPluginRequest.hpp>

TorrentPluginRequest::TorrentPluginRequest(const libtorrent::sha1_hash & info_hash)
    : _info_hash(info_hash) {
}

libtorrent::sha1_hash TorrentPluginRequest::info_hash() const {
    return _info_hash;
}

void TorrentPluginRequest::setInfo_hash(const libtorrent::sha1_hash & info_hash) {
    _info_hash = info_hash;
}

