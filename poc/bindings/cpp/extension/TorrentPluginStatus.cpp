#include <addon/extension/TorrentPluginStatus.hpp>

using namespace v8;

Nan::Persistent<Function> TorrentPluginStatus::constructor;

NAN_MODULE_INIT(TorrentPluginStatus::Init) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("TorrentPluginStatus").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Local<ObjectTemplate> inst = tpl->InstanceTemplate();
  Nan::SetAccessor(inst, Nan::New("infoHash").ToLocalChecked(), TorrentPluginStatus::info_hash);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("TorrentPluginStatus").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

Local<Object> TorrentPluginStatus::New(const joystream::extension::status::TorrentPlugin& tp) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    Nan::ObjectWrap::Unwrap<TorrentPluginStatus>(obj.ToLocalChecked())->torrent_plugin_status_ = tp;

    return scope.Escape(obj.ToLocalChecked());
};

NAN_METHOD(TorrentPluginStatus::NewInstance) {
  TorrentPluginStatus* obj = new TorrentPluginStatus();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};

NAN_GETTER(TorrentPluginStatus::info_hash) {
  libtorrent::sha1_hash info_hash = TorrentPluginStatus::Unwrap(info.This())->infoHash;

  info.GetReturnValue().Set(Nan::New<String>(libtorrent::to_hex(info_hash.to_string())).ToLocalChecked());
};
