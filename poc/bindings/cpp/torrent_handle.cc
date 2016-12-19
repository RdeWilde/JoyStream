#include "torrent_handle.h"

// Wrapper Impl

Nan::Persistent<v8::Function> TorrentHandleWrap::constructor;

NAN_MODULE_INIT(TorrentHandleWrap::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("TorrentHandleWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("TorrentHandleWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

TorrentHandleWrap::TorrentHandleWrap() {
}

TorrentHandleWrap::~TorrentHandleWrap() {
}


v8::Local<v8::Object> TorrentHandleWrap::New(const libtorrent::torrent_handle& th) {
    Nan::EscapableHandleScope scope;

    v8::Local<v8::Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<v8::Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    Nan::ObjectWrap::Unwrap<TorrentHandleWrap>(obj.ToLocalChecked())->th_ = new libtorrent::torrent_handle(th);

    return scope.Escape(obj.ToLocalChecked());
};

NAN_METHOD(TorrentHandleWrap::NewInstance) {
  Nan::HandleScope scope;

  if (!info.IsConstructCall()) {
    Nan::ThrowTypeError("Use the new operator to create instances of this object.");
    return;
  }

  TorrentHandleWrap* obj = new TorrentHandleWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};
