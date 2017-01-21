#ifndef SESSION_H
#define SESSION_H

#include <nan.h>
#include "alert.h"
#include "torrent_handle.h"
#include "plugin.hpp" //
#include <iostream>
//#include <extension/extension.hpp>
#include <joystream_libtorrent_session/Session.hpp>


class SessionWrap : public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);
    static libtorrent::sha1_hash object_to_sha1_hash(Local<Value> infoHash);

  private:
    Session session_;

    // Alert converters associated with installed plugins
    // Is populated by add_extension calls
    std::vector<libtorrent::node::plugin::converter> _converters;

    static Nan::Persistent<v8::Function> constructor;

    static NAN_METHOD(New);
    static NAN_METHOD(add_torrent);
    static NAN_METHOD(remove_torrent);
    static NAN_METHOD(listen_port);
    static NAN_METHOD(post_torrent_updates);
    static NAN_METHOD(pause);
    static NAN_METHOD(is_paused);
    static NAN_METHOD(resume);
    static NAN_METHOD(find_torrent);
    static NAM_METHOD(add_extension);
    static NAN_METHOD(pop_alerts);
    static NAN_METHOD(set_alert_notify);
    static NAN_METHOD(dht_announce);
    static NAN_METHOD(dht_get_peers);

};

#endif
