/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Januar 16 2017
 */

#include "endpoint.hpp"
#include "address.hpp"
#include "utils.hpp"

#define ENDPOINT_ADDRESS_KEY "address"
#define ENDPOINT_PORT_KEY "port"

namespace joystream {
namespace node_addon {
namespace endpoint {

v8::Local<v8::Object> toObject(const libtorrent::tcp::endpoint & ep) {
    Nan::EscapableHandleScope scope;

    v8::Local<v8::Object> endPointObject = Nan::New(); //v8::Object::New();

    SET_OBJ(o, ENDPOINT_ADDRESS_KEY, address::toObject(ep.address));
    SET_UINT32(o, ENDPOINT_PORT_KEY, ep.port);

    return scope.Escape(endPointObject);
}

libtorrent::tcp::endpoint fromObject(const v8::Local<v8::Object> & o) {

  v8::Local<v8::Value> addressValue = GET_VAL(o, ENDPOINT_ADDRESS_KEY);
  libtorrent::address a = address::fromObject(addressValue);

  uint32_t port = GET_UINT32(o, ENDPOINT_PORT_KEY);

  return libtorrent::tcp::endpoint()


}

}
}
}
