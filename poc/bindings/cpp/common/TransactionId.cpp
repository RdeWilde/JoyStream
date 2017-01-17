#include <addon/common/TransactionId.hpp>
#include <addon/util/buffers.hpp>
#include <addon/util/helpers.hpp>

namespace joystream {
namespace addon {
namespace common {
namespace TransactionId {

static Nan::Persistent<v8::Function> constructor;

const std::string VALUE_KEY("txid");

NAN_MODULE_INIT(Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("TransactionId").ToLocalChecked());
  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("TransactionId").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

/*  
    { 
      'txid' : Buffer  // serialized TransactionId
    }
*/
v8::Local<v8::Value> toObject(const Coin::TransactionId &txid) {
    Nan::EscapableHandleScope scope;
    auto data = txid.toUCharVector(); // RPC byte order
    auto obj = util::UCharVectorToObject(data, VALUE_KEY);
    return scope.Escape(obj);
}

Coin::TransactionId fromObject(const v8::Local<v8::Value>& value) {
    auto buf = util::GetBufferByKey(value, VALUE_KEY);
    auto data = util::NodeBufferToUCharVector(buf);
    return Coin::TransactionId::fromRPCByteOrder(data);
}

v8::Local<v8::Object> NewInstance(const Coin::TransactionId &tx) {
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    auto value = toObject(tx);
    auto instance = cons->NewInstance(Nan::GetCurrentContext(), 1, &value).ToLocalChecked();
    return scope.Escape(instance);
}

NAN_METHOD(New) {
  if(info.Length() != 1) {
      Nan::ThrowTypeError("Argument Required");
      return;
  }

  // == Use new constructor guard macro here ==

  if (info.IsConstructCall()) {

    try {
        fromObject(info[0]);
        auto buf = util::GetBufferByKey(info[0], VALUE_KEY);
        // copy the buffer
        auto bufcopy = Nan::CopyBuffer(node::Buffer::Data(buf), node::Buffer::Length(buf));
        Nan::Set(info.This(), Nan::New(VALUE_KEY).ToLocalChecked(), bufcopy.ToLocalChecked());
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
