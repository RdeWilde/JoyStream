#include <common/PrivateKey.hpp>
#include "buffers.hpp"
#include "PrivateKey.hpp"

namespace joystream {
namespace node {
namespace private_key {

v8::Local<v8::Object> encode(const Coin::PrivateKey &sk) {
    return UCharVectorBasedToV8Value<Coin::PrivateKey>(sk);
}

Coin::PrivateKey decode(const v8::Local<v8::Value>& value) {
    return V8ValueToUCharVectorBased<Coin::PrivateKey>(value);
}

}
}
}
