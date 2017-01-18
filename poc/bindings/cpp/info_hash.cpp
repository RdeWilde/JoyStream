/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Januar 16 2017
 */

#include "info_hash.hpp"
#include "utils.hpp"

namespace joystream {
namespace node_addon {
namespace info_hash {

v8::Local<v8::Value> toObject(const libtorrent::sha1_hash & h) {
  Nan::EscapableHandleScope scope;

  v8::Local<v8::String> localHexString = Nan::New(h.to_string()).ToLocalChecked();

  return scope.Escape(localHexString);
}

libtorrent::sha1_hash sha1FromHex(const std::string & hex) {

  if(info_hash.size() != 2*libtorrent::sha1_hash::size)
    throw std::runtime_error("incorrent length of hex string");

  char buf[libtorrent::sha1_hash::size];

  if(!libtorrent::from_hex(info_hash.c_str(), info_hash.size(), buf))
    throw std::runtime_error("invalid hex string");

  return libtorrent::sha1_hash(buf);
}

libtorrent::sha1_hash fromObject(const v8::Local<v8::Value> & o) {
  std::string hexString = To<std::string>(o);
  return sha1FromHex(hexString);
}

}
}
}
