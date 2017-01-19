/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Lola Rigaut-Luczak <rllola80@gmail.com>, January 18 2017
 */

#ifndef JOYSTREAM_NODE_ADDON_PEER_INFO_H
#define JOYSTREAM_NODE_ADDON_PEER_INFO_H

#include <nan.h>
#include <libtorrent/peer_info.hpp>

#include "endpoint.h"
#include "utils.hpp"

namespace joystream {
namespace node_addon {
namespace peer_info {

/**
 * PeerInfo is encoded as
 * {
 *  ip : { Endpoint object },
 * }
 */
v8::Local<v8::Object> toObject(const libtorrent::peer_info & pi);

}}}

#endif
