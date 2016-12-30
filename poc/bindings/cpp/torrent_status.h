#ifndef TORRENT_STATUS_H
#define TORRENT_STATUS_H

#include <nan.h>
#include <libtorrent/torrent_status.hpp>

using namespace v8;

class TorrentStatusWrap: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);
      static Local<Object> New(libtorrent::torrent_status ts);
      static libtorrent::torrent_status* Unwrap(const Local<Object>& obj) {
        TorrentStatusWrap* ts = Nan::ObjectWrap::Unwrap<TorrentStatusWrap>(obj);
        return &ts->torrent_status_;
      };

    private:
      libtorrent::torrent_status torrent_status_;
      static Nan::Persistent<Function> constructor;

      static NAN_METHOD(NewInstance);
      static NAN_GETTER(info_hash);
      static NAN_GETTER(state);
      static NAN_GETTER(progress);

};

#endif
