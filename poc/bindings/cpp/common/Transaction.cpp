#include <addon/common/common.hpp>
#include <addon/common/Transaction.hpp>

namespace joystream {
namespace addon {
namespace common {
namespace transaction {

v8::Local<v8::Value> toValue(const Coin::Transaction &tx) {
    auto data = tx.getSerialized();
    auto buf = util::UCharVectorToNodeBuffer(data);
    return buf;
}

Coin::Transaction fromValue(const v8::Local<v8::Value>& value) {
    return fromV8ValueToUCharVectorBased<Coin::Transaction>(value);
}

}}}}
