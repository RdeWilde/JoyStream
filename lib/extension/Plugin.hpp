#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include "BitCoin/Client.hpp"
#include "SellerTorrentPlugin.hpp"
#include "BuyerTorrentPlugin.hpp"

#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/policy.hpp>
#include <libtorrent/peer.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/lazy_entry.hpp>
#include <libtorrent/peer_id.hpp> // libtorrent::sha1_hash
#include <libtorrent/aux_/session_impl.hpp>

#include <boost/weak_ptr.hpp>

#include <QObject>
#include <QMutex>
#include <QQueue>

// Forward declaration
class Controller;
class TorrentPlugin;
class SellerTorrentPlugin;
class BuyerTorrentPlugin;
class PluginRequest;
class TorrentPluginRequest;
class PeerPluginRequest;
class TorrentPluginConfiguration;
//class BuyerTorrentPlugin::Configuration;
//class SellerTorrentPlugin::Configuration;
class QNetworkReply;
class Wallet;

namespace libtorrent {
    class alert;
    class session_impl;
}

namespace boost {
    template<class T> class shared_ptr;
}

class Plugin : public QObject, public libtorrent::plugin {

    Q_OBJECT

public:

    // Constructor
    Plugin(Controller * controller, Wallet * wallet, QNetworkAccessManager & manager, QString bitcoindAccount, QLoggingCategory & category);

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
    //virtual boost::shared_ptr<libtorrent::torrent_plugin> new_torrent(libtorrent::torrent * torrent, void * userData);
    virtual void added(boost::weak_ptr<libtorrent::aux::session_impl> session);
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


    /**
     * Synchronized routines called from controller by Qt thread.
     *
     * In all of these routines, plugin takes ownership of request object.
     */
    void submitPluginRequest(PluginRequest * pluginRequest);
    void submitTorrentPluginRequest(TorrentPluginRequest * torrentPluginRequest);
    void submitPeerPluginRequest(PeerPluginRequest * peerPluginRequest);

private:

    // Controller
    Controller * _controller;

    // Wallet
    Wallet * _wallet;

    // Libtorrent session. Is set by added() call, not constructor
    boost::weak_ptr<libtorrent::aux::session_impl> _session;

    // Maps info hash to pointer to torrent plugin,
    // has to be TorrentPlugin weak pointer, since this is
    // what libtorrent requires
    QMap<libtorrent::sha1_hash, boost::weak_ptr<libtorrent::torrent_plugin> > _plugins;

    // BitCoind wrapper
    BitCoindRPC::Client _btcClient;

    // Logging category
    QLoggingCategory & _category;

    // Has this plugin been added to session.
    // Do not use the _session pointer before this.
    bool _addedToSession;

    /**
     * Request processing
     */

    // Plugin Request
    QQueue<PluginRequest *> _pluginRequestQueue;
    QMutex _pluginRequestQueueMutex; // mutex protecting queue

    // Torrent Plugin Request
    QQueue<TorrentPluginRequest *> _torrentPluginRequestQueue; // queue
    QMutex _torrentPluginRequestQueueMutex; // mutex protecting queue

    // Peer Plugin Request
    QQueue<PeerPluginRequest *> _peerPluginRequestQueue; // queue
    QMutex _peerPluginRequestQueueMutex; // mutex protecting queue

    void processesRequests();
    void processPluginRequest(const PluginRequest * pluginRequest);

    // Removes torrent plugin
    // 1) Remove plugin from torrentPlugins_ map
    // 2) Deletes peer_plugin object
    // 3) Notifies controller
    void removeTorrentPlugin(const libtorrent::sha1_hash & info_hash);

    // Start plugin
    bool startTorrentPlugin(const libtorrent::sha1_hash & infoHash, const TorrentPlugin::Configuration * configuration);

    //bool startBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration & configuration);
    //bool startSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration);

    /**
     * Status
     */

    void processStatus();
    QNetworkReply * _getBalanceReply;

    // Send alert to session object
    void sendAlertToSession(const libtorrent::alert & alert);
};

#endif
