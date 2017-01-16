#ifndef JOYSTREAM_NODE_ADDON_COMMON_PUBLICKEY_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_PUBLICKEY_HPP

#include <nan.h>
#include <common/PublicKey.hpp>

namespace joystream {
namespace addon {
namespace common {

class PublicKey: public Nan::ObjectWrap {
    public:
      PublicKey(const Coin::PublicKey&);

      static NAN_MODULE_INIT(Init);

      static v8::Local<v8::Object> NewInstance(const Coin::PublicKey&);
      static bool IsInstance(v8::Object&);

      Coin::PublicKey publicKey() const;

    private:
      Coin::PublicKey _publicKey;
      static Nan::Persistent<v8::Function> constructor;

      static NAN_METHOD(New);
      static NAN_METHOD(ToBuffer);

};

}}}
#endif
