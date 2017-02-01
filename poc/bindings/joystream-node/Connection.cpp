#include "Connection.hpp"

namespace joystream {
namespace node {

Nan::Persistent<v8::Function> Connection::constructor;

NAN_MODULE_INIT(Connection::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Connection").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Connection").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

v8::Local<v8::Object> Connection::NewInstance(const joystream::protocol_session::status::Connection<libtorrent::tcp::endpoint>& c) {
  Nan::EscapableHandleScope scope;

  v8::Local<v8::Function> cons = Nan::New(constructor);
  Nan::MaybeLocal<v8::Object> obj = cons->NewInstance(Nan::GetCurrentContext());

  Nan::ObjectWrap::Unwrap<Connection>(obj.ToLocalChecked())->connection_ = c;

  return scope.Escape(obj.ToLocalChecked());
}

NAN_METHOD(Connection::New) {
  if (info.IsConstructCall()) {
    Connection* obj = new Connection();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    v8::Local<v8::Function> cons = Nan::New(constructor);
    info.GetReturnValue().Set(cons->NewInstance(Nan::GetCurrentContext()).ToLocalChecked());
  }
}


}
}
