#ifndef JOYSTREAM_NODE_ADDON_COMMON_TRANSACTION_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_TRANSACTION_HPP

#include <nan.h>
#include <CoinCore/CoinNodeData.h>

namespace joystream {
namespace addon {
namespace common {
namespace Transaction {

      NAN_MODULE_INIT(Init);

      v8::Local<v8::Object> NewInstance(const Coin::Transaction&);
      v8::Local<v8::Value> toObject(const Coin::Transaction &tx);
      Coin::Transaction fromObject(const v8::Local<v8::Value>& value);
      bool IsInstance(v8::Object &obj);
      NAN_METHOD(New);
}

}}}
#endif
