#include "PeerPluginStatus.hpp"

namespace joystream {
namespace node {

Nan::Persistent<v8::Function> PeerPluginStatus::constructor;

NAN_MODULE_INIT(PeerPluginStatus::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("PeerPluginStatus").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("PeerPluginStatus").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

v8::Local<v8::Object> PeerPluginStatus::NewInstance(const joystream::extension::status::PeerPlugin& pp) {
  Nan::EscapableHandleScope scope;

  v8::Local<v8::Function> cons = Nan::New(constructor);
  Nan::MaybeLocal<v8::Object> obj = cons->NewInstance(Nan::GetCurrentContext());

  Nan::ObjectWrap::Unwrap<PeerPluginStatus>(obj.ToLocalChecked())->peer_plugin_status_ = pp;

  return scope.Escape(obj.ToLocalChecked());
}

NAN_METHOD(PeerPluginStatus::New) {
  if (info.IsConstructCall()) {
    PeerPluginStatus* obj = new PeerPluginStatus();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    v8::Local<v8::Function> cons = Nan::New(constructor);
    info.GetReturnValue().Set(cons->NewInstance(Nan::GetCurrentContext()).ToLocalChecked());
  }
}

}
}
