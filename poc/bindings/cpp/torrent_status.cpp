/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Januar 16 2017
 */

#include "torrent_status.hpp"

#include <libtorrent/torrent_status.hpp>


namespace joystream {
namespace node_addon {
namespace torrent_status {

v8::Local<v8::Object> toObject(const libtorrent::torrent_status & s) {

  //info.GetReturnValue().Set(Nan::New<Number>(ts->progress));
  //info.GetReturnValue().Set(Nan::New<Number>(ts->state));
  //info.GetReturnValue().Set(Nan::New<String>(libtorrent::to_hex(ts->info_hash.to_string())).ToLocalChecked());

}

libtorrent::torrent_status fromObject(const v8::Local<v8::Object> & o) {

}

}
}
}
