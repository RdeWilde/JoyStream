#ifndef ALERT_H
#define ALERT_H

#include <nan.h>
#include "torrent_handle.h"
#include "torrent_status.h"
#include "add_torrent_params.h"
#include <extension/Alert.hpp>

using namespace v8;

/*
 * alert* pointer issue
 */

class AlertWrap: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);
      static Local<Object> New(const libtorrent::alert* a);
      static const libtorrent::alert* Unwrap(const Local<Object>& obj) {
        return Nan::ObjectWrap::Unwrap<AlertWrap>(obj)->alert_;
      };

    private:
      explicit AlertWrap();
      ~AlertWrap();

      static NAN_METHOD(NewInstance);
      static NAN_METHOD(what);
      static NAN_METHOD(type);
      static NAN_METHOD(message);
      static NAN_METHOD(category);

      // torrent_alert
      static NAN_METHOD(handle);

      //
      static NAN_METHOD(error);
      static NAN_METHOD(params);

      // dht_get_peers_reply_alert torrent_removed_alert
      static NAN_METHOD(info_hash);

      // state_update_alert
      static NAN_METHOD(status);

      static Nan::Persistent<Function> constructor;
      const libtorrent::alert* alert_;

};

#endif
