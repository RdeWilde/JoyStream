#ifndef JOYSTREAM_NODE_ADDON_EXTENSION_PEER_PLUGIN_STATUS_H
#define JOYSTREAM_NODE_ADDON_EXTENSION_PEER_PLUGIN_STATUS_H

#include <nan.h>
#include <extension/Status.hpp>

using namespace v8;

class PeerPluginStatus: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);
      static Local<Object> New(const joystream::extension::status::PeerPlugin& pp);
      static joystream::extension::status::PeerPlugin* Unwrap(const Local<Object>& obj) {
        PeerPluginStatus* tps = Nan::ObjectWrap::Unwrap<PeerPluginStatus>(obj);
        return &tps->peer_plugin_status_;
      };

    private:
      joystream::extension::status::PeerPlugin peer_plugin_status_;
      static Nan::Persistent<Function> constructor;

      static NAN_METHOD(NewInstance);
};

#endif
