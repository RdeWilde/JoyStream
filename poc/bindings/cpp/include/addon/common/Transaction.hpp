#ifndef JOYSTREAM_NODE_ADDON_COMMON_TRANSACTION_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_TRANSACTION_HPP

#include <nan.h>
#include <CoinCore/CoinNodeData.h>

namespace joystream {
namespace addon {
namespace common {

class Transaction: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);

      static v8::Local<v8::Object> NewInstance(const Coin::Transaction&);

      Coin::Transaction transaction() const;

    private:
      Transaction();

      Coin::Transaction _tx;
      static Nan::Persistent<v8::Function> constructor;

      static NAN_METHOD(New);
      static NAN_METHOD(ToBuffer);
      static NAN_METHOD(GetVersion);
      static NAN_METHOD(GetLockTime);
      static NAN_METHOD(GetHash);

      void setTx(const Coin::Transaction&);

};

}}}
#endif
