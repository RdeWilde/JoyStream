#ifndef JOYSTREAM_NODE_ADDON_COMMON_PUBLICKEY_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_PUBLICKEY_HPP

#include <nan.h>
#include <common/PublicKey.hpp>

namespace joystream {
namespace addon {
namespace common {
namespace publickey {

v8::Local<v8::Value> toValue(const Coin::PublicKey &);
Coin::PublicKey fromValue(const v8::Local<v8::Value>&);

}}}}
#endif
