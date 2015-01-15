#ifndef TORRENT_PLUGIN_STATE_HPP
#define TORRENT_PLUGIN_STATE_HPP

/*
 * Mutually exclusive set of states for torrent plugin.
 *
 * Sharded!
 */
enum class TorrentPluginState {
    dormant, // Before plugin has been started after torrent ready alert

    // Buy
    finding_sellers,
    waiting_for_refund_signatures,
    downloading_pieces,
    waiting_for_all_refunds_to_be_spent,
    done

    // Seller has not torrent level state
};

#endif // TORRENTPLUGINSTATE_HPP
