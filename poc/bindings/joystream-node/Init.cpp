/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 25 2017
 */

#include "Init.hpp"
#include "LibtorrentInteraction.hpp"

namespace joystream {
namespace node {

  NAN_MODULE_INIT(Init) {

    libtorrent_interaction::Init(target);

    // Call inits for each joystream type

  }
}
}
