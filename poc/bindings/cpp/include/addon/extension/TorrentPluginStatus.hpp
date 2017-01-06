#ifndef TORRENT_PLUGIN_STATUS_H
#define TORRENT_PLUGIN_STATUS_H

#include <nan.h>
#include <extension/Status.hpp>

using namespace v8;

class TorrentPluginStatus: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);
      static Local<Object> New(const joystream::extension::status::TorrentPlugin& tp);
      static joystream::extension::status::TorrentPlugin* Unwrap(const Local<Object>& obj) {
        TorrentPluginStatus* tps = Nan::ObjectWrap::Unwrap<TorrentPluginStatus>(obj);
        return &tps->torrent_plugin_status_;
      };

    private:
      joystream::extension::status::TorrentPlugin torrent_plugin_status_;
      static Nan::Persistent<Function> constructor;

      static NAN_METHOD(NewInstance);
      static NAN_GETTER(info_hash);

};

#endif
