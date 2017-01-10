#include <addon/common/PublicKey.hpp>
#include <addon/util/buffers.hpp>

namespace joystream {
namespace addon {
namespace common {

Nan::Persistent<v8::Function> PublicKey::constructor;

NAN_MODULE_INIT(PublicKey::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("PublicKey").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "toBuffer", ToBuffer);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("PublicKey").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

v8::Local<v8::Object> PublicKey::NewInstance(const Coin::PublicKey &pk) {
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    auto instance = cons->NewInstance(Nan::GetCurrentContext()).ToLocalChecked();
    PublicKey* publicKey = ObjectWrap::Unwrap<PublicKey>(instance);
    publicKey->_publicKey = pk;
    return scope.Escape(instance);
}

bool PublicKey::IsInstance(v8::Object &obj) {
    return obj.GetPrototype() == constructor;
}

Coin::PublicKey PublicKey::publicKey() const {
    return _publicKey;
}

NAN_METHOD(PublicKey::New) {
  if (info.IsConstructCall()) {
    PublicKey *obj = new PublicKey();

    if(info.Length() > 0){
        // If argument is provided, it must be a buffer
        if(!info[0]->IsUint8Array()) {
            Nan::ThrowTypeError("Argument must be a buffer");
            return;
        }

        try {
            obj->_publicKey = Coin::PublicKey(util::NodeBufferToUCharVector(info[0]));
        } catch (const std::exception &ex) {
            Nan::ThrowTypeError(ex.what());
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

NAN_METHOD(PublicKey::ToBuffer) {
    PublicKey* publicKey = ObjectWrap::Unwrap<PublicKey>(info.This());
    auto data = publicKey->_publicKey.toUCharVector();
    info.GetReturnValue().Set(util::UCharVectorToNodeBuffer(data));
}

}}}
