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
#include "Plugin.hpp"
#include "PeerPluginStatus.hpp"
#include "TorrentPluginStatus.hpp"
#include "payment_channel.hpp"

namespace joystream {
namespace node {

  NAN_MODULE_INIT(Init) {
    libtorrent_interaction::Init(target);
    RequestResult::Init(target);
    peer_plugin_status::Init(target);
    TorrentPluginStatus::Init(target);
    Plugin::Init(target);
    payment_channel::Init(target);
  }

}
}
