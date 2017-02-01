/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 25 2017
 */

#include "Init.hpp"
#include "LibtorrentInteraction.hpp"
#include "RequestResult.hpp"
#include "Connection.hpp"
#include "PeerPluginStatus.hpp"
#include "Plugin.hpp"
#include "TorrentPluginStatus.hpp"

namespace joystream {
namespace node {

  NAN_MODULE_INIT(Init) {
    libtorrent_interaction::Init(target);
    RequestResult::Init(target);
    protocol_session::Connection::Init(target);
    extension::PeerPluginStatus::Init(target);
    extension::TorrentPluginStatus::Init(target);
    Plugin::Init(target);
  }

}
}
