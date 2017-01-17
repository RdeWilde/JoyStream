#include <addon/common/PrivateKey.hpp>
#include <addon/util/buffers.hpp>

namespace joystream {
namespace addon {
namespace common {

namespace PrivateKey {

static Nan::Persistent<v8::Function> constructor;

NAN_METHOD(Generate);

NAN_MODULE_INIT(Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("PrivateKey").ToLocalChecked());

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("PrivateKey").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

/*  
    { 
      'sk' : Buffer  // serialized PrivateKey
    }
*/
v8::Local<v8::Value> toObject(const Coin::PrivateKey &sk) {
    Nan::EscapableHandleScope scope;
    auto data = sk.toUCharVector();
    auto buffer = util::UCharVectorToNodeBuffer(data);
    auto obj = Nan::New<v8::Object>();
    Nan::Set(obj, Nan::New("sk").ToLocalChecked(), buffer);
    return scope.Escape(obj);
}

Coin::PrivateKey fromObject(const v8::Local<v8::Value>& value) {
    Nan::HandleScope scope;
    auto obj = Nan::To<v8::Object>(value);
    auto buf = Nan::Get(obj.ToLocalChecked(), Nan::New("sk").ToLocalChecked()).ToLocalChecked();
    if(!buf->IsUint8Array()) {
        throw std::runtime_error("Invalid PrivateKey Object");
    }
    return Coin::PrivateKey(util::NodeBufferToUCharVector(buf));
}

v8::Local<v8::Object> NewInstance(const Coin::PrivateKey &sk) {
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    auto value = toObject(sk);
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {value};
    auto instance = cons->NewInstance(Nan::GetCurrentContext(), argc, argv).ToLocalChecked();
    return scope.Escape(instance);
}

NAN_METHOD(New) {
  if(info.Length() == 0) {
      info.GetReturnValue().Set(NewInstance());
      return;
  }

  if (info.IsConstructCall()) {

    try {
        fromObject(info[0]);
        auto sk = Nan::Get(Nan::To<v8::Object>(info[0]).ToLocalChecked(), Nan::New("sk").ToLocalChecked())
                    .ToLocalChecked();
        // copy the buffer
        auto buf = Nan::CopyBuffer(node::Buffer::Data(sk), node::Buffer::Length(sk));
        Nan::Set(info.This(), Nan::New("sk").ToLocalChecked(), buf.ToLocalChecked());
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
