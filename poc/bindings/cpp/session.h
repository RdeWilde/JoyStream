#ifndef SESSION_H
#define SESSION_H

#include <nan.h>
#include "alert.h"
#include "torrent_handle.h"
#include <iostream>
#include <extension/extension.hpp>
#include <joystream_libtorrent_session/Session.hpp>


class SessionWrap : public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);
    static libtorrent::sha1_hash object_to_sha1_hash(Local<Value> infoHash);

  private:
    Session session_;
    static Nan::Persistent<v8::Function> constructor;

    static NAN_METHOD(New);
    static NAN_METHOD(AddTorrent);
    static NAN_METHOD(RemoveTorrent);
    static NAN_METHOD(ListenPort);
    static NAN_METHOD(PostTorrentUpdates);
    static NAN_METHOD(Pause);
    static NAN_METHOD(IsPaused);
    static NAN_METHOD(Resume);
    static NAN_METHOD(FindTorrent);
    static NAN_METHOD(PopAlerts);
    static NAN_METHOD(SetAlertNotify);

    static NAN_METHOD(dht_announce);
    static NAN_METHOD(dht_get_peers);

};

#endif
