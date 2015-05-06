#ifndef ALERT_TYPES_HPP
#define ALERT_TYPES_HPP

#include <libtorrent/alert_types.hpp>

#define PLUGIN_STATUS_ALERT_ID                  (libtorrent::user_alert_id + 1)

#define TORRENT_PLUGIN_STARTED_ALERT_ID         (libtorrent::user_alert_id + 2)

#define BUYER_TORRENT_PLUGIN_STATUS_ALERT_ID    (libtorrent::user_alert_id + 3)
#define SELLER_TORRENT_PLUGIN_STATUS_ALERT_ID   (libtorrent::user_alert_id + 4)

#define BUYER_PEER_PLUGIN_STARTED_ALERT_ID      (libtorrent::user_alert_id + 5)
#define SELLER_PEER_PLUGIN_STARTED_ALERT_ID     (libtorrent::user_alert_id + 6)

#endif // ALERT_TYPES_HPP
