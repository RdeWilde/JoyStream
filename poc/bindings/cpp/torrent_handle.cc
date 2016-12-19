#include "torrent_handle.h"

// Wrapper Impl

Nan::Persistent<v8::Function> TorrentHandleWrap::constructor;

NAN_MODULE_INIT(TorrentHandleWrap::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("TorrentHandleWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("TorrentHandleWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}
