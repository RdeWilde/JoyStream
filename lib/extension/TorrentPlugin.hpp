
#ifndef TORRENT_PLUGIN_HPP
#define TORRENT_PLUGIN_HPP

#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>
#include <libtorrent/socket.hpp> // tcp::endpoint

#include <boost/shared_ptr.hpp>

#include <QObject>

#include "TorrentPluginStatus.hpp"

// Forward declaration
class Plugin;
class PeerPlugin;

class TorrentPlugin : public QObject, public libtorrent::torrent_plugin {

    Q_OBJECT

public:

    // Constructor
    TorrentPlugin(Plugin * plugin, libtorrent::torrent * torrent, QLoggingCategory & category, bool pluginOn);

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
    virtual void on_add_peer(libtorrent::tcp::endpoint const & tcpEndPoint, int src, int flags);

    /**
      * Routines called by libtorrent network thread via tick() entry point on
      * peer plugins.
      */
    // Adds peer to respective set, and returns whether it was actually added or existed in the set from before.
    bool addToPeersWithoutExtensionSet(const libtorrent::tcp::endpoint & endPoint);
    bool addToIrregularPeersSet(const libtorrent::tcp::endpoint & endPoint);

    // Remove plugin from peerPlugins_ map
    void removePlugin(PeerPlugin * plugin);

    /**
     * Public routines used by non-libtorrent thread
     */

    // Returns plugin
    Plugin * getPlugin();

    // Returns torrent
    libtorrent::torrent * getTorrent();
    const libtorrent::sha1_hash & getInfoHash() const;

signals:

    void updateTorrentPluginStatus(TorrentPluginStatus status);

private:

    // Parent plugin for BitSwapr: SHOULD THIS BE WEAK_PTR ?
    Plugin * plugin_;

    // Torrent for this torrent_plugin
    libtorrent::torrent * torrent_;

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

    // Use the two sets below when accepting new peers in new_connect
    bool enableBanningSets;

    // Set of all endpoints known to not have extension. Is populated
    // by previous failed extended handshakes.
    std::set<libtorrent::tcp::endpoint> peersWithoutExtension_;

    // Set of endpoints banned for irregular conduct during extended protocol
    std::set<libtorrent::tcp::endpoint> irregularPeer_;

    // subroutines for the tick() method
    void sendTorrentPluginStatusSignal();
};

#endif
