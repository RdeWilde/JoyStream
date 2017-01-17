#ifndef JOYSTREAM_NODE_ADDON_COMMON_PUBKEYHASH_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_PUBKEYHASH_HPP

#include <nan.h>
#include <common/PubKeyHash.hpp>

namespace joystream {
namespace addon {
namespace common {
namespace PubKeyHash {

      NAN_MODULE_INIT(Init);

      v8::Local<v8::Object> NewInstance(const Coin::PubKeyHash&);
      v8::Local<v8::Value> toObject(const Coin::PubKeyHash &tx);
      Coin::PubKeyHash fromObject(const v8::Local<v8::Value>& value);
      bool IsInstance(v8::Object &obj);
      NAN_METHOD(New);

}

}}}
#endif
