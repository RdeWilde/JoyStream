#ifndef TORRENT_HANDLE_H
#define TORRENT_HANDLE_H

#include <nan.h>
#include <libtorrent/torrent_handle.hpp>

class TorrentHandleWrap : public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> New(const libtorrent::torrent_handle& th);

  private:
    explicit TorrentHandleWrap();
    ~TorrentHandleWrap();

    static NAN_METHOD(NewInstance);
    static Nan::Persistent<v8::Function> constructor;

     libtorrent::torrent_handle* th_;
};

#endif
