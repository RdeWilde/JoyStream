#ifndef TORRENT_PLUGIN_HPP
#define TORRENT_PLUGIN_HPP

#include "BEPSupportStatus.hpp"

#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>

#include <boost/weak_ptr.hpp>

#include <QObject>

// Forward declaration
class Plugin;
class PeerPlugin;
//class PeerPluginId;
class TorrentPluginStatus;
class TorrentPluginRequest;
class TorrentPluginConfiguration;
//class PeerPluginConfiguration;
//class SetConfigurationTorrentPluginRequest;
//class SetPluginModeTorrentPluginRequest;
class TorrentPluginStatusAlert;
class TorrentPluginAlert;
enum class PluginMode;

/**
 * @brief Abstract type for all torrent plugin types (buyer, seller, observer).
 */
class TorrentPlugin : public QObject, public libtorrent::torrent_plugin {

    Q_OBJECT

public:

    // Constructor from member fields
    TorrentPlugin(Plugin * plugin, const boost::weak_ptr<libtorrent::torrent> & torrent, const TorrentPluginConfiguration & configuration, QLoggingCategory & category);

    /**
     * Virtual routines
     */

    // Destructor
    virtual ~TorrentPlugin();

    // Libtorrent callbacks
    virtual boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection * connection) = 0;
    virtual void on_piece_pass(int index) = 0;
    virtual void on_piece_failed(int index) = 0;
    virtual void tick() = 0;
    virtual bool on_resume() = 0;
    virtual bool on_pause() = 0;
    virtual void on_files_checked() = 0;
    virtual void on_state(int s) = 0;
    virtual void on_add_peer(const libtorrent::tcp::endpoint & endPoint, int src, int flags) = 0;

    // Removes peer plugin
    virtual void removePeerPlugin(PeerPlugin * plugin) = 0;
    virtual TorrentPluginConfiguration config() const = 0;

    /**
     * Routines called by libtorrent network thread from other plugin objects
     */

    // Get peer plugin, throws std::exception if there is no match
    boost::weak_ptr<libtorrent::peer_plugin> peerPlugin(const libtorrent::tcp::endpoint & endPoint);

    // Process torrent plugin requests
    // void processTorrentPluginRequest(const TorrentPluginRequest * request);

    /**
     * Getters and setters
     */

protected:

    // Parent plugin for BitSwapr: SHOULD THIS BE WEAK_PTR ?
    Plugin * _plugin;

    // Torrent for this torrent_plugin
    boost::weak_ptr<libtorrent::torrent> _torrent;

    // Maps endpoint to weak peer plugin pointer, is peer_plugin, since this is
    // the type of weak_ptr libtrrrent requires, hence might as well put it
    // in this type, rather than corresponding subclass of TorrentPlugin.
    QMap<libtorrent::tcp::endpoint, boost::weak_ptr<libtorrent::peer_plugin> > _peerPlugins;

    // Set of all endpoints known to not have extension. Is populated by previous failed extended handshakes.
    QSet<libtorrent::tcp::endpoint> _peersWithoutExtension;

    // Set of endpoints banned for irregular conduct during extended protocol
    QSet<libtorrent::tcp::endpoint> _irregularPeer;

    // Logging category
    QLoggingCategory & _category;

    /**
    // Plugin is active and therefore does tick() processing.
    // Is set by controller after file torrent metadata is acquired and/or
    // resume data has been validated.
    bool _pluginStarted;
    */

    // Tick processor
    void _tick();

    // Adds peer to respective set, and returns whether it was actually added or existed in the set from before.
    void addToPeersWithoutExtensionSet(const libtorrent::tcp::endpoint & endPoint);
    void addToIrregularPeersSet(const libtorrent::tcp::endpoint & endPoint);

    // Checks that peer is not banned and that it is a bittorrent connection
    bool isPeerWellBehaved(libtorrent::peer_connection * connection) const;

    // Send torrent plugin alert to libtorrent session
    void sendTorrentPluginAlert(const TorrentPluginAlert & alert);

    // Getters
    PluginMode pluginMode() const = 0;

private:

    // Creates torrent plugin status alert based on current state
    TorrentPluginStatusAlert createTorrentPluginStatusAlert();    
};

#endif // TORRENT_PLUGIN_HPP
