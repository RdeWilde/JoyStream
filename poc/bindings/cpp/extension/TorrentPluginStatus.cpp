#include <addon/extension/TorrentPluginStatus.hpp>

namespace joystream {
  namespace addon {
    namespace extension {

      Nan::Persistent<v8::Function> TorrentPluginStatus::constructor;

      NAN_MODULE_INIT(TorrentPluginStatus::Init) {
        v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
        tpl->SetClassName(Nan::New("TorrentPluginStatus").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        v8::Local<v8::ObjectTemplate> inst = tpl->InstanceTemplate();
        Nan::SetAccessor(inst, Nan::New("infoHash").ToLocalChecked(), TorrentPluginStatus::info_hash);

        constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
        Nan::Set(target, Nan::New("TorrentPluginStatus").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
      }

      v8::Local<v8::Object> TorrentPluginStatus::NewInstance(const joystream::extension::status::TorrentPlugin& tp) {
          Nan::EscapableHandleScope scope;

          v8::Local<v8::Function> cons = Nan::New(constructor);
          Nan::MaybeLocal<v8::Object> obj = cons->NewInstance(Nan::GetCurrentContext());

          Nan::ObjectWrap::Unwrap<TorrentPluginStatus>(obj.ToLocalChecked())->torrent_plugin_status_ = tp;

          return scope.Escape(obj.ToLocalChecked());
      }

      NAN_METHOD(TorrentPluginStatus::New) {
        if (info.IsConstructCall()) {
          TorrentPluginStatus* obj = new TorrentPluginStatus();
          obj->Wrap(info.This());
          info.GetReturnValue().Set(info.This());
        } else {
          v8::Local<v8::Function> cons = Nan::New(constructor);
          info.GetReturnValue().Set(cons->NewInstance(Nan::GetCurrentContext()).ToLocalChecked());
        }
      }

      NAN_GETTER(TorrentPluginStatus::info_hash) {
        auto tps = Nan::ObjectWrap::Unwrap<TorrentPluginStatus>(info.This());
        libtorrent::sha1_hash info_hash = tps->torrent_plugin_status_.infoHash;

        info.GetReturnValue().Set(Nan::New<v8::String>(libtorrent::to_hex(info_hash.to_string())).ToLocalChecked());
      }
    }
  }
}
