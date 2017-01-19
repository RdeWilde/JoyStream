#include "buffers.hpp"
#include "PubKeyHash.hpp"

namespace joystream {
namespace node {
namespace pubkeyhash {

v8::Local<v8::Value> toValue(const Coin::PubKeyHash &hash) {
    return UCharVectorBasedToV8Value<Coin::PubKeyHash>(hash);
}

Coin::PubKeyHash fromValue(const v8::Local<v8::Value>& value) {
    return V8ValueToUCharVectorBased<Coin::PubKeyHash>(value);
}

}}}
