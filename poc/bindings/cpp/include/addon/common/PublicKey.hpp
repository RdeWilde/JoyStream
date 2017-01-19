#ifndef JOYSTREAM_NODE_PUBLICKEY_HPP
#define JOYSTREAM_NODE_PUBLICKEY_HPP

#include <nan.h>

namespace Coin {
    class PublicKey;
}

namespace joystream {
namespace addon {
namespace common {
namespace publickey {

v8::Local<v8::Value> toValue(const Coin::PublicKey &);
Coin::PublicKey fromValue(const v8::Local<v8::Value>&);

}}}}
#endif
