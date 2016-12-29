#include "torrent_info.h"


Nan::Persistent<v8::Function> TorrentInfoWrap::constructor;

NAN_MODULE_INIT(TorrentInfoWrap::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("TorrentInfoWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);


  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("TorrentInfoWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

Local<Object> TorrentInfoWrap::New(const libtorrent::torrent_info* ti) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    Nan::ObjectWrap::Unwrap<TorrentInfoWrap>(obj.ToLocalChecked())->torrent_info_ = ti;

    return scope.Escape(obj.ToLocalChecked());
};

NAN_METHOD(TorrentInfoWrap::NewInstance) {
  Nan::HandleScope scope;

  TorrentInfoWrap* obj = new TorrentInfoWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};
