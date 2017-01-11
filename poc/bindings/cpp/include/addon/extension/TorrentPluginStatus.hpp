#ifndef JOYSTREAM_NODE_ADDON_EXTENSION_TORRENT_PLUGIN_STATUS_H
#define JOYSTREAM_NODE_ADDON_EXTENSION_TORRENT_PLUGIN_STATUS_H

#include <nan.h>
#include <extension/Status.hpp>

namespace joystream {
  namespace addon {
    namespace extension {

      class TorrentPluginStatus: public Nan::ObjectWrap {
          public:
            static NAN_MODULE_INIT(Init);
            static v8::Local<v8::Object> NewInstance(const joystream::extension::status::TorrentPlugin& tp);

          private:
            joystream::extension::status::TorrentPlugin torrent_plugin_status_;
            static Nan::Persistent<v8::Function> constructor;

            static NAN_METHOD(New);
            static NAN_GETTER(info_hash);

      };
    }
  }
}

#endif
