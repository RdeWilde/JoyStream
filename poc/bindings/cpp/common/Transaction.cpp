#include <addon/common/Transaction.hpp>
#include <addon/util/buffers.hpp>

namespace joystream {
namespace addon {
namespace common {
namespace Transaction {

static Nan::Persistent<v8::Function> constructor;

NAN_MODULE_INIT(Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Transaction").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Transaction").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

/*  
    { 
      'tx' : Buffer  // serialized transaction
    }
*/
v8::Local<v8::Value> toObject(const Coin::Transaction &tx) {
    Nan::EscapableHandleScope scope;
    auto data = tx.getSerialized();
    auto buffer = util::UCharVectorToNodeBuffer(data);
    auto obj = Nan::New<v8::Object>();
    Nan::Set(obj, Nan::New("tx").ToLocalChecked(), buffer);
    return scope.Escape(obj);
}

Coin::Transaction fromObject(const v8::Local<v8::Value>& value) {
    Nan::HandleScope scope;
    auto obj = Nan::To<v8::Object>(value);
    auto buf = Nan::Get(obj.ToLocalChecked(), Nan::New("tx").ToLocalChecked()).ToLocalChecked();
    if(!buf->IsUint8Array()) {
        throw std::runtime_error("Invalid Transaction Object");
    }
    return Coin::Transaction(util::NodeBufferToUCharVector(buf));
}

v8::Local<v8::Object> NewInstance(const Coin::Transaction &tx) {
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    auto value = toObject(tx);
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {value};
    auto instance = cons->NewInstance(Nan::GetCurrentContext(), argc, argv).ToLocalChecked();
    return scope.Escape(instance);
}

NAN_METHOD(New) {
  if(info.Length() != 1) {
      Nan::ThrowTypeError("Argument Required");
      return;
  }

  if (info.IsConstructCall()) {

    try {
        fromObject(info[0]);
        auto tx = Nan::Get(Nan::To<v8::Object>(info[0]).ToLocalChecked(), Nan::New("tx").ToLocalChecked())
                    .ToLocalChecked();
        // copy the buffer
        auto buf = Nan::CopyBuffer(node::Buffer::Data(tx), node::Buffer::Length(tx));
        Nan::Set(info.This(), Nan::New("tx").ToLocalChecked(), buf.ToLocalChecked());
    } catch(std::exception &e) {
        Nan::ThrowTypeError(e.what());
    }

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

bool IsInstance(v8::Object &obj) {
    return obj.GetPrototype() == constructor;
}

}

}}}
