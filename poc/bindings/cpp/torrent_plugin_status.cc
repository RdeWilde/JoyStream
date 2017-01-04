#include "torrent_plugin_status.h"

using namespace v8;

Nan::Persistent<Function> TorrentPluginStatusWrap::constructor;

NAN_MODULE_INIT(TorrentPluginStatusWrap::Init) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("TorrentPluginStatus").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();
  Nan::SetAccessor(inst, Nan::New("infoHash").ToLocalChecked(), TorrentPluginStatusWrap::info_hash);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("TorrentPluginStatus").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

Local<Object> TorrentPluginStatusWrap::New(const joystream::extension::status::TorrentPlugin& tp) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    Nan::ObjectWrap::Unwrap<TorrentPluginStatusWrap>(obj.ToLocalChecked())->torrent_plugin_status_ = tp;

    return scope.Escape(obj.ToLocalChecked());
};

NAN_METHOD(TorrentPluginStatusWrap::NewInstance) {
  TorrentPluginStatusWrap* obj = new TorrentPluginStatusWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};

NAN_GETTER(TorrentPluginStatusWrap::info_hash) {
  libtorrent::sha1_hash info_hash = TorrentPluginStatusWrap::Unwrap(info.This())->infoHash;

  info.GetReturnValue().Set(Nan::New<String>(libtorrent::to_hex(info_hash.to_string())).ToLocalChecked());
};
