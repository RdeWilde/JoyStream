#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>
#include <libtorrent/aux_/session_impl.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/policy.hpp>
#include <libtorrent/peer.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/lazy_entry.hpp>
#include <libtorrent/peer_id.hpp> // libtorrent::sha1_hash

#include <boost/shared_ptr.hpp>

#include <QObject>
#include <QMutex>

#include <queue>

// Forward declaration
class Controller;
class TorrentPlugin;
class PluginRequest;
class TorrentPluginRequest;
class PeerPluginRequest;

namespace libtorrent {
    class alert;
}

class Plugin : public QObject, public libtorrent::plugin {

    Q_OBJECT

private:

    // Controller
    Controller * _controller;

    // Libtorrent session. Is set by added() call, not constructor
    libtorrent::aux::session_impl * _session;

    // Maps info hash to pointer to corresponding torrent plugin
    std::map<libtorrent::sha1_hash, TorrentPlugin *> _torrentPlugins; // Must be pointers, since TorrentPlugin::_category is reference, hence type is not copyable

    // Logging category
    QLoggingCategory & _category;

    // Plugin Request
    std::queue<PluginRequest *> _pluginRequestQueue; // queue
    QMutex _pluginRequestQueueMutex; // mutex protecting queue

    // Torrent Plugin Request
    std::queue<TorrentPluginRequest *> _torrentPluginRequestQueue; // queue
    QMutex _torrentPluginRequestQueueMutex; // mutex protecting queue

    // Peer Plugin Request
    std::queue<PeerPluginRequest *> _peerPluginRequestQueue; // queue
    QMutex _peerPluginRequestQueueMutex; // mutex protecting queue

    /**
     * Subroutines for libtorrent thread.
     */

    void processesRequests();
    void processPluginRequest(const PluginRequest * pluginRequest);

public:

    // Constructor
    Plugin(Controller * controller, QLoggingCategory & category);

    // Destructor
    ~Plugin();

    // Returns controller
    //Controller * getController();

    /**
     * All virtual functions below should ONLY
     * be called by libtorrent network thread,
     * never by other threads, as this causes synchronization
     * failures.
     */
    virtual boost::shared_ptr<libtorrent::torrent_plugin> new_torrent(libtorrent::torrent * newTorrent, void * userData);
    virtual void added(libtorrent::aux::session_impl * session);
    virtual void on_alert(libtorrent::alert const * a);
    virtual void on_tick();
    virtual bool on_optimistic_unchoke(std::vector<libtorrent::policy::peer*> & peers);
    virtual void save_state(libtorrent::entry & stateEntry) const;
    virtual void load_state(libtorrent::lazy_entry const & stateEntry);

    /**
     * Routines called by libtorrent network thread via tick() entry point on
     * torrent plugins.
     *
     * THESE SHOULD NOT BE PUBLIC, RATHER MAKE THEM PRIVATE AND ADD FRIENDING WITH
     * TORRENT/PEER PLUGIN TYPES
     */

    // Removes torrent plugin
    // 1) Remove plugin from torrentPlugins_ map
    // 2) Deletes peer_plugin object
    // 3) Notifies controller
    void removeTorrentPlugin(const libtorrent::sha1_hash & info_hash);

    // Send alert to session object
    void sendAlertToSession(const libtorrent::alert & alert);

    /**
     * Synchronized routines called from controller by Qt thread.
     *
     * In all of these routines, plugin takes ownership of request object.
     */
    void submitPluginRequest(PluginRequest * pluginRequest);
    void submitTorrentPluginRequest(TorrentPluginRequest * torrentPluginRequest);
    void submitPeerPluginRequest(PeerPluginRequest * peerPluginRequest);

signals:

    // void pluginStatusUpdated(PluginStatus pluginStatus); // total account balance?
    //void torrentRemoved(libtorrent::sha1_hash info_hash);
};

#endif
