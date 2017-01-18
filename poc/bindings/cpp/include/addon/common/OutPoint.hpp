#ifndef JOYSTREAM_NODE_ADDON_COMMON_OUTPOINT_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_OUTPOINT_HPP

#include <nan.h>
#include <common/typesafeOutPoint.hpp>

namespace joystream {
namespace addon {
namespace common {
namespace outpoint {

v8::Local<v8::Value> toValue(const Coin::typesafeOutPoint &);
Coin::typesafeOutPoint fromValue(const v8::Local<v8::Value>&);

}}}}
#endif
