#ifndef JOYSTREAM_NODE_ADDON_COMMON_PUBLICKEY_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_PUBLICKEY_HPP

#include <nan.h>
#include <common/PublicKey.hpp>

namespace joystream {
namespace addon {
namespace common {
namespace PublicKey {

      NAN_MODULE_INIT(Init);

      v8::Local<v8::Object> NewInstance(const Coin::PublicKey&);
      v8::Local<v8::Value> toObject(const Coin::PublicKey &tx);
      Coin::PublicKey fromObject(const v8::Local<v8::Value>& value);
      bool IsInstance(v8::Object &obj);
      NAN_METHOD(New);
}

}}}
#endif
