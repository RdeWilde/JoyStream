/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 12 2017
 */

#include "Alert.hpp"
#include "torrent_handle.h"
#include "endpoint.h"

#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>

#define SET_INT(o, key, value) o->Set(Nan::New(key).ToLocalChecked(), Nan::New(value));
#define SET_STR(o, key, value) o->Set(Nan::New(key).ToLocalChecked(), Nan::New(value).ToLocalChecked());
#define SET_OBJ(o, key, value) o->Set(Nan::New(key).ToLocalChecked(), value);

namespace joystream {
namespace node_addon {

void Init() {

  // some type macro to recover what type of alert it is
  // categories
  // ....

  // enums to support

  // libtorrent::static_category
  // libtorrent::socket_type_t
  // operation_t
  // error_code
  // close_reason_t

}

void toObject(const libtorrent::alert & a, v8::Local<v8::Object> & o) {
  SET_INT(o, "type", a.type())
  SET_STR(o, "what", a.what())
  SET_STR(o, "message", a.message().c_str());
  SET_INT(o, "category", a.category());
}

void toObject(const libtorrent::torrent_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::alert &>(a), o);

  SET_OBJ(o, "handle", TorrentHandleWrap::New(a.handle))
}

void toObject(const libtorrent::peer_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  SET_OBJ(o, "ip", EndpointWrap::New(a.ip))
  //SET_OBJ(o, "pid",) // pid;
}

// Non-virtual alerts

void toObject(const libtorrent::dht_get_peers_reply_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::alert &>(a), o);

  //SET_STR(o, "info_hash", a.info_hash.)
  SET_INT(o, "num_peers", a.num_peers())
  // std::vector<tcp::endpoint> peers() const
}

void toObject(const libtorrent::listen_succeeded_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::alert &>(a), o);

  //alert.address;
  //alert.port;
  //alert.socket_type; // libtorrent::socket_type_t

}

void toObject(const libtorrent::state_update_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::alert &>(a), o);

  // std::vector<torrent_status> const status;

}

// Non-virtual (torrent) alerts

void toObject(const libtorrent::metadata_received_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);
}

void toObject(const libtorrent::metadata_failed_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  // indicates what failed when parsing the metadata. This error is
  // what's returned from lazy_bdecode().
  // error_code const error;

}

void toObject(const libtorrent::add_torrent_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  // a copy of the parameters used when adding the torrent, it can be used
  // to identify which invocation to ``async_add_torrent()`` caused this alert.
  //add_torrent_params const params;

  // set to the error, if one occurred while adding the torrent.
  //error_code const error;

}

void toObject(const libtorrent::torrent_finished_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);
}

void toObject(const libtorrent::torrent_removed_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  // is required, since handle has expired most likeyl
  //sha1_hash const info_hash;
}

void toObject(const libtorrent::torrent_resumed_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);
}

void toObject(const libtorrent::save_resume_data_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  // points to the resume data.
	//std::shared_ptr<entry> const resume_data;
}

void toObject(const libtorrent::save_resume_data_failed_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  // the error code from the resume_data failure
	// error_code const error;

}

void toObject(const libtorrent::torrent_paused_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);
}

void toObject(const libtorrent::torrent_checked_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);
}

void toObject(const libtorrent::read_piece_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  // error_code const error;
  // boost::shared_array<char> const buffer;
  // piece_index_t const piece;
  // int const size;

}

void toObject(const libtorrent::piece_finished_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  // the index of the piece that finished
  // piece_index_t const piece_index;

}

// Non-virtual (peer) alerts

void toObject(const libtorrent::peer_connect_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::peer_alert &>(a), o);

  // int const socket_type;

}

void toObject(const libtorrent::peer_disconnected_alert & a, v8::Local<v8::Object> & o) {
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

}
}
