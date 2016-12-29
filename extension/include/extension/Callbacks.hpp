/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 22 2016
 */

#ifndef JOYSTREAM_EXTENSION_CALLBACKS_HPP
#define JOYSTREAM_EXTENSION_CALLBACKS_HPP

#include <libtorrent/sha1_hash.hpp>
#include <CoinCore/CoinNodeData.h>

namespace joystream {
namespace extension {

// Broadcast a transaction associated with a given torrent
typedef std::function<void(const libtorrent::sha1_hash &, const Coin::Transaction &)> TransactionBroadcaster;

}
}

#endif // JOYSTREAM_EXTENSION_CALLBACKS_HPP
