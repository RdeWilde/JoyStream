#include "torrent_status.h"

// Wrapper Impl

Nan::Persistent<v8::Function> TorrentStatusWrap::constructor;

NAN_MODULE_INIT(TorrentStatusWrap::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("TorrentStatusWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(0);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("TorrentStatusWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(TorrentStatusWrap::New) {
  if (info.IsConstructCall()) {
    TorrentStatusWrap *obj = new TorrentStatusWrap();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {info[0]};
    v8::Local<v8::Function> cons = Nan::New(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}
