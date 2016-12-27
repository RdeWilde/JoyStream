#include "endpoint.h"

using namespace v8;

Nan::Persistent<Function> EndpointWrap::constructor;

NAN_MODULE_INIT(EndpointWrap::Init) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("EndpointWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);


  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("EndpointWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

Local<Object> EndpointWrap::New(const libtorrent::tcp::endpoint* ep) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    if(ep) {
        Nan::ObjectWrap::Unwrap<EndpointWrap>(obj.ToLocalChecked())->endpoint_ = ep;
    } else {
      obj = Nan::New<Object>();
    }
    return scope.Escape(obj.ToLocalChecked());
};

EndpointWrap::EndpointWrap() {
  endpoint_ = NULL;
};

EndpointWrap::~EndpointWrap() {
  if (endpoint_ != NULL)
    delete endpoint_;
};

NAN_METHOD(EndpointWrap::NewInstance) {
  Nan::HandleScope scope;

  EndpointWrap* obj = new EndpointWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};
