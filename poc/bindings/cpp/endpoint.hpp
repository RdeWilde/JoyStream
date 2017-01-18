/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Januar 16 2017
 */

#ifndef JOYSTREAM_NODE_ADDON_ENDPOINT_HPP
#define JOYSTREAM_NODE_ADDON_ENDPOINT_HPP

#include <nan.h>
#include <libtorrent/socket.hpp> // cannot forward declare libtorrent::tcp::endpoint

namespace joystream {
namespace node_addon {
namespace endpoint {

  /**
   * Endpoint is encoded as
   * {
   *  address : { see address for format },
   *  port : { Number }
   * }
   */

  v8::Local<v8::Object> toObject(const libtorrent::tcp::endpoint & e);
  libtorrent::tcp::endpoint fromObject(const v8::Local<v8::Object> & o);
}
}
}

#endif // JOYSTREAM_NODE_ADDON_ENDPOINT_HPP
