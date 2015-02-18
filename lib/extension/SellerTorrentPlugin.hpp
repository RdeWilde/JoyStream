#ifndef SELLER_TORRENT_PLUGIN_HPP
#define SELLER_TORRENT_PLUGIN_HPP

#include "TorrentPlugin.hpp"

#include <QMap>

class SellerPeerPlugin;

class SellerTorrentPlugin : public TorrentPlugin
{
public:

    /**
     * Mutually exclusive set of states for seller torrent plugin,
     * in terms of cause of tick processing not advancing.
     */
    enum class State {

        //..

    };

    /**
     * @brief Configuration of seller torrent plugin.
     */
    class Configuration : public TorrentPlugin::Configuration {

    public:

    private:

        // Maximum price accepted (satoshies)
        quint64 _minPrice;

        // Minimum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
        quint32 _minLock;

        // Minimum fee per byte in contract transaction (satoshies)
        quint64 _minFeePerByte;

        // Maximum time (s) for which seller is willing to seed without contract getting at least one confirmation
        quint32 _maxContractConfirmationDelay;
    };

    // Constructor
    SellerTorrentPlugin(Plugin * plugin, libtorrent::torrent * torrent, QLoggingCategory & category, const Configuration & configuration);

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
    Configuration _sellerTorrentPluginConfiguration;
};

#endif // SELLER_TORRENT_PLUGIN_HPP
