#ifndef JOYSTREAM_NODE_ADDON_COMMON_TRANSACTIONID_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_TRANSACTIONID_HPP

#include <nan.h>
#include <common/TransactionId.hpp>

namespace joystream {
namespace addon {
namespace common {
namespace transactionid {

v8::Local<v8::Value> toValue(const Coin::TransactionId &);
Coin::TransactionId fromValue(const v8::Local<v8::Value>&);

}}}}
#endif
