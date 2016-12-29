#include "peer_info.h"

using namespace v8;

Nan::Persistent<Function> PeerInfoWrap::constructor;

NAN_MODULE_INIT(PeerInfoWrap::Init) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("PeerInfoWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "ip", ip);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("PeerInfoWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

Local<Object> PeerInfoWrap::New(libtorrent::peer_info pi) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    Nan::ObjectWrap::Unwrap<PeerInfoWrap>(obj.ToLocalChecked())->peer_info_ = pi;

    return scope.Escape(obj.ToLocalChecked());
};

NAN_METHOD(PeerInfoWrap::NewInstance) {
  Nan::HandleScope scope;

  PeerInfoWrap* obj = new PeerInfoWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};

NAN_METHOD(PeerInfoWrap::ip) {
  Nan::HandleScope scope;

  libtorrent::peer_info* pi = PeerInfoWrap::Unwrap(info.This());

  info.GetReturnValue().Set(EndpointWrap::New(pi->ip));
};
