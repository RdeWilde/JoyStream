#ifndef JOYSTREAM_NODE_ADDON_COMMON_TRANSACTION_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_TRANSACTION_HPP

#include <nan.h>
#include <CoinCore/CoinNodeData.h>

namespace joystream {
namespace addon {
namespace common {
namespace transaction {

v8::Local<v8::Value> toValue(const Coin::Transaction &);
Coin::Transaction fromValue(const v8::Local<v8::Value>&);

}}}}
#endif
