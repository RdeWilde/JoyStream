/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Lola Rigaut-Luczak <rllola80@gmail.com>, January 18 2017
 */

#include "peer_info.hpp"

#define ADD_TORRENT_PARAMS_IP_KEY "ip"

namespace joystream {
namespace node_addon {
namespace peer_info {

v8::Local<v8::Object> toObject(const libtorrent::peer_info & pi) {
    Nan::EscapableHandleScope scope;

    v8::Local<v8::Object> PeerInfoObject = Nan::New<v8::Object>();

    SET_VAL(PeerInfoObject, ADD_TORRENT_PARAMS_IP_KEY, EndpointWrap::New(pi.ip)
);
    return scope.Escape(PeerInfoObject);
}

}}}
