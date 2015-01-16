#ifndef TORRENT_PLUGIN_STATE_HPP
#define TORRENT_PLUGIN_STATE_HPP

/*
 * Mutually exclusive set of states for torrent plugin,
 * in terms of cause of tick processing not advancing.
 *
 * Sharded!
 */
enum class TorrentPluginState {

    // Do we even need dormant, how does it related to active?
    dormant, // Before plugin has been started after torrent ready alert

    /**
     * Buy
     */

    waiting_for_enough_contract_participants,

    waiting_for_refund_signatures,
    downloading_pieces,
    waiting_for_all_refunds_to_be_spent,
    done

    // Seller has not torrent level state
};

#endif // TORRENTPLUGINSTATE_HPP
