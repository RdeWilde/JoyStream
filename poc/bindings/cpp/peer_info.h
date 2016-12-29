#ifndef PEER_INFO_H
#define PEER_INFO_H

#include <nan.h>
#include <libtorrent/peer_info.hpp>

#include "endpoint.h"

using namespace v8;

class PeerInfoWrap: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);
      static Local<Object> New(libtorrent::peer_info pi);
      static libtorrent::peer_info* Unwrap(const Local<Object>& obj) {
        PeerInfoWrap* pi = Nan::ObjectWrap::Unwrap<PeerInfoWrap>(obj);
        return &pi->peer_info_;
      };

    private:
      static Nan::Persistent<Function> constructor;
      libtorrent::peer_info peer_info_;

      static NAN_METHOD(NewInstance);
      static NAN_METHOD(ip);
};

#endif
