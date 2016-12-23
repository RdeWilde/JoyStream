#include "torrent_status.h"

using namespace v8;

Nan::Persistent<Function> TorrentStatusWrap::constructor;

NAN_MODULE_INIT(TorrentStatusWrap::Init) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("TorrentStatusWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("TorrentStatusWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

Local<Object> TorrentStatusWrap::New(const libtorrent::torrent_status* ts) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    if (ts) {
        Nan::ObjectWrap::Unwrap<TorrentStatusWrap>(obj.ToLocalChecked())->torrent_status_ = ts;
    } else {
      obj = Nan::New<Object>();
    }
    return scope.Escape(obj.ToLocalChecked());
};

TorrentStatusWrap::TorrentStatusWrap() {
  torrent_status_ = NULL;
};

TorrentStatusWrap::~TorrentStatusWrap() {
  if (torrent_status_ != NULL)
    delete torrent_status_;
};

NAN_METHOD(TorrentStatusWrap::NewInstance) {
  Nan::HandleScope scope;

  TorrentStatusWrap* obj = new TorrentStatusWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};
