#include <addon/common/PubKeyHash.hpp>
#include <addon/util/buffers.hpp>

Nan::Persistent<v8::Function> PubKeyHash::constructor;

NAN_MODULE_INIT(PubKeyHash::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("PubKeyHash").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "toBuffer", ToBuffer);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("PubKeyHash").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

PubKeyHash::PubKeyHash(){

}

v8::Local<v8::Object> PubKeyHash::Make(const Coin::PubKeyHash &hash) {
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    auto instance = cons->NewInstance(Nan::GetCurrentContext()).ToLocalChecked();
    PubKeyHash* pubKeyHash = ObjectWrap::Unwrap<PubKeyHash>(instance);
    pubKeyHash->setPubKeyHash(hash);
    return scope.Escape(instance);
}

NAN_METHOD(PubKeyHash::New) {
  if (info.IsConstructCall()) {
    PubKeyHash *obj = new PubKeyHash();

    if(info.Length() > 0){
        if(info[0]->IsUint8Array()) {
            Coin::PubKeyHash hash(NodeBufferToUCharVector(info[0]));
            obj->setPubKeyHash(hash);
        }

        if(info[0]->IsString()){
            Coin::PubKeyHash hash(StringToUCharVector(info[0]));
            obj->setPubKeyHash(hash);
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

NAN_METHOD(PubKeyHash::ToBuffer) {
    PubKeyHash* pubKeyHash = ObjectWrap::Unwrap<PubKeyHash>(info.This());
    auto data = pubKeyHash->_pubKeyHash.toUCharVector();
    info.GetReturnValue().Set(UCharVectorToNodeBuffer(data));
}

void PubKeyHash::setPubKeyHash(const Coin::PubKeyHash &hash) {
    _pubKeyHash = hash;
}
