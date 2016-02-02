/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/request/PeerPluginRequest.hpp>

namespace joystream {
namespace extension {
namespace request {

    PeerPluginRequest::PeerPluginRequest(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endpoint)
        : _info_hash(info_hash)
        , _endpoint(endpoint) {
    }

}
}
}
