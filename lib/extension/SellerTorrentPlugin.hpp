#ifndef SELLER_TORRENT_PLUGIN_HPP
#define SELLER_TORRENT_PLUGIN_HPP

#include "TorrentPlugin.hpp"
#include "SellerTorrentPluginConfiguration.hpp"

class SellerPeerPlugin;

/**
 * This is the simplest possible seller plugin, it sells to all peers
 * above a given set price by user, hence it has no price discovery
 * phase.
 */
class SellerTorrentPlugin : public TorrentPlugin
{
public:

    // Constructor
    SellerTorrentPlugin(Plugin * plugin, libtorrent::torrent * torrent, QLoggingCategory & category, bool pluginOn, const TorrentPluginConfiguration & torrentPluginParameters, const SellerTorrentPluginConfiguration & sellerTorrentPluginParameters);

    // Parameters for running plugin
    virtual const TorrentPluginConfiguration & getTorrentPluginConfiguration();

    /**
     * All virtual functions below should ONLY be called by libtorrent network thread,
     * never by other threads, as this causes synchronization failures.
     */
    virtual boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection * peerConnection);
    virtual void tick();

private:

    // Parameters for
    SellerTorrentPluginConfiguration _sellerTorrentPluginParameters;

    // Maps endpoint to corresponding peer for all peers being sold to: IS THIS REDUNDANT?
    std::map<libtorrent::tcp::endpoint, SellerPeerPlugin *> _presentlySellingTo;
};

#endif // SELLER_TORRENT_PLUGIN_HPP
