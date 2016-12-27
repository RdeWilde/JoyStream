#ifndef PEER_INFO_H
#define PEER_INFO_H

#include <nan.h>
#include <libtorrent/peer_info.hpp>

using namespace v8;

class PeerInfoWrap: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);
      static Local<Object> New(const libtorrent::peer_info* pi);
      static const libtorrent::peer_info* Unwrap(const Local<Object>& obj) {
        return Nan::ObjectWrap::Unwrap<PeerInfoWrap>(obj)->peer_info_;
      };

    private:
      explicit PeerInfoWrap();
      ~PeerInfoWrap();

      static Nan::Persistent<Function> constructor;
      const libtorrent::peer_info* peer_info_;

      static NAN_METHOD(NewInstance);

};

#endif
