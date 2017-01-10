#include <addon/common/Transaction.hpp>
#include <addon/util/buffers.hpp>

namespace joystream {
namespace addon {
namespace common {

Nan::Persistent<v8::Function> Transaction::constructor;

NAN_MODULE_INIT(Transaction::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Transaction").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "toBuffer", ToBuffer);
  Nan::SetPrototypeMethod(tpl, "version", GetVersion);
  Nan::SetPrototypeMethod(tpl, "locktime", GetLockTime);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Transaction").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

v8::Local<v8::Object> Transaction::NewInstance(const Coin::Transaction &tx) {
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    auto instance = cons->NewInstance(Nan::GetCurrentContext()).ToLocalChecked();
    Transaction* transaction = ObjectWrap::Unwrap<Transaction>(instance);
    transaction->_tx = tx;
    return scope.Escape(instance);
}

bool Transaction::IsInstance(v8::Object &obj) {
    return obj.GetPrototype() == constructor;
}

Coin::Transaction Transaction::transaction() const {
    return _tx;
}

NAN_METHOD(Transaction::New) {
  if (info.IsConstructCall()) {
    Transaction *obj = new Transaction();

    if(info.Length() > 0){
        // If argument is provided, it must be a buffer
        if(!info[0]->IsUint8Array()) {
            Nan::ThrowTypeError("Argument must be a buffer");
            return;
        }

        obj->_tx.setSerialized(util::NodeBufferToUCharVector(info[0]));
    }

    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    v8::Local<v8::Function> cons = Nan::New(constructor);
    if(info.Length() > 0) {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]};
      info.GetReturnValue().Set(cons->NewInstance(Nan::GetCurrentContext(), argc, argv).ToLocalChecked());
      return;
    }
    info.GetReturnValue().Set(cons->NewInstance(Nan::GetCurrentContext()).ToLocalChecked());
  }
}

NAN_METHOD(Transaction::ToBuffer) {
    Transaction* transaction = ObjectWrap::Unwrap<Transaction>(info.This());
    auto data = transaction->_tx.getSerialized();
    info.GetReturnValue().Set(util::UCharVectorToNodeBuffer(data));
}

NAN_METHOD(Transaction::GetVersion) {
    Transaction* transaction = ObjectWrap::Unwrap<Transaction>(info.This());
    info.GetReturnValue().Set(transaction->_tx.version);
}

NAN_METHOD(Transaction::GetLockTime) {
    Transaction* transaction = ObjectWrap::Unwrap<Transaction>(info.This());
    info.GetReturnValue().Set(transaction->_tx.lockTime);
}

NAN_METHOD(Transaction::GetHash) {
    Transaction* transaction = ObjectWrap::Unwrap<Transaction>(info.This());
    auto data = transaction->_tx.hash();
    info.GetReturnValue().Set(util::UCharVectorToNodeBuffer(data));
}

}}}
