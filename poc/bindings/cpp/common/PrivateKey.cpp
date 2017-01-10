#include <addon/common/PrivateKey.hpp>
#include <addon/util/buffers.hpp>

namespace joystream {
namespace addon {
namespace common {

Nan::Persistent<v8::Function> PrivateKey::constructor;

NAN_MODULE_INIT(PrivateKey::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("PrivateKey").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "toBuffer", ToBuffer);
  Nan::SetPrototypeMethod(tpl, "valid", Valid);

  Nan::SetMethod(tpl, "generate", Generate);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("PrivateKey").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

v8::Local<v8::Object> PrivateKey::NewInstance(const Coin::PrivateKey &sk) {
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    auto instance = cons->NewInstance(Nan::GetCurrentContext()).ToLocalChecked();
    PrivateKey* privateKey = ObjectWrap::Unwrap<PrivateKey>(instance);
    privateKey->_privateKey = sk;
    return scope.Escape(instance);
}

Coin::PrivateKey PrivateKey::privateKey() const {
    return _privateKey;
}

NAN_METHOD(PrivateKey::New) {
  if (info.IsConstructCall()) {
    PrivateKey *obj = new PrivateKey();

    if(info.Length() > 0 && info[0]->IsUint8Array()){
        try {
            obj->_privateKey = Coin::PrivateKey(util::NodeBufferToUCharVector(info[0]));
        } catch (const std::exception &ex) {
            Nan::ThrowTypeError(ex.what());
            info.GetReturnValue().SetUndefined();
            return;
        }
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

NAN_METHOD(PrivateKey::ToBuffer) {
    PrivateKey* privateKey = ObjectWrap::Unwrap<PrivateKey>(info.This());
    auto data = privateKey->_privateKey.toUCharVector();
    info.GetReturnValue().Set(util::UCharVectorToNodeBuffer(data));
}

NAN_METHOD(PrivateKey::Generate) {
    auto sk = Coin::PrivateKey::generate();
    info.GetReturnValue().Set(NewInstance(sk));
}

NAN_METHOD(PrivateKey::Valid) {
    PrivateKey* privateKey = ObjectWrap::Unwrap<PrivateKey>(info.This());
    info.GetReturnValue().Set(Coin::PrivateKey::valid(privateKey->_privateKey));
}

}}}
