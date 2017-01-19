/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Januar 16 2017
 */

#include "torrent_status.hpp"

#define INFO_HASH_KEY "infoHash"
#define STATE_KEY "state"
#define PROGRESS_KEY "progress"

namespace libtorrent {
namespace node {
namespace torrent_status {

v8::Local<v8::Object> toObject(const libtorrent::torrent_status & ts) {
  v8::Local<v8::Object> o = Nan::New<v8::Object>();

  SET_VAL(o, INFO_HASH_KEY, info_hash::toObject(ts.info_hash));
  SET_UINT32(o, STATE_KEY, ts.state);
  SET_DOUBLE(o, PROGRESS_KEY, ts.progress);

  return o;
}

libtorrent::torrent_status fromObject(const v8::Local<v8::Object> & o) {

}

}}}
