/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Januar 17 2017
 */

#ifndef JOYSTREAM_NODE_ADDON_ADDRESS_HPP
#define JOYSTREAM_NODE_ADDON_ADDRESS_HPP

#include <nan.h>
#include <libtorrent/address.hpp> // cannot forward declare libtorrent::address

namespace joystream {
namespace node_addon {
namespace address {

  // Address is encoded as a string in dotted decimal format.

  v8::Local<v8::Value> toObject(const libtorrent::address & a);
  libtorrent::address fromObject(const v8::Local<v8::Value> & o);

}
}
}

#endif // JOYSTREAM_NODE_ADDON_ADDRESS_HPP
