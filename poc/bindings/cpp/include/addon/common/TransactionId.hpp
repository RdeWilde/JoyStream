#ifndef JOYSTREAM_NODE_ADDON_COMMON_TRANSACTIONID_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_TRANSACTIONID_HPP

#include <nan.h>
#include <common/TransactionId.hpp>

namespace joystream {
namespace addon {
namespace common {
namespace TransactionId {

      NAN_MODULE_INIT(Init);

      v8::Local<v8::Object> NewInstance(const Coin::TransactionId&);
      v8::Local<v8::Value> toObject(const Coin::TransactionId &);
      Coin::TransactionId fromObject(const v8::Local<v8::Value>& value);
      bool IsInstance(v8::Object &obj);
      NAN_METHOD(New);

}

}}}

#endif