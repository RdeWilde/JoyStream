/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 1 2016
 */

#ifndef JOYSTREAM_EXTENSION_BUYER_TORRENT_PLUGIN_STATE_HPP
#define JOYSTREAM_EXTENSION_BUYER_TORRENT_PLUGIN_STATE_HPP

namespace joystream {
namespace extension {

    enum class BuyerTorrentPluginState {

        // _payor has reached, or passed, Payor:State::paying status
        waiting_for_payor_to_be_ready,

        // Requesting and downloading pieces
        downloading_pieces,

        // Have full torrent
        done
    };
}
}

#endif // JOYSTREAM_EXTENSION_BUYER_TORRENT_PLUGIN_STATE_HPP

