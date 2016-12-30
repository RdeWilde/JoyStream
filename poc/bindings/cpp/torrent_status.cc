#include "torrent_status.h"

using namespace v8;

Nan::Persistent<Function> TorrentStatusWrap::constructor;

NAN_MODULE_INIT(TorrentStatusWrap::Init) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("TorrentStatus").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();
  Nan::SetAccessor(inst, Nan::New("infoHash").ToLocalChecked(), TorrentStatusWrap::info_hash);
  Nan::SetAccessor(inst, Nan::New("state").ToLocalChecked(), TorrentStatusWrap::state);
  Nan::SetAccessor(inst, Nan::New("progress").ToLocalChecked(), TorrentStatusWrap::progress);


  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("TorrentStatus").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

Local<Object> TorrentStatusWrap::New(libtorrent::torrent_status ts) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());
    Nan::ObjectWrap::Unwrap<TorrentStatusWrap>(obj.ToLocalChecked())->torrent_status_ = ts;

    return scope.Escape(obj.ToLocalChecked());
};

NAN_METHOD(TorrentStatusWrap::NewInstance) {

  TorrentStatusWrap* obj = new TorrentStatusWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};

NAN_GETTER(TorrentStatusWrap::info_hash) {

    libtorrent::torrent_status* ts = TorrentStatusWrap::Unwrap(info.This());

    info.GetReturnValue().Set(Nan::New<String>(libtorrent::to_hex(ts->info_hash.to_string())).ToLocalChecked());
};

NAN_GETTER(TorrentStatusWrap::state) {

    libtorrent::torrent_status* ts = TorrentStatusWrap::Unwrap(info.This());

    info.GetReturnValue().Set(Nan::New<Number>(ts->state));
};

NAN_GETTER(TorrentStatusWrap::progress) {

    libtorrent::torrent_status* ts = TorrentStatusWrap::Unwrap(info.This());

    info.GetReturnValue().Set(Nan::New<Number>(ts->progress));
};
