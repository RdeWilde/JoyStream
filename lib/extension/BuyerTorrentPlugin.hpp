#ifndef BUYER_TORRENT_PLUGIN_HPP
#define BUYER_TORRENT_PLUGIN_HPP

#include "TorrentPlugin.hpp"
#include "BuyerTorrentPluginConfiguration.hpp"
#include "PaymentChannel/PayorPaymentChannel.hpp"
//#include "BitCoin/PublicKey.hpp"

class BuyerPeerPlugin;

/**
 * @brief Torrent plugin for buyer mode.
 */
class BuyerTorrentPlugin : public TorrentPlugin
{
public:

    // Constructor
    BuyerTorrentPlugin(Plugin * plugin, const boost::weak_ptr<libtorrent::torrent> & torrent, const BuyerTorrentPluginConfiguration & configuration, QLoggingCategory & category);

    /**
     * All virtual functions below should ONLY be called by libtorrent network thread,
     * never by other threads, as this causes synchronization failures.
     */
    virtual boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection * connection);
    virtual void on_piece_pass(int index);
    virtual void on_piece_failed(int index);
    virtual void tick();
    virtual bool on_resume();
    virtual bool on_pause();
    virtual void on_files_checked();
    virtual void on_state(int s);
    virtual void on_add_peer(const libtorrent::tcp::endpoint & endPoint, int src, int flags);

    // Removes peer plugin by
    void removePeerPlugin(const libtorrent::tcp::endpoint & endPoint);

    /**
     * Mutually exclusive set of states for torrent plugin,
     * in terms of cause of tick processing not advancing.
     */
    enum class State {

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

    // Getters and setters
    State state() const;

private:

    // Buyer plugin state
    State _state;

    // Payment channel
    PayorPaymentChannel _channel;

    // Time since plugin was created, is used to keep track of when to start picking sellers.
    QTime _timeSincePluginStarted;

    // what requests have been sent out for pieces we still
    // dont have, and how long have we been waiting (so that we can discard slow bastards).

    // What refunds have been spent,and what have not.
    // Use timer to keep checking back?

    // Parameters
    BuyerTorrentPluginConfiguration _configuration;
};

#endif // BUYER_TORRENT_PLUGIN_HPP
