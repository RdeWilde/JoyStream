#ifndef JOYSTREAM_NODE_ADDON_COMMON_PRIVATEKEY_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_PRIVATEKEY_HPP

#include <nan.h>
#include <common/PrivateKey.hpp>

namespace joystream {
namespace addon {
namespace common {

namespace PrivateKey {

      NAN_MODULE_INIT(Init);

      v8::Local<v8::Object> NewInstance(const Coin::PrivateKey&);
      v8::Local<v8::Value> toObject(const Coin::PrivateKey &tx);
      Coin::PrivateKey fromObject(const v8::Local<v8::Value>& value);
      bool IsInstance(v8::Object &obj);
      NAN_METHOD(New);
}

}}}
#endif
