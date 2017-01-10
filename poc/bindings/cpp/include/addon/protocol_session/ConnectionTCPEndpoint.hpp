#ifndef JOYSTREAM_NODE_ADDON_PROTOCOL_SESSION_CONNECTION_H
#define JOYSTREAM_NODE_ADDON_PROTOCOL_SESSION_CONNECTION_H

#include <nan.h>
#include <protocol_session/Status.hpp>
#include <libtorrent/socket.hpp>

namespace joystream {
  namespace addon {
    namespace protocol_session {

      class ConnectionTCPEndpoint: public Nan::ObjectWrap {
          public:
            static NAN_MODULE_INIT(Init);
            static v8::Local<v8::Object> New(const joystream::protocol_session::status::Connection<libtorrent::tcp::endpoint>& c);
            static joystream::protocol_session::status::Connection<libtorrent::tcp::endpoint>* Unwrap(const v8::Local<v8::Object>& obj) {
              ConnectionTCPEndpoint* c = Nan::ObjectWrap::Unwrap<ConnectionTCPEndpoint>(obj);
              return &c->connection_;
            };

          private:
            joystream::protocol_session::status::Connection<libtorrent::tcp::endpoint> connection_;
            static Nan::Persistent<v8::Function> constructor;

            static NAN_METHOD(NewInstance);

      };
    }
  }
}

#endif
