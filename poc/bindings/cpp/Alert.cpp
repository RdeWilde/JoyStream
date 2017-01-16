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

/// Do we perhaps need to move these macroes at some point?
#define SET_INT(o, key, value) Nan::Set(o, Nan::New(key).ToLocalChecked(), Nan::New(value));
#define SET_STR(o, key, value) Nan::Set(o, Nan::New(key).ToLocalChecked(), Nan::New(value).ToLocalChecked());
#define SET_OBJ(o, key, value) Nan::Set(o, Nan::New(key).ToLocalChecked(), value);

namespace joystream {
namespace node_addon {

#define SET_ALERT_TYPE(o, alert_name) SET_INT(o, #alert_name, libtorrent::alert_name::type);

NAN_MODULE_INIT(InitAlertTypes) {

  v8::Local<v8::ObjectTemplate> tpl = Nan::New<v8::ObjectTemplate>();

  // do we need some sort of EscapableHandleScope ?????

  //SET_INT(alert_types, "torrent_checked_alert", libtorrent::torrent_checked_alert::type)
  SET_ALERT_TYPE(alert_types, torrent_checked_alert)

  //SET_ALERT_TYPE(alert_types, torrent_checked_alert)
  //SET_ALERT_TYPE(alert_types, torrent_checked_alert)
  //SET_ALERT_TYPE(alert_types, torrent_checked_alert)

  Nan::Set(target, Nan::New("alert_types").ToLocalChecked(), alert_types);
}

//#define SET_NOTIFICATION(o, notification_name) SET_INT();

NAN_MODULE_INIT(InitCategoryTypes) {



  //SET_NOTIFICATION(o, error_notification)

/**
      error_notification            = 0x1,
			peer_notification             = 0x2,
			port_mapping_notification     = 0x4,
			storage_notification          = 0x8,
			tracker_notification          = 0x10,
			debug_notification            = 0x20,
			status_notification           = 0x40,
			progress_notification         = 0x80,
			ip_block_notification         = 0x100,
			performance_warning           = 0x200,
			dht_notification              = 0x400,
			stats_notification            = 0x800,
#ifndef TORRENT_NO_DEPRECATE
			rss_notification              = 0x1000,
#endif
			session_log_notification      = 0x2000,
			torrent_log_notification      = 0x4000,
			peer_log_notification         = 0x8000,
			incoming_request_notification = 0x10000,
			dht_log_notification          = 0x20000,
			dht_operation_notification    = 0x40000,
			port_mapping_log_notification = 0x80000,
			picker_log_notification       = 0x100000,
			all_categories = 0x7fffffff
*/
}

NAN_MODULE_INIT(InitSocketTypes) {
/**
  enum class socket_type_t : std::uint8_t
	{
		tcp, tcp_ssl, udp, i2p, socks5, utp_ssl
	};
  */

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
