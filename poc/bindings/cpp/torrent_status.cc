#include "torrent_status.h"

using namespace v8;

Nan::Persistent<Function> TorrentStatusWrap::constructor;

NAN_MODULE_INIT(TorrentStatusWrap::Init) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("TorrentStatusWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "infoHash", info_hash);
  Nan::SetPrototypeMethod(tpl, "state", state);
  Nan::SetPrototypeMethod(tpl, "progress", progress);


  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("TorrentStatusWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

Local<Object> TorrentStatusWrap::New(libtorrent::torrent_status ts) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());
    Nan::ObjectWrap::Unwrap<TorrentStatusWrap>(obj.ToLocalChecked())->torrent_status_ = ts;

    return scope.Escape(obj.ToLocalChecked());
};

NAN_METHOD(TorrentStatusWrap::NewInstance) {
  Nan::HandleScope scope;

  TorrentStatusWrap* obj = new TorrentStatusWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};

NAN_METHOD(TorrentStatusWrap::info_hash) {
    Nan::HandleScope scope;

    libtorrent::torrent_status* ts = TorrentStatusWrap::Unwrap(info.This());

    info.GetReturnValue().Set(Nan::New<String>(libtorrent::to_hex(ts->info_hash.to_string())).ToLocalChecked());
};

NAN_METHOD(TorrentStatusWrap::state) {
    Nan::HandleScope scope;

    libtorrent::torrent_status* ts = TorrentStatusWrap::Unwrap(info.This());

    info.GetReturnValue().Set(Nan::New<Number>(ts->state));
};

NAN_METHOD(TorrentStatusWrap::progress) {
    Nan::HandleScope scope;

    libtorrent::torrent_status* ts = TorrentStatusWrap::Unwrap(info.This());

    info.GetReturnValue().Set(Nan::New<Number>(ts->progress));
};
