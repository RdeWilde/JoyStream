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

typedef std::function<void(const libtorrent::tcp::endpoint &)> NodeStarted;
typedef std::function<void(const libtorrent::tcp::endpoint &, libtorrent::error_code)> NodeStartFailed;

typedef std::function<void()> NodeStopped;

//typedef std::function<void(const libtorrent::error_code)> AddedTorrent;
typedef extension::request::AddTorrent::AddTorrentHandler AddedTorrent;

}
}

#endif // JOYSTREAM_CORE_CALLBACKS_HPP
