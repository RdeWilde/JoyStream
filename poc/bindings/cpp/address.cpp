/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Januar 17 2017
 */

#include "address.hpp"
#include "utils.hpp"

#include <libtorrent/socket_io.hpp>

namespace joystream {
namespace node_addon {
namespace address {

v8::Local<v8::Value> toObject(const libtorrent::address & a) {
    Nan::EscapableHandleScope scope;

    std::string addressString = libtorrent::print_address(a);
    Nan::MaybeLocal<v8::String> uncheckedV8AddressString = Nan::New(addressString);

    return scope.Escape(uncheckedV8AddressString.ToLocalChecked());
}

libtorrent::address fromObject(const v8::Local<v8::Value> & o) {

  std::string addressString = To<std::string>(o);

  libtorrent::error_code ec;
  libtorrent::address a = libtorrent::address::from_string(addressString, ec);

  if(ec)
    throw std::runtime_error(ec.message());
  else
    return a;
}

}
}
}
