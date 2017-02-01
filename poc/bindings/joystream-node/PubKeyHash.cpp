#include "buffers.hpp"
#include "PubKeyHash.hpp"

namespace joystream {
namespace node {
namespace pubkey_hash {

v8::Local<v8::Object> encode(const Coin::PubKeyHash &hash) {
    return UCharVectorBasedToV8Value<Coin::PubKeyHash>(hash);
}

Coin::PubKeyHash decode(const v8::Local<v8::Value>& value) {
    return V8ValueToUCharVectorBased<Coin::PubKeyHash>(value);
}

}
}
}
