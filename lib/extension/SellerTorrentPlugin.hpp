#ifndef SELLER_TORRENT_PLUGIN_HPP
#define SELLER_TORRENT_PLUGIN_HPP

#include "TorrentPlugin.hpp"
#include "SellerTorrentPluginConfiguration.hpp"

#include <QMap>

class SellerPeerPlugin;

class SellerTorrentPlugin : public TorrentPlugin
{
public:

    // Constructor
    SellerTorrentPlugin(Plugin * plugin, libtorrent::torrent * torrent, QLoggingCategory & category, const SellerTorrentPluginConfiguration & configuration);

    // Removes peer plugin by
    void removePeerPlugin(const libtorrent::tcp::endpoint & endPoint);

    // Parameters for running plugin
    virtual const TorrentPluginConfiguration & getTorrentPluginConfiguration();

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

private:

    // Parameters for
    SellerTorrentPluginConfiguration _configuration;

    /**
    // Maps endpoint to corresponding peer for all peers being sold to: IS THIS REDUNDANT?
    QMap<libtorrent::tcp::endpoint, SellerPeerPlugin *> _peers;
    */
};

#endif // SELLER_TORRENT_PLUGIN_HPP
