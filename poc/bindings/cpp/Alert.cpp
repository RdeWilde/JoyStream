/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 12 2017
 */

#include "Alert.hpp"
#include "torrent_handle.h"
#include "endpoint.h"
#include "utils.hpp"

#include <chrono>
#include <ctime>

#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>

#define TIMESTAMP_KEY "timestamp"
#define TYPE_KEY "type"
#define WHAT_KEY "what"
#define MESSAGE_KEY "message"
#define CATEGORY_KEY "category"
#define HANDLE_KEY "handle"
#define TORRENT_NAME_KEY "torrentName"
#define IP_KEY "ip"
#define PID_KEY "pid"
#define TRACKER_URL_KEY "trackerURL"
#define INFO_HASH_KEY "infoHash"

namespace joystream {
namespace node {
namespace alert_types {

v8::Local<v8::Object> toObject(const libtorrent::alert & a) {

  v8::Local<v8::Object> o = Nan::New<v8::Object>();

  SET_CONST_CHAR(o, TIMESTAMP_KEY, std::ctime(std::chrono::system_clock::to_time_t(a.timestamp())));
  SET_UINT32(o, TYPE_KEY, a.type());
  SET_STD_STRING(o, WHAT_KEY, a.what());
  SET_STD_STRING(o, MESSAGE_KEY, a.message().c_str());
  SET_UINT32(o, CATEGORY_KEY, a.category());

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::torrent_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::alert &>(a));

  SET_VAL(o, HANDLE_KEY, TorrentHandleWrap::New(a.handle))
  SET_CONST_CHAR(o, TORRENT_NAME_KEY, a.torrent_name());
}

v8::Local<v8::Object> toObject(const libtorrent::peer_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  SET_VAL(o, IP_KEY, EndpointWrap::New(a.ip))
  // TODO: peer_id class
  //SET_OBJ(o, PID_KEY,) // pid;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::tracker_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  SET_CONST_CHAR(o, TRACKER_URL_KEY, a.tracker_url());

  return o;
}

/* * * * * * * * * * * *
 *
 *  Non-virtual alerts
 *
 * * * * * * * * * * * */

 v8::Local<v8::Object> toObject(const libtorrent::torrent_added_alert final & a) {
   v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

   return o;
 }

 v8::Local<v8::Object> toObject(const libtorrent::torrent_removed_alert final & a) {
   v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

   SET_VAL(o, INFO_HASH_KEY, a.info_hash)

   return o;
 }

v8::Local<v8::Object> toObject(const libtorrent::dht_get_peers_reply_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::alert &>(a));

  //SET_STR(o, "info_hash", a.info_hash.)
  SET_INT(o, "num_peers", a.num_peers())
  // std::vector<tcp::endpoint> peers() const

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::listen_succeeded_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::alert &>(a), o);

  //alert.address;
  //alert.port;
  //alert.socket_type; // libtorrent::socket_type_t

}

v8::Local<v8::Object> toObject(const libtorrent::state_update_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::alert &>(a), o);

  // std::vector<torrent_status> const status;

}

// Non-virtual (torrent) alerts

v8::Local<v8::Object> toObject(const libtorrent::metadata_received_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);
}

v8::Local<v8::Object> toObject(const libtorrent::metadata_failed_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  // indicates what failed when parsing the metadata. This error is
  // what's returned from lazy_bdecode().
  // error_code const error;

}

v8::Local<v8::Object> toObject(const libtorrent::add_torrent_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  // a copy of the parameters used when adding the torrent, it can be used
  // to identify which invocation to ``async_add_torrent()`` caused this alert.
  //add_torrent_params const params;

  // set to the error, if one occurred while adding the torrent.
  //error_code const error;

}

v8::Local<v8::Object> toObject(const libtorrent::torrent_finished_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);
}

v8::Local<v8::Object> toObject(const libtorrent::torrent_removed_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  // is required, since handle has expired most likeyl
  //sha1_hash const info_hash;
}

v8::Local<v8::Object> toObject(const libtorrent::torrent_resumed_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);
}

v8::Local<v8::Object> toObject(const libtorrent::save_resume_data_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  // points to the resume data.
	//std::shared_ptr<entry> const resume_data;
}

v8::Local<v8::Object> toObject(const libtorrent::save_resume_data_failed_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  // the error code from the resume_data failure
	// error_code const error;

}

v8::Local<v8::Object> toObject(const libtorrent::torrent_paused_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);
}

v8::Local<v8::Object> toObject(const libtorrent::torrent_checked_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);
}

v8::Local<v8::Object> toObject(const libtorrent::read_piece_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  // error_code const error;
  // boost::shared_array<char> const buffer;
  // piece_index_t const piece;
  // int const size;

}

v8::Local<v8::Object> toObject(const libtorrent::piece_finished_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  // the index of the piece that finished
  // piece_index_t const piece_index;

}

// Non-virtual (peer) alerts

v8::Local<v8::Object> toObject(const libtorrent::peer_connect_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::peer_alert &>(a), o);

  // int const socket_type;

}

v8::Local<v8::Object> toObject(const libtorrent::peer_disconnected_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::peer_alert &>(a), o);

  // the kind of socket this peer was connected over
  //int const socket_type;

  // the operation or level where the error occurred. Specified as an
  // value from the operation_t enum. Defined in operations.hpp.
  //operation_t const operation;

  // tells you what error caused peer to disconnect.
  // error_code const error;

  // the reason the peer disconnected (if specified)
  // close_reason_t const reason;

}

}}}
