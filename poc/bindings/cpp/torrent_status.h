#ifndef TORRENT_STATUS_H
#define TORRENT_STATUS_H

#include <nan.h>

class TorrentStatusWrap : public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);

  private:
    explicit TorrentStatusWrap();
    ~TorrentStatusWrap();

    static NAN_METHOD(New);
    static Nan::Persistent<v8::Function> constructor;
};

#endif
