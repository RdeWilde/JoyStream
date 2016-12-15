#include "functions.h"

// Wrapper Impl

Nan::Persistent<v8::Function> SessionWrap::constructor;

NAN_MODULE_INIT(SessionWrap::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("SessionWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "addTorrent", AddTorrent);
  Nan::SetPrototypeMethod(tpl, "pause", Pause);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("SessionWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

SessionWrap::SessionWrap() {
}

SessionWrap::~SessionWrap() {
}

NAN_METHOD(SessionWrap::New) {
  if (info.IsConstructCall()) {
    SessionWrap *obj = new SessionWrap();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {info[0]};
    v8::Local<v8::Function> cons = Nan::New(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

NAN_METHOD(SessionWrap::AddTorrent) {
  Nan::HandleScope scope;

  if (info.Length() < 7) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  /*int uploadLimit = To<uint>(info[0]).FromJust();
  int downloadLimit = To<uint>(info[1]).FromJust();
  std::string name = To<std::string>(info[2]).FromJust();
  std::vector<char> resumeData = To<std::vector<char>>(info[3]).FromJust();
  std::string savePath = To<std::string>(info[3]).FromJust();*/
}

NAN_METHOD(SessionWrap::Pause) {
  Nan::HandleScope scope;

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());
  session_wrap->session_.s->pause();
  printf("Plugin pause");
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(SessionWrap::IsPaused) {
  Nan::HandleScope scope;

  SessionWrap* session_wrap = ObjectWrap::Unwrap<SessionWrap>(info.This());
  session_wrap->session_.s->pause();
  info.GetReturnValue().Set(Nan::Undefined());
}
