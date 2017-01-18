#include <addon/common/common.hpp>
#include <addon/common/PrivateKey.hpp>

namespace joystream {
namespace addon {
namespace common {
namespace privatekey {

v8::Local<v8::Value> toValue(const Coin::PrivateKey &sk) {
    return UCharVectorBasedToV8Value<Coin::PrivateKey>(sk);
}

Coin::PrivateKey fromValue(const v8::Local<v8::Value>& value) {
    return fromV8ValueToUCharVectorBased<Coin::PrivateKey>(value);
}

}}}}
