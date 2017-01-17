#ifndef JOYSTREAM_NODE_ADDON_COMMON_OUTPOINT_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_OUTPOINT_HPP

#include <nan.h>
#include <common/typesafeOutPoint.hpp>

namespace joystream {
namespace addon {
namespace common {
namespace OutPoint {

      v8::Local<v8::Value> toObject(const Coin::typesafeOutPoint &op);
      Coin::typesafeOutPoint fromObject(const v8::Local<v8::Value>& value);

}

}}}
#endif
