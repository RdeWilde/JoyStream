#include "torrent_info.h"


Nan::Persistent<v8::Function> TorrentInfo::constructor;

NAN_MODULE_INIT(TorrentInfo::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("TorrentInfo").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);


  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("TorrentInfo").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

Local<Object> TorrentInfo::New(boost::shared_ptr<const libtorrent::torrent_info>& ti) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    Nan::ObjectWrap::Unwrap<TorrentInfo>(obj.ToLocalChecked())->torrent_info_ = ti;

    return scope.Escape(obj.ToLocalChecked());
};

NAN_METHOD(TorrentInfo::NewInstance) {
  Nan::HandleScope scope;

  TorrentInfo* obj = new TorrentInfo();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};
