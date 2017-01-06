#ifndef JOYSTREAM_NODE_ADDON_COMMON_PRIVATEKEY_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_PRIVATEKEY_HPP

#include <nan.h>
#include <common/PrivateKey.hpp>

class PrivateKey: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);

      PrivateKey();

      static v8::Local<v8::Object> Make(const Coin::PrivateKey&);

    private:
      Coin::PrivateKey _privateKey;
      static Nan::Persistent<v8::Function> constructor;

      static NAN_METHOD(New);
      static NAN_METHOD(ToBuffer);
      static NAN_METHOD(Generate);
      static NAN_METHOD(Valid);

      void setPrivateKey(const Coin::PrivateKey&);

};

#endif
