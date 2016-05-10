/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2015
 */

#ifndef JOYSTREAM_EXTENSION_PEER_MODE_ANNOUNCED_HPP
#define JOYSTREAM_EXTENSION_PEER_MODE_ANNOUNCED_HPP

namespace joystream {
namespace extension {

    // Mode which has been announced by a peer
    enum class PeerModeAnnounced {
        none,
        observer,
        seller,
        buyer
    };

}
}

#endif // JOYSTREAM_EXTENSION_PEER_MODE_ANNOUNCED_HPP
