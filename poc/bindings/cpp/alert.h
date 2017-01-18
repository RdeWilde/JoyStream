#ifndef ALERT_H
#define ALERT_H

#include <nan.h>
#include "torrent_handle.h"
#include "torrent_status.h"
#include "add_torrent_params.hpp"
#include "endpoint.h"
#include "bencode.h"
#include <addon/extension/TorrentPluginStatus.hpp>
#include <addon/extension/PeerPluginStatus.hpp>
#include <addon/protocol_session/Connection.hpp>
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
      const libtorrent::alert* alert_;
      static Nan::Persistent<Function> constructor;

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

      // dht_get_peers_reply_alert
      static NAN_METHOD(peers);

      // listen_succeeded_alert
      static NAN_METHOD(endpoint);

      // listen_succeeded_alert
      static NAN_METHOD(ip);

      // ResultRequest
      static NAN_METHOD(loaded_callback);

      // save_resume_data_alert
      static NAN_METHOD(resume_data);

      // TorrentPluginStatusUpdateAlert
      static NAN_METHOD(statuses);

      // ConnectionAddedToSession
      static NAN_METHOD(connection_status);

      // TorrentPluginAdded
      static NAN_METHOD(torrent_plugin_status);

      // PeerPluginAdded
      static NAN_METHOD(peer_plugin_status);


};

#endif
