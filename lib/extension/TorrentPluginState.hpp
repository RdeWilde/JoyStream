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
    //dormant, // Before plugin has been started after torrent ready alert

    /**
     * Buy
     */

    // Sending out join_contract messages while trying to get enough
    // peers to sign contract refunds to start the channel
    populating_payment_channel,

    // Requesting, validating, downlading, saving and paying for pieces
    downloading_pieces,

    // While contract outputs have not all been spent, we still wait to use refunds
    waiting_to_spend_refunds,

    // Nothing more to do
    finished
};

#endif // TORRENT_PLUGIN_STATE_HPP
