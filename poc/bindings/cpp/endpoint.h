#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <nan.h>
#include <libtorrent/socket.hpp>

using namespace v8;

class EndpointWrap: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);
      static Local<Object> New(libtorrent::tcp::endpoint ep);
      static libtorrent::tcp::endpoint* Unwrap(const Local<Object>& obj) {
        EndpointWrap* ep = Nan::ObjectWrap::Unwrap<EndpointWrap>(obj);
        return &ep->endpoint_;
      };

    private:
      static NAN_METHOD(NewInstance);
      static NAN_METHOD(address);

      static Nan::Persistent<Function> constructor;
      libtorrent::tcp::endpoint endpoint_;

};

#endif
