#include "endpoint.h"

using namespace v8;

Nan::Persistent<Function> EndpointWrap::constructor;

NAN_MODULE_INIT(EndpointWrap::Init) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("Endpoint").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();
  Nan::SetAccessor(inst, Nan::New("address").ToLocalChecked(), EndpointWrap::address);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Endpoint").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

Local<Object> EndpointWrap::New(libtorrent::tcp::endpoint ep) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    Nan::ObjectWrap::Unwrap<EndpointWrap>(obj.ToLocalChecked())->endpoint_ = ep;

    return scope.Escape(obj.ToLocalChecked());
};

NAN_METHOD(EndpointWrap::NewInstance) {
  EndpointWrap* obj = new EndpointWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};

NAN_GETTER(EndpointWrap::address) {
  libtorrent::tcp::endpoint* ep = EndpointWrap::Unwrap(info.This());
  std::string address = ep->address().to_string();

  info.GetReturnValue().Set(Nan::New<String>(address).ToLocalChecked());
};
