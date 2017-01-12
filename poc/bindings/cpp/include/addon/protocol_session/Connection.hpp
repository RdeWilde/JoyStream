#ifndef JOYSTREAM_NODE_ADDON_PROTOCOL_SESSION_CONNECTION_H
#define JOYSTREAM_NODE_ADDON_PROTOCOL_SESSION_CONNECTION_H

#include <nan.h>
#include <protocol_session/Status.hpp>
#include <libtorrent/socket.hpp>

namespace joystream {
namespace addon {
namespace protocol_session {

class Connection: public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> NewInstance(const joystream::protocol_session::status::Connection<libtorrent::tcp::endpoint>& c);

  private:
    joystream::protocol_session::status::Connection<libtorrent::tcp::endpoint> connection_;
    static Nan::Persistent<v8::Function> constructor;

    static NAN_METHOD(New);

};

}}}
#endif
