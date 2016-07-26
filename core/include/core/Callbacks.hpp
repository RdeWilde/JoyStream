/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#ifndef JOYSTREAM_CORE_CALLBACKS_HPP
#define JOYSTREAM_CORE_CALLBACKS_HPP

#include <libtorrent/socket.hpp>
#include <libtorrent/error_code.hpp>
#include <extension/extension.hpp>
#include <functional>

namespace joystream {
namespace core {

typedef std::function<void(const Coin::Transaction &)> BroadcastTransaction;
//typedef std::function<void(const libtorrent::tcp::endpoint &)> NodeStarted;
typedef std::function<void()> NodeUnPaused;
typedef std::function<void()> NodePaused;
typedef extension::request::AddTorrent::AddTorrentHandler AddedTorrent;
typedef extension::request::SubroutineHandler RemovedTorrent;
typedef extension::request::SubroutineHandler TorrentPaused;
typedef extension::request::SubroutineHandler TorrentResumed;

}
}

#endif // JOYSTREAM_CORE_CALLBACKS_HPP
