#include "peer_info.h"

using namespace v8;

Nan::Persistent<Function> PeerInfoWrap::constructor;

NAN_MODULE_INIT(PeerInfoWrap::Init) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("PeerInfoWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("PeerInfoWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

Local<Object> PeerInfoWrap::New(const libtorrent::peer_info* pi) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    if(pi) {
        Nan::ObjectWrap::Unwrap<PeerInfoWrap>(obj.ToLocalChecked())->peer_info_ = pi;
    } else {
      obj = Nan::New<Object>();
    }
    return scope.Escape(obj.ToLocalChecked());
};

PeerInfoWrap::PeerInfoWrap() {
  peer_info_ = NULL;
};

PeerInfoWrap::~PeerInfoWrap() {
  if (peer_info_ != NULL)
    delete peer_info_;
};

NAN_METHOD(PeerInfoWrap::NewInstance) {
  Nan::HandleScope scope;

  PeerInfoWrap* obj = new PeerInfoWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};
