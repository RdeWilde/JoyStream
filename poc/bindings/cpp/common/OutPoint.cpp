#include <addon/common/OutPoint.hpp>
#include <addon/common/TransactionId.hpp>
#include <utils.hpp>

namespace joystream {
namespace addon {
namespace common {
namespace outpoint {
/*
    {
        txid: TransactionId,
        index: number  // index
    }
*/
v8::Local<v8::Value> toValue(const Coin::typesafeOutPoint &op) {
    auto txid = transactionid::toValue(op.transactionId());
    auto index = op.index();
    auto obj = Nan::New<v8::Object>();
    SET_VAL(obj, "txid", txid);
    SET_INT32(obj, "index", index);
    return obj;
}

Coin::typesafeOutPoint fromValue(const v8::Local<v8::Value>& value) {
    auto obj = Nan::To<v8::Object>(value).ToLocalChecked();
    auto txidObj = GET_VAL(obj, "txid");
    auto index = GET_INT32(obj, "index");
    auto txid = transactionid::fromValue(txidObj);
    return Coin::typesafeOutPoint(txid, index);
}

}}}}
