#include "torrent_info.h"


Nan::Persistent<v8::Function> TorrentInfo::constructor;

NAN_MODULE_INIT(TorrentInfo::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("TorrentInfo").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "name", name);
  Nan::SetPrototypeMethod(tpl, "totalSize", total_size);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("TorrentInfo").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

Local<Object> TorrentInfo::New(boost::shared_ptr<const libtorrent::torrent_info> ti) {
    Nan::EscapableHandleScope scope;

    v8::Local<v8::Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<v8::Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    Nan::ObjectWrap::Unwrap<TorrentInfo>(obj.ToLocalChecked())->torrent_info_ = ti;

    return scope.Escape(obj.ToLocalChecked());
}

NAN_METHOD(TorrentInfo::NewInstance) {
  Nan::HandleScope scope;

  TorrentInfo* obj = new TorrentInfo();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(TorrentInfo::name) {

    info.GetReturnValue().Set(Nan::New<v8::String>(TorrentInfo::Unwrap(info.This())->name()).ToLocalChecked());
}

NAN_METHOD(TorrentInfo::total_size) {

    std::int64_t total_size = TorrentInfo::Unwrap(info.This())->total_size();

    info.GetReturnValue().Set(Nan::New<v8::Number>(total_size));
}
