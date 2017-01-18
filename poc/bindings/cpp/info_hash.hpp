/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Januar 16 2017
 */

#ifndef JOYSTREAM_NODE_ADDON_INFO_HASH_HPP
#define JOYSTREAM_NODE_ADDON_INFO_HASH_HPP

#include <nan.h>

namespace libtorrent {
  struct info_hash; // cannot forward declare
}

namespace joystream {
namespace node_addon {
namespace info_hash {

  /**
   * Encoded as hex string.
   */

 v8::Local<v8::Value> toObject(const libtorrent::sha1_hash & h);
 libtorrent::sha1_hash fromObject(const v8::Local<v8::Value> & o);

}
}
}

#endif // JOYSTREAM_NODE_ADDON_INFO_HASH_HPP
