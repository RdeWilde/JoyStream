#include <common/PublicKey.hpp>
#include "buffers.hpp"
#include "PublicKey.hpp"

namespace joystream {
namespace node {
namespace public_key {

v8::Local<v8::Object> encode(const Coin::PublicKey &pk) {
    return UCharVectorBasedToV8Value<Coin::PublicKey>(pk);
}

Coin::PublicKey decode(const v8::Local<v8::Value>& value) {
    return V8ValueToUCharVectorBased<Coin::PublicKey>(value);
}

}
}
}

