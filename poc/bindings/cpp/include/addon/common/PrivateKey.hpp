#ifndef JOYSTREAM_NODE_ADDON_COMMON_PRIVATEKEY_HPP
#define JOYSTREAM_NODE_ADDON_COMMON_PRIVATEKEY_HPP

#include <nan.h>
#include <common/PrivateKey.hpp>

namespace joystream {
namespace addon {
namespace common {
namespace privatekey {

v8::Local<v8::Value> toValue(const Coin::PrivateKey &);
Coin::PrivateKey fromValue(const v8::Local<v8::Value>&);

}}}}
#endif
