#ifndef JOYSTREAM_NODE_ADDON_EXTENSION_PEER_PLUGIN_STATUS_H
#define JOYSTREAM_NODE_ADDON_EXTENSION_PEER_PLUGIN_STATUS_H

#include <nan.h>
#include <extension/Status.hpp>

namespace joystream {
namespace addon {
namespace extension {

class PeerPluginStatus: public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> NewInstance(const joystream::extension::status::PeerPlugin& pp);

  private:
    joystream::extension::status::PeerPlugin peer_plugin_status_;
    static Nan::Persistent<v8::Function> constructor;

    static NAN_METHOD(New);
};


}}}
#endif
