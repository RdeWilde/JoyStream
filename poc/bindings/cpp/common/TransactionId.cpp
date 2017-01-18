#include <addon/common/common.hpp>
#include <addon/common/TransactionId.hpp>

namespace joystream {
namespace addon {
namespace common {
namespace transactionid {

// Buffer - rpc byteorder
v8::Local<v8::Value> toValue(const Coin::TransactionId &id) {
    return UCharVectorBasedToV8Value<Coin::TransactionId>(id);
}

Coin::TransactionId fromValue(const v8::Local<v8::Value>& value) {
    auto data = util::NodeBufferToUCharVector(value);
    return Coin::TransactionId::fromRPCByteOrder(data);
}

}}}}
