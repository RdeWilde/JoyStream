#ifndef TORRENT_STATUS_H
#define TORRENT_STATUS_H

#include <nan.h>
#include <libtorrent/torrent_status.hpp>

using namespace v8;

class TorrentStatusWrap: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);
      static Local<Object> New(const libtorrent::torrent_status* ts);
      static const libtorrent::torrent_status* Unwrap(const Local<Object>& obj) {
        return Nan::ObjectWrap::Unwrap<TorrentStatusWrap>(obj)->torrent_status_;
      };

    private:
      explicit TorrentStatusWrap();
      ~TorrentStatusWrap();

      static NAN_METHOD(NewInstance);
      static NAN_METHOD(info_hash);

      static Nan::Persistent<Function> constructor;
      const libtorrent::torrent_status* torrent_status_;

};

#endif
