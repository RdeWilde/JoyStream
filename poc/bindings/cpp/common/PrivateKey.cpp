#include <addon/common/PrivateKey.hpp>
#include <addon/util/buffers.hpp>

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

PrivateKey::PrivateKey(){

}

v8::Local<v8::Object> PrivateKey::Make(const Coin::PrivateKey &sk) {
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    auto instance = cons->NewInstance(Nan::GetCurrentContext()).ToLocalChecked();
    PrivateKey* privateKey = ObjectWrap::Unwrap<PrivateKey>(instance);
    privateKey->setPrivateKey(sk);
    return scope.Escape(instance);
}

NAN_METHOD(PrivateKey::New) {
  if (info.IsConstructCall()) {
    PrivateKey *obj = new PrivateKey();

    if(info.Length() > 0){
        if(info[0]->IsUint8Array()) {
            Coin::PrivateKey sk(NodeBufferToUCharVector(info[0]));
            obj->setPrivateKey(sk);
        }

        if(info[0]->IsString()){
            Coin::PrivateKey sk(StringToUCharVector(info[0]));
            obj->setPrivateKey(sk);
        }
    }

    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {info[0]};
    v8::Local<v8::Function> cons = Nan::New(constructor);
    info.GetReturnValue().Set(cons->NewInstance(Nan::GetCurrentContext(), argc, argv).ToLocalChecked());
  }
}

NAN_METHOD(PrivateKey::ToBuffer) {
    PrivateKey* privateKey = ObjectWrap::Unwrap<PrivateKey>(info.This());
    auto data = privateKey->_privateKey.toUCharVector();
    info.GetReturnValue().Set(UCharVectorToNodeBuffer(data));
}

NAN_METHOD(PrivateKey::Generate) {
    auto sk = Coin::PrivateKey::generate();
    info.GetReturnValue().Set(Make(sk));
}

NAN_METHOD(PrivateKey::Valid) {
    PrivateKey* privateKey = ObjectWrap::Unwrap<PrivateKey>(info.This());
    info.GetReturnValue().Set(Coin::PrivateKey::valid(privateKey->_privateKey));
}

void PrivateKey::setPrivateKey(const Coin::PrivateKey &sk) {
    _privateKey = sk;
}
