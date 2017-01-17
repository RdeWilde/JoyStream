#include <addon/common/PubKeyHash.hpp>
#include <addon/util/helpers.hpp>
#include <addon/util/buffers.hpp>

namespace joystream {
namespace addon {
namespace common {

namespace PubKeyHash {

static Nan::Persistent<v8::Function> constructor;

const std::string VALUE_KEY("hash");

NAN_MODULE_INIT(Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("PubKeyHash").ToLocalChecked());
  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("PubKeyHash").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

/*  
    { 
      'hash' : Buffer  // serialized PubKeyHash
    }
*/
v8::Local<v8::Value> toObject(const Coin::PubKeyHash &sk) {
    Nan::EscapableHandleScope scope;
    uchar_vector data = sk.toUCharVector();
    auto obj = util::UCharVectorToObject(data, VALUE_KEY);
    return scope.Escape(obj);
}

Coin::PubKeyHash fromObject(const v8::Local<v8::Value>& value) {
    return util::GetNativeTypeFromBufferByKey<Coin::PubKeyHash>(value, VALUE_KEY);
}

v8::Local<v8::Object> NewInstance(const Coin::PubKeyHash &sk) {
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    auto value = toObject(sk);
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
        auto sk = fromObject(info[0]);
        
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