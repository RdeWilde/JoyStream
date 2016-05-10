/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/PeerPluginId.hpp>

namespace joystream {
namespace extension {

    PeerPluginId::PeerPluginId() {
    }

    PeerPluginId::PeerPluginId(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endPoint)
        : info_hash_(info_hash)
        , endPoint_(endPoint) {
    }

}
}
