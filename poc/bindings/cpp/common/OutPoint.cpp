#include <addon/common/OutPoint.hpp>
#include <addon/common/TransactionId.hpp>

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
    Nan::EscapableHandleScope scope;
    auto txid = transactionid::toValue(op.transactionId());
    auto index = op.index();
    auto obj = Nan::New<v8::Object>();
    Nan::Set(obj, Nan::New("txid").ToLocalChecked(), txid);
    Nan::Set(obj, Nan::New("index").ToLocalChecked(), Nan::New(index));
    return scope.Escape(obj);
}

Coin::typesafeOutPoint fromValue(const v8::Local<v8::Value>& value) {
    auto obj = Nan::To<v8::Object>(value).ToLocalChecked();
    auto txidObj = Nan::Get(obj, Nan::New("txid").ToLocalChecked()).ToLocalChecked();
    auto index = Nan::Get(obj, Nan::New("index").ToLocalChecked()).ToLocalChecked();
    auto txid = transactionid::fromValue(txidObj);
    return Coin::typesafeOutPoint(txid, index->IntegerValue());
}

}}}}
