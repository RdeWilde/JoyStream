#ifndef JOYSTREAM_NODE_ADDON_COMMON_PUBKEYHASH_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_PUBKEYHASH_HPP

#include <nan.h>
#include <common/PubKeyHash.hpp>

namespace joystream {
namespace addon {
namespace common {

class PubKeyHash: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);

      static v8::Local<v8::Object> NewInstance(const Coin::PubKeyHash&);

      Coin::PubKeyHash pubKeyHash() const;

    private:
      Coin::PubKeyHash _pubKeyHash;
      static Nan::Persistent<v8::Function> constructor;

      static NAN_METHOD(New);
      static NAN_METHOD(ToBuffer);

};

}}}
#endif
