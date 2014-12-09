#ifndef BUYER_TORRENT_PLUGIN_HPP
#define BUYER_TORRENT_PLUGIN_HPP

#include "TorrentPlugin.hpp"
#include "BuyerTorrentPluginParameters.hpp"

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
    BuyerTorrentPlugin(Plugin * plugin, libtorrent::torrent * torrent, QLoggingCategory & category, bool pluginOn, const TorrentPluginParameters & torrentPluginParameters, const BuyerTorrentPluginParameters & buyerTorrentPluginParameters);

    /**
     * All virtual functions below should ONLY be called by libtorrent network thread,
     * never by other threads, as this causes synchronization failures.
     */
    virtual boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection * peerConnection);
    virtual void tick();

private:

    // Parameters
    BuyerTorrentPluginParameters buyerTorrentPluginParameters_;
};

#endif // BUYER_TORRENT_PLUGIN_HPP
