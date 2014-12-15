#ifndef TORRENT_PLUGIN_HPP
#define TORRENT_PLUGIN_HPP

#include "TorrentPluginConfiguration.hpp"

#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>

#include <boost/shared_ptr.hpp>

#include <QObject>

// Forward declaration
class Plugin;
class PeerPlugin;
class PeerPluginId;
class TorrentPluginStatus;

class TorrentPlugin : public QObject, public libtorrent::torrent_plugin {

    Q_OBJECT

public:

    // Constructor
    TorrentPlugin(Plugin * plugin, libtorrent::torrent * torrent, QLoggingCategory & category, const TorrentPluginConfiguration & torrentPluginConfiguration);

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

    // Removes peer plugin
    // 1) Remove plugin from peerPlugins_ map
    // 2) Deletes peer_plugin object
    // 3) Notifies controller
    //void removePeerPlugin(PeerPlugin * plugin);

    // Getters
    //libtorrent::torrent * getTorrent();
    const libtorrent::sha1_hash & getInfoHash() const;
    const TorrentPluginConfiguration & getTorrentPluginConfiguration() const;

signals:

    void torrentPluginStatusUpdated(const TorrentPluginStatus & status);

    /*
    void peerAdded(const PeerPluginId & peerPluginId);
    void peerRemoved(const PeerPluginId & peerPluginId);
    */

protected:

    // Parent plugin for BitSwapr: SHOULD THIS BE WEAK_PTR ?
    Plugin * _plugin;

    // Torrent for this torrent_plugin
    libtorrent::torrent * _torrent;

    // Map of peer plugin objects for each peer presently connected to this node through this torrent swarm
    std::map<libtorrent::tcp::endpoint, PeerPlugin *> _peerPlugins; // // Must be pointers, since TorrentPlugin::_category is reference, hence type is not copyable

    // Logging category
    QLoggingCategory & _category;

    // Configuration of torrent plugin
    TorrentPluginConfiguration _torrentPluginConfiguration;

    // Token accounting since session start
    int _tokensReceived, _tokensSent;

    // Set of all endpoints known to not have extension. Is populated by previous failed extended handshakes.
    std::set<libtorrent::tcp::endpoint> _peersWithoutExtension;

    // Set of endpoints banned for irregular conduct during extended protocol
    std::set<libtorrent::tcp::endpoint> _irregularPeer;

private:

    // Subroutines for the tick() method
    void sendTorrentPluginStatusSignal();


    // Checks that peer is not banned and that it is a bittorrent connection
    bool installPluginOnNewConnection(libtorrent::peer_connection * peerConnection);

};

#endif // TORRENT_PLUGIN_HPP
