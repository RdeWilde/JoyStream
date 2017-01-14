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

    struct dht_get_peers_reply_alert;
    struct listen_succeeded_alert;
    struct metadata_received_alert;
    struct metadata_failed_alert;
    struct add_torrent_alert;
    struct torrent_finished_alert;
    struct state_update_alert;
    struct torrent_removed_alert;
    struct torrent_resumed_alert;
    struct save_resume_data_alert;
    struct save_resume_data_failed_alert;
    struct torrent_paused_alert;
    struct torrent_checked_alert;

    struct peer_connect_alert;
    struct peer_disconnected_alert;
    struct read_piece_alert;
    struct piece_finished_alert;
}

namespace joystream {
namespace node_addon {

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

}
}
