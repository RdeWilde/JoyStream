#ifndef TORRENT_PLUGIN_HPP
#define TORRENT_PLUGIN_HPP

#include "TorrentPluginParameters.hpp"

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

    // Destructor
    ~TorrentPlugin();

    /**
     * All virtual functions below should ONLY
     * be called by libtorrent network thread,
     * never by other threads, as this causes synchronization
     * failures.
     */
    virtual boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection * peerConnection) = 0;
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

    // Checks that peer is not banned and that it is a bittorrent connection
    bool installPluginOnNewConnection(libtorrent::peer_connection * peerConnection);

    // Adds peer to respective set, and returns whether it was actually added or existed in the set from before.
    bool addToPeersWithoutExtensionSet(const libtorrent::tcp::endpoint & endPoint);
    bool addToIrregularPeersSet(const libtorrent::tcp::endpoint & endPoint);

    // Removes peer plugin
    // 1) Remove plugin from peerPlugins_ map
    // 2) Deletes peer_plugin object
    // 3) Notifies controller
    void removePeerPlugin(PeerPlugin * plugin);

    // Returns plugin
    //Plugin * getPlugin();

    // Returns torrent
    //libtorrent::torrent * getTorrent();
    const libtorrent::sha1_hash & getInfoHash() const;

signals:

    void torrentPluginStatusUpdated(const TorrentPluginStatus & status);

    /*
    void peerAdded(const PeerPluginId & peerPluginId);
    void peerRemoved(const PeerPluginId & peerPluginId);
    */

protected:

    // Constructor
    TorrentPlugin(Plugin * plugin, libtorrent::torrent * torrent, QLoggingCategory & category, bool pluginOn, const TorrentPluginParameters & torrentPluginParameters);

    // Parent plugin for BitSwapr: SHOULD THIS BE WEAK_PTR ?
    Plugin * plugin_;

    // Torrent for this torrent_plugin
    libtorrent::torrent * torrent_;

    // Parameters for running plugin
    TorrentPluginParameters torrentPluginParameters_;

    // Map of peer plugin objects for each peer presently connected to this node through this torrent swarm
    std::map<libtorrent::tcp::endpoint, PeerPlugin *> peerPlugins_;

    // Logging category
    QLoggingCategory & category_;

    // State of this torrent plugin
    // Determines whether libtorrent automanagement is active for this torrent
    // When not (on), the peer plugins behave as buyers or sellers depending
    // on whether full file has been aquired.
    bool pluginOn_;

    // Token accounting since session start
    int tokensReceived_, tokensSent_;

    // Set of all endpoints known to not have extension. Is populated
    // by previous failed extended handshakes.
    std::set<libtorrent::tcp::endpoint> peersWithoutExtension_;

    // Set of endpoints banned for irregular conduct during extended protocol
    std::set<libtorrent::tcp::endpoint> irregularPeer_;

    /**
      * Utility routines, not part of entity representation.
      */

    // subroutines for the tick() method
    void sendTorrentPluginStatusSignal();
};

#endif
