#ifndef ALERT_TYPES_HPP
#define ALERT_TYPES_HPP

#include <libtorrent/alert_types.hpp>

#define PLUGIN_STATUS_ALERT_ID                  (libtorrent::user_alert_id + 1)

// Defunct??
#define TORRENT_PLUGIN_STARTED_ALERT_ID         (libtorrent::user_alert_id + 2)

#define STARTED_SELLER_TORRENT_PLUGIN_ID        (libtorrent::user_alert_id + 3)
#define STARTED_BUYER_TORRENT_PLUGIN_ID         (libtorrent::user_alert_id + 4)

#define BUYER_TORRENT_PLUGIN_STATUS_ALERT_ID    (libtorrent::user_alert_id + 5)
#define SELLER_TORRENT_PLUGIN_STATUS_ALERT_ID   (libtorrent::user_alert_id + 6)

#define BUYER_PEER_PLUGIN_STARTED_ALERT_ID      (libtorrent::user_alert_id + 7)
#define SELLER_PEER_PLUGIN_STARTED_ALERT_ID     (libtorrent::user_alert_id + 8)

#define BUYER_PEER_PLUGIN_ADDED_ALERT_ID        (libtorrent::user_alert_id + 9)
#define SELLER_PEER_PLUGIN_ADDED_ALERT_ID       (libtorrent::user_alert_id + 10)

#define BUYER_PEER_PLUGIN_REMOVED_ALERT_ID      (libtorrent::user_alert_id + 11)
#define SELLER_PEER_PLUGIN_REMOVED_ALERT_ID     (libtorrent::user_alert_id + 12)

#endif // ALERT_TYPES_HPP
