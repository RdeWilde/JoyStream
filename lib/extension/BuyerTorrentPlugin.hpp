#ifndef BUYER_TORRENT_PLUGIN_HPP
#define BUYER_TORRENT_PLUGIN_HPP

#include "TorrentPlugin.hpp"
#include "BuyerTorrentPluginConfiguration.hpp"

#include "BuyerTorrentPluginState.hpp"
#include "PaymentChannel/PayorPaymentChannel.hpp"
#include "BitCoin/PublicKey.hpp"

class BuyerPeerPlugin;

/**
 * This is a very simple buyer plugin, it has a price
 * discovery phase where it just connects to peers and gathers
 * prices from sellers, after which it decides to pick some
 * subset from which to buy from.
 */
class BuyerTorrentPlugin : public TorrentPlugin
{
public:

    // Constructor
    BuyerTorrentPlugin(Plugin * plugin, libtorrent::torrent * torrent, QLoggingCategory & category, bool pluginOn, const BuyerTorrentPluginConfiguration * buyerTorrentPluginConfiguration);

    // Removes peer plugin by
    void removePeerPlugin(const libtorrent::tcp::endpoint & endPoint);

    /**
     * All virtual functions below should ONLY be called by libtorrent network thread,
     * never by other threads, as this causes synchronization failures.
     */
    virtual boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection * peerConnection);
    virtual void tick();

    // Getters
    BuyerTorrentPluginState buyerTorrentPluginState() const;

    // Buyer plugin state
    BuyerTorrentPluginState _state;

    // Payment channel
    PayorPaymentChannel _channel;

    // Counts from buyer plugin was started,
    // is used to keep track of when to start picking sellers.
    QTime _timeSincePluginStarted;

    // what requests have been sent out for pieces we still
    // dont have, and how long have we been waiting (so that we can discard slow bastards).

    // What refunds have been spent,and what have not.
    // Use timer to keep checking back?

private:

    // Parameters
    BuyerTorrentPluginConfiguration _buyerTorrentPluginConfiguration;
};

#endif // BUYER_TORRENT_PLUGIN_HPP
