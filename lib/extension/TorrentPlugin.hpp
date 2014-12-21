#ifndef TORRENT_PLUGIN_HPP
#define TORRENT_PLUGIN_HPP

#include "StartedPluginMode.hpp"

#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>

#include <boost/shared_ptr.hpp>

#include <QObject>

// Forward declaration
class Plugin;
class PeerPlugin;
class PeerPluginId;
class TorrentPluginStatus;
class TorrentPluginRequest;
class TorrentPluginConfiguration;
class PeerPluginConfiguration;
class SetConfigurationTorrentPluginRequest;
class SetPluginModeTorrentPluginRequest;

class TorrentPlugin : public QObject, public libtorrent::torrent_plugin {

    Q_OBJECT

public:

    // Default constructor
    TorrentPlugin();

    // Constructor from member fields
    TorrentPlugin(Plugin * plugin, libtorrent::torrent * torrent, QLoggingCategory & category, TorrentPluginConfiguration * torrentPluginConfiguration);

    // Destructor
    ~TorrentPlugin();

    /**
     * All virtual functions below should ONLY
     * be called by libtorrent network thread,
     * never by other threads, as this causes synchronization
     * failures.
     */
    virtual boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection * peerConnection);
    virtual void on_piece_pass(int index);
    virtual void on_piece_failed(int index);
    virtual void tick();
    virtual bool on_resume();
    virtual bool on_pause();
    virtual void on_files_checked();
    virtual void on_state(int s);
    virtual void on_add_peer(const libtorrent::tcp::endpoint & endPoint, int src, int flags);

    /**
     * Routines called by libtorrent network thread from other plugin objects
     */

    // Adds peer to respective set, and returns whether it was actually added or existed in the set from before.
    bool addToPeersWithoutExtensionSet(const libtorrent::tcp::endpoint & endPoint);
    bool addToIrregularPeersSet(const libtorrent::tcp::endpoint & endPoint);

    // Get peer plugin
    PeerPlugin * getPeerPlugin(const libtorrent::tcp::endpoint & endPoint);

    // Removes peer plugin
    // 1) Remove plugin from peerPlugins_ map
    // 2) Deletes peer_plugin object
    // 3) Notifies controller
    void removePeerPlugin(PeerPlugin * plugin);

    // Process torrent plugin requests
    void processTorrentPluginRequest(const TorrentPluginRequest * torrentPluginRequest);
    void processSetConfigurationTorrentPluginRequest(const SetConfigurationTorrentPluginRequest * setConfigurationTorrentPluginRequest);
    //void processSetPluginModeTorrentPluginRequest(const SetPluginModeTorrentPluginRequest * setPluginModeTorrentPluginRequest);


    // Getters
    //libtorrent::torrent * getTorrent();
    //const libtorrent::sha1_hash & getInfoHash() const;
    //const TorrentPluginConfiguration & getTorrentPluginConfiguration() const;

signals:

    /*
    void torrentPluginStatusUpdated(const TorrentPluginStatus & status);
    void peerAdded(const PeerPluginId & peerPluginId);
    void peerRemoved(const PeerPluginId & peerPluginId);
    */

protected:

    // Parent plugin for BitSwapr: SHOULD THIS BE WEAK_PTR ?
    Plugin * _plugin;

    // Torrent for this torrent_plugin
    libtorrent::torrent * _torrent;

    // Map of peer plugin objects for each peer presently connected to this node through this torrent swarm
    std::map<libtorrent::tcp::endpoint, PeerPlugin *> _peerPlugins;

    // Logging category
    QLoggingCategory & _category;

    // Set of all endpoints known to not have extension. Is populated by previous failed extended handshakes.
    std::set<libtorrent::tcp::endpoint> _peersWithoutExtension;

    // Set of endpoints banned for irregular conduct during extended protocol
    std::set<libtorrent::tcp::endpoint> _irregularPeer;

    // Indicates if torrent has been properly checked and
    // if value in _torrentPluginConfiguration is reliable
    // Perhaps a better way of representing state can be found in the future
    // I hate relying on the null pointer below
    bool _pluginStarted;

    // Configuration: only relevant when (_pluginStarted == true)
    // NULL means we dont buy or sell
    // NON-NULL means we are buyer or seller
    TorrentPluginConfiguration * _torrentPluginConfiguration;

private:

    // Subroutines for the tick() method
    void sendTorrentPluginStatusSignal();

    // Checks that peer is not banned and that it is a bittorrent connection
    bool installPluginOnNewConnection(libtorrent::peer_connection * peerConnection) const;

    /**
    // Creates configurations for new peer plugins
    PeerPluginConfiguration * createPeerPluginConfiguration(const libtorrent::tcp::endpoint & endPoint) const;
    */
};

#endif // TORRENT_PLUGIN_HPP
