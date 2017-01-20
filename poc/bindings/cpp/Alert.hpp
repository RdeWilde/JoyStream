/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 12 2017
 */

#include <nan.h>

namespace libtorrent {
    class alert;
    struct torrent_alert;
    struct peer_alert;
    struct tracker_alert;

    // alert
    struct udp_error_alert; // todo
    struct external_ip_alert; // todo
    struct listen_failed_alert; // todo
    struct listen_succeeded_alert; // todo
    struct portmap_error_alert; // todo
    struct portmap_alert; // todo
    struct portmap_log_alert; // todo
    struct dht_announce_alert; // todo
    struct dht_get_peers_alert; // todo
    struct dht_bootstrap_alert; // todo
    struct session_stats_alert; // todo
    struct dht_error_alert; // todo
    struct dht_immutable_item_alert; // todo
    struct dht_mutable_item_alert; // todo
    struct dht_put_alert; // todo
    struct i2p_alert; // todo
    struct dht_outgoing_get_peers_alert; // todo
    struct log_alert; // todo
    struct lsd_error_alert; // todo
    struct dht_stats_alert; // todo
    struct dht_log_alert; // todo
    struct dht_pkt_alert; // todo
    struct dht_direct_response_alert; // todo
    struct session_error_alert; // todo

    // torrent_alert
    struct dht_get_peers_reply_alert;
    struct listen_succeeded_alert;
    struct metadata_received_alert;
    struct metadata_failed_alert;
    struct add_torrent_alert;
    struct torrent_finished_alert;
    struct state_update_alert;
    struct tracker_alert; // todo
    struct torrent_added_alert; // todo
    struct read_piece_alert;
    struct file_completed_alert; // todo
    struct file_renamed_alert; // todo
    struct file_rename_failed_alert; // todo
    struct performance_alert; // todo
    struct state_changed_alert; // todo
    struct torrent_removed_alert;
    struct torrent_resumed_alert;
    struct save_resume_data_alert;
    struct save_resume_data_failed_alert;
    struct torrent_paused_alert;
    struct torrent_checked_alert;
    struct storage_moved_alert; // todo
    struct storage_moved_failed_alert; // todo
    struct torrent_deleted_alert; // todo
    struct torrent_delete_failed_alert; // todo
    struct piece_finished_alert;
    struct hash_failed_alert; // todo
    struct url_seed_alert; // todo
    struct file_error_alert; // todo
    struct fastresume_rejected_alert; // todo
    struct stats_alert; // todo
    struct cache_flushed_alert; // todo
    struct anonymous_mode_alert; // todo
    struct torrent_need_cert_alert; // todo
    struct incoming_connection_alert; // todo
    struct torrent_log_alert; // todo

    // tracker_alert
    struct tracker_error_alert; // todo
    struct tracker_warning_alert; // todo
    struct scrape_reply_alert; // todo
    struct scrape_failed_alert; // todo
    struct tracker_reply_alert; // todo
    struct tracker_warning_alert; // todo
    struct dht_reply_alert; // todo
    struct tracker_announce_alert; // todo
    struct trackerid_alert; // todo

    // peer_alert
    struct peer_connect_alert;
    struct peer_disconnected_alert;
    struct peer_ban_alert; // todo
    struct peer_unsnubbed_alert; // todo
    struct peer_snubbed_alert; // todo
    struct peer_error_alert; // todo
    struct invalid_request_alert; // todo
    struct request_dropped_alert; // todo
    struct block_timeout_alert; // todo
    struct block_finished_alert; // todo
    struct unwanted_block_alert; // todo
    struct peer_blocked_alert; // todo
    struct lsd_peer_alert; // todo
    struct peer_log_alert; // todo
    struct incoming_request_alert; // todo
    struct picker_log_alert; // todo

namespace node {
namespace alert_types {

/**
 * Routines for converting libtorrent alerts to javascript
 * objects. No constructors are offered, since creating
 * these alerts is not possible when using libtorrent natively.
 * Moreover, javascript objects are not wrapping any c++ class,
 * we rather copy all member data into the javascript objects, since
 * only data members are present in alerts.
 */

// Abstract (base) alerts
void toObject(const libtorrent::alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::torrent_alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::peer_alert &, v8::Local<v8::Object> & o);

// Non-virtual alerts
void toObject(const libtorrent::dht_get_peers_reply_alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::listen_succeeded_alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::state_update_alert &, v8::Local<v8::Object> & o);

// Non-virtual (torrent) alerts
void toObject(const libtorrent::metadata_received_alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::metadata_failed_alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::add_torrent_alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::torrent_finished_alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::torrent_removed_alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::torrent_resumed_alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::save_resume_data_alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::save_resume_data_failed_alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::torrent_paused_alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::torrent_checked_alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::read_piece_alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::piece_finished_alert &, v8::Local<v8::Object> & o);

// Non-virtual (peer) alerts
void toObject(const libtorrent::peer_connect_alert &, v8::Local<v8::Object> & o);
void toObject(const libtorrent::peer_disconnected_alert &, v8::Local<v8::Object> & o);

}}}
