#ifndef JOYSTREAM_NODE_ADDON_COMMON_PUBKEYHASH_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_PUBKEYHASH_HPP

#include <nan.h>
#include <common/PubKeyHash.hpp>

namespace joystream {
namespace addon {
namespace common {
namespace pubkeyhash {

v8::Local<v8::Value> toValue(const Coin::PubKeyHash &);
Coin::PubKeyHash fromValue(const v8::Local<v8::Value>&);

}}}}
#endif
