#include "endpoint.h"

using namespace v8;

Nan::Persistent<Function> EndpointWrap::constructor;

NAN_MODULE_INIT(EndpointWrap::Init) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("EndpointWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "address", address);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("EndpointWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

Local<Object> EndpointWrap::New(libtorrent::tcp::endpoint ep) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    Nan::ObjectWrap::Unwrap<EndpointWrap>(obj.ToLocalChecked())->endpoint_ = ep;

    return scope.Escape(obj.ToLocalChecked());
};

NAN_METHOD(EndpointWrap::NewInstance) {
  Nan::HandleScope scope;

  EndpointWrap* obj = new EndpointWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};

NAN_METHOD(EndpointWrap::address) {
  Nan::HandleScope scope;

  libtorrent::tcp::endpoint* ep = EndpointWrap::Unwrap(info.This());
  std::string address = ep->address().to_string();

  info.GetReturnValue().Set(Nan::New<String>(address).ToLocalChecked());
};
