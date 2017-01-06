#ifndef JOYSTREAM_NODE_ADDON_COMMON_PUBKEYHASH_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_PUBKEYHASH_HPP

#include <nan.h>
#include <common/PubKeyHash.hpp>

class PubKeyHash: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);

      PubKeyHash();

      static v8::Local<v8::Object> Make(const Coin::PubKeyHash&);

    private:
      Coin::PubKeyHash _pubKeyHash;
      static Nan::Persistent<v8::Function> constructor;

      static NAN_METHOD(New);
      static NAN_METHOD(ToBuffer);

      void setPubKeyHash(const Coin::PubKeyHash&);

};

#endif
