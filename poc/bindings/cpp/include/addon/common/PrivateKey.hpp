#ifndef JOYSTREAM_NODE_PRIVATEKEY_HPP
#define JOYSTREAM_NODE_PRIVATEKEY_HPP

#include <nan.h>

namespace Coin {
    class PrivateKey;
}
namespace joystream {
namespace addon {
namespace common {
namespace privatekey {

v8::Local<v8::Value> toValue(const Coin::PrivateKey &);
Coin::PrivateKey fromValue(const v8::Local<v8::Value>&);

}}}}
#endif
