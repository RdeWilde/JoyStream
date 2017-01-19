#include <common/PrivateKey.hpp>
#include "buffers.hpp"
#include "PrivateKey.hpp"

namespace joystream {
namespace node {
namespace privatekey {

v8::Local<v8::Value> toValue(const Coin::PrivateKey &sk) {
    return UCharVectorBasedToV8Value<Coin::PrivateKey>(sk);
}

Coin::PrivateKey fromValue(const v8::Local<v8::Value>& value) {
    return V8ValueToUCharVectorBased<Coin::PrivateKey>(value);
}

}}}
