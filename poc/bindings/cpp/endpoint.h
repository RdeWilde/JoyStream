#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <nan.h>
#include <libtorrent/socket.hpp>

using namespace v8;

class EndpointWrap: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);
      static Local<Object> New(const libtorrent::tcp::endpoint ep);
      static libtorrent::tcp::endpoint Unwrap(const Local<Object>& obj) {
        return Nan::ObjectWrap::Unwrap<EndpointWrap>(obj)->endpoint_;
      };

    private:
      explicit EndpointWrap();
      ~EndpointWrap();

      static NAN_METHOD(NewInstance);

      static Nan::Persistent<Function> constructor;
      libtorrent::tcp::endpoint endpoint_;

};

#endif
