/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 12 2017
 */

#include "Alert.hpp"
#include "torrent_handle.h"
#include "add_torrent_params.hpp"
#include "endpoint.hpp"
#include "info_hash.hpp"
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
#define SIZE_KEY "size"
#define NEW_NAME_KEY "newName"
#define STATE_KEY "state"
#define PREV_STATE_KEY "prevState"
#define TIMES_IN_ROW_KEY "timesInRow"
#define STATUS_CODE_KEY "statusCode"
#define ERROR_MESSAGE_KEY "errorMessage"
#define INCOMPLETE_KEY "incomplete"
#define COMPLETE_KEY "complete"
#define NUM_PEERS_KEY "numPeers"
#define EVENT_KEY "event"
#define OPERATION_KEY "operation"
#define SOCKET_TYPE_KEY "socketType"
#define WE_HAVE_KEY "weHave"
#define PEER_INTERESTED_KEY "peerInterested"
#define WITHHELD_KEY "withheld"
#define BLOCK_INDEX_KEY "blockIndex"
#define STORAGE_PATH_KEY "storagePath"
#define FILE_PATH_KEY "filePath"
#define SERVER_URL_KEY "serverURL"
#define FILENAME_KEY "filename"
#define PORT_KEY "port"
#define MAPPING_KEY "mapping"
#define MAP_TYPE_KEY "mapType"
#define EXTERNAL_PORT_KEY "externalPort"
#define PROTOCOL_KEY "protocol"
#define LOG_MESSAGE_KEY "logMessage"
#define REASON_KEY "reason"
#define INTERVAL_KEY "interval"
#define KIND_KEY "kind"
#define STR_KEY "str"
#define TRACKER_ID_KEY "trackerID"
#define ENDPOINT_KEY "endpoint"
#define ADD_TORRENT_PARAMS_KEY "addTorrentParams"

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

 v8::Local<v8::Object> toObject(const libtorrent::torrent_added_alert & a) {
   v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

   return o;
 }

 v8::Local<v8::Object> toObject(const libtorrent::torrent_removed_alert & a) {
   v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

   SET_VAL(o, INFO_HASH_KEY, info_hash::toObject(a.info_hash))

   return o;
 }

 v8::Local<v8::Object> toObject(const libtorrent::read_piece_alert & a) {
   v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

   // error_code const error;
   // boost::shared_array<char> const buffer;
   // piece_index_t const piece;
   SET_INT32(o, SIZE_KEY, a.size());

   return o;
 }

 v8::Local<v8::Object> toObject(const libtorrent::file_completed_alert & a) {
   v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

   //	file_index_t const index;

   return o;
 }

v8::Local<v8::Object> toObject(const libtorrent::file_renamed_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  SET_CONST_CHAR(o, NEW_NAME_KEY, a.new_name());

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::file_rename_failed_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  // file_index_t const index;
  // error_code const error;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::performance_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

	// performance_warning_t const warning_code;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::state_changed_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  SET_VAL(o, STATE_KEY, state_t::toObject(a.state));
  SET_VAL(o, PREV_STATE_KEY, state_t::toObject(a.prev_state));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::tracker_error_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::tracker_alert &>(a));

  SET_INT32(o, TIMES_IN_ROW_KEY, a.times_in_row)
  SET_INT32(o, STATUS_CODE_KEY, a.status_code)
  //error_code const error;
  SET_CONST_CHAR(o, ERROR_MESSAGE_KEY, a.error_message());

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::tracker_warning_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::tracker_alert &>(a));

  SET_INT32(o, TIMES_IN_ROW_KEY, a.times_in_row)
  SET_INT32(o, STATUS_CODE_KEY, a.status_code)
  //error_code const error;
  SET_CONST_CHAR(o, ERROR_MESSAGE_KEY, a.error_message());

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::scrape_reply_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::tracker_alert &>(a));

  SET_INT32(o, INCOMPLETE_KEY, a.incomplete)
  SET_INT32(o, COMPLETE_KEY, a.complete)

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::scrape_failed_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::tracker_alert &>(a));

  // error_code const error;
  SET_CONST_CHAR(o, ERROR_MESSAGE_KEY, a.error_message());

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::tracker_reply_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::tracker_alert &>(a));

  SET_INT32(o, NUM_PEERS_KEY, a.num_peers);

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::peer_connect_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::peer_alert &>(a));

  SET_INT32(o, SOCKET_TYPE_KEY, a.socket_type);

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::peer_disconnected_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::peer_alert &>(a));

  // the kind of socket this peer was connected over
  SET_INT32(o, SOCKET_TYPE_KEY, a.socket_type);

  // the operation or level where the error occurred. Specified as an
  // value from the operation_t enum. Defined in operations.hpp.
  //operation_t const operation;

  // tells you what error caused peer to disconnect.
  // error_code const error;

  // the reason the peer disconnected (if specified)
  // close_reason_t const reason;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::invalid_request_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::peer_alert &>(a));

  // the request we received from the peer
  // peer_request const request;

  // true if we have this piece
  SET_BOOL(o, WE_HAVE_KEY, a.we_have);

  // true if the peer indicated that it was interested to download before
  // sending the request
  SET_BOOL(o, PEER_INTERESTED_KEY, a.peer_interested);

  // if this is true, the peer is not allowed to download this piece because
  // of superseeding rules.
  SET_BOOL(o, WITHHELD_KEY, a.withheld);

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::torrent_finished_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::piece_finished_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  // the index of the piece that finished
  // piece_index_t const piece_index;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::request_dropped_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::peer_alert &>(a));

  SET_INT32(o, BLOCK_INDEX_KEY, a.block_index);
  // piece_index_t const piece_index;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::block_finished_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::peer_alert &>(a));

  SET_INT32(o, BLOCK_INDEX_KEY, a.block_index);
  // piece_index_t const piece_index;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::block_timeout_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::peer_alert &>(a));

  SET_INT32(o, BLOCK_INDEX_KEY, a.block_index);
  // piece_index_t const piece_index;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::block_downloading_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::peer_alert &>(a));

  SET_INT32(o, BLOCK_INDEX_KEY, a.block_index);
  // piece_index_t const piece_index;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::unwanted_block_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::peer_alert &>(a));

  SET_INT32(o, BLOCK_INDEX_KEY, a.block_index);
  // piece_index_t const piece_index;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::storage_moved_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  SET_CONST_CHAR(o, STORAGE_PATH_KEY, a.storage_path());

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::storage_moved_failed_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  // 	error_code const error;
  SET_CONST_CHAR(o, FILE_PATH_KEY, a.file_path());
  SET_INT32(o, OPERATION_KEY, a.operation);


  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::torrent_deleted_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  SET_VAL(o, INFO_HASH_KEY, info_hash::toObject(a.info_hash));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::torrent_delete_failed_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  // tells you why it failed.
  // error_code const error;
  SET_VAL(o, INFO_HASH_KEY, info_hash::toObject(a.info_hash));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::save_resume_data_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  // points to the resume data.
	//std::shared_ptr<entry> const resume_data;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::save_resume_data_failed_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  // the error code from the resume_data failure
	// error_code const error;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::torrent_paused_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::torrent_resumed_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::torrent_checked_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::url_seed_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  // the error the web seed encountered. If this is not set, the server
  // sent an error message, call ``error_message()``.
  // error_code const error;

  SET_CONST_CHAR(o, SERVER_URL_KEY, a.server_url());
  SET_CONST_CHAR(o, ERROR_MESSAGE_KEY, a.error_message());

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::file_error_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  // the error code describing the error.
  //error_code const error;
  SET_CONST_CHAR(o, OPERATION_KEY, a.operation);
  SET_CONST_CHAR(o, FILENAME_KEY, a.filename());

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::metadata_failed_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  // indicates what failed when parsing the metadata. This error is
  // what's returned from lazy_bdecode().
  // error_code const error;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::metadata_received_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::udp_error_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::alert &>(a));

  // the source address associated with the error (if any)
  // udp::endpoint const endpoint;

  // the error code describing the error
  //error_code const error;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::external_ip_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::alert &>(a));

  // address const external_address;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::listen_failed_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::alert &>(a));

  SET_CONST_CHAR(o, LISTEN_INTERFACE_KEY, a.listen_interface());
  // error_code const error;
  SET_INT32(o, OPERATION_KEY, a.operation)
  //libtorrent::socket_type_t const socket_type;
  //libtorrent::address const address;
  SET_INT32(o, PORT_KEY, a.port)

  // NOTE: PORT_KEY AND ADDRESS_KEY are defined in endpoint.cpp

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::listen_succeeded_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::alert &>(a));

  //alert.address;
  SET_INT32(o, PORT_KEY, a.port);
  //alert.socket_type; // libtorrent::socket_type_t

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::portmap_error_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::alert &>(a));

  SET_INT32(o, MAPPING_KEY, a.mapping);
  SET_INT32(o, MAP_TYPE_KEY, a.map_type);
  // error_code const error;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::portmap_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::alert &>(a));

  SET_INT32(o, MAPPING_KEY, a.mapping);
  SET_INT32(o, EXTERNAL_PORT_KEY, a.external_port);
  SET_INT32(o, MAP_TYPE_KEY, a.map_type);
  SET_INT32(o, PROTOCOL_KEY, a.protocol);

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::portmap_log_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::alert &>(a));

  SET_INT32(o, MAP_TYPE_KEY, a.map_type);
  SET_CONST_CHAR(o, LOG_MESSAGE_KEY, a.log_message());

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::fastresume_rejected_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  // 	error_code const error;
  SET_CONST_CHAR(o, FILE_PATH_KEY, a.file_path());
  SET_INT32(o, OPERATION_KEY, a.operation);

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::peer_blocked_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::peer_alert &>(a));

  SET_INT32(o, REASON_KEY, a.reason);

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::dht_announce_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::alert &>(a));

  // address const ip;
  SET_INT32(o, PORT_KEY, a.port);
  SET_VAL(o, INFO_HASH_KEY, info_hash::toObject(a.info_hash));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::dht_get_peers_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::alert &>(a));

  SET_VAL(o, INFO_HASH_KEY, info_hash::toObject(a.info_hash));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::stats_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  // an array of samples. The enum describes what each sample is a
  // measurement of. All of these are raw, and not smoothing is performed.
  // std::array<int, num_channels> const transferred;

  SET_INT32(o, INTERVAL_KEY, a.interval);

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::cache_flushed_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::anonymous_mode_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  // specifies what error this is, see kind_t.
  SET_INT32(o, KIND_KEY, a.kind);
  SET_STD_STRING(o, STR_KEY, a.str);

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::lsd_peer_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::peer_alert &>(a));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::trackerid_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::tracker_alert &>(a));

  SET_CONST_CHAR(o, TRACKER_ID_KEY, a.tracker_id());

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::dht_bootstrap_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::alert &>(a));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::torrent_error_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

	// error_code const error;
  SET_CONST_CHAR(o, FILENAME_KEY, a.filename());

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::torrent_need_cert_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

	// error_code const error;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::incoming_connection_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::alert &>(a));

  //int const socket_type;
  SET_VAL(o, ENDPOINT_KEY, EndpointWrap::New(a.endpoint))

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::add_torrent_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  SET_VAL(o, ADD_TORRENT_PARAMS_KEY, libtorrent::node::add_torrent_params::toObject(a.params))
  //error_code const error;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::state_update_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::alert &>(a));

  // std::vector<torrent_status> const status;

  return o;
}


////////////////////////////////////////////////////////////////////////////////////////////////

v8::Local<v8::Object> toObject(const libtorrent::dht_reply_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::tracker_alert &>(a));

  SET_INT32(o, NUM_PEERS_KEY, a.num_peers)

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::tracker_announce_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::tracker_alert &>(a));

  SET_INT32(o, EVENT_KEY, a.event)

  return o;
}

v8::Local<v8::Object> toObject(consv8::Local<v8::Object> toObject(const libtorrent::torrent_paused_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);
}t libtorrent::hash_failed_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::torrent_alert &>(a));

  //piece_index_t const piece_index;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::peer_ban_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::peer_alert &>(a));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::peer_unsnubbed_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::peer_alert &>(a));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::peer_snubbed_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::peer_alert &>(a));

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::peer_error_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::peer_alert &>(a));

  SET_INT32(o, OPERATION_KEY, a.operation)
  // error_code const error;

  return o;
}

v8::Local<v8::Object> toObject(const libtorrent::dht_get_peers_reply_alert & a) {
  v8::Local<v8::Object> o = toObject(static_cast<const libtorrent::alert &>(a));

  //SET_STR(o, "info_hash", a.info_hash.)
  SET_INT(o, "num_peers", a.num_peers())
  // std::vector<tcp::endpoint> peers() const

  return o;
}

// Non-virtual (torrent) alerts


v8::Local<v8::Object> toObject(const libtorrent::torrent_removed_alert & a, v8::Local<v8::Object> & o) {
  toObject(static_cast<const libtorrent::torrent_alert &>(a), o);

  // is required, since handle has expired most likeyl
  //sha1_hash const info_hash;
}
}}}
