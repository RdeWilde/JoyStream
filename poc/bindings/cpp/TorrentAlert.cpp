/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 12 2017
 */

#include "TorrentAlert.hpp"
#include "torrent_handle.h"

#include <libtorrent/torrent_status.hpp>

namespace joystream {
namespace node_addon {

TorrentAlert::TorrentAlert(const libtorrent::torrent_handle & handle)
  : _handle(handle) {
}

void TorrentAlert::Init(const std::string & className,
                        v8::Local<v8::FunctionTemplate> & t,
                        int internalFieldCount) {

  Alert::Init(className, t, internalFieldCount);

  Nan::SetPrototypeMethod(t, "handle", handle);
}

NAN_METHOD(TorrentAlert::handle) {

  const TorrentAlert * a = Nan::ObjectWrap::Unwrap<TorrentAlert>(info.This());

  // update New => NewInstance when handle is updated

  info.GetReturnValue().Set(TorrentHandleWrap::New(a->_handle));
}

std::string TorrentAlert::message() const {
  return _handle.is_valid() ? _handle.status().name : " - ";
}

}
}
