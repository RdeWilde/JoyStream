/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/peer.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/lazy_entry.hpp>
#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/aux_/session_impl.hpp>

#include <boost/weak_ptr.hpp>

// Forward declaration
class Controller;
class TorrentPlugin;
class SellerTorrentPlugin;
class BuyerTorrentPlugin;
class PluginRequest;
class TorrentPluginRequest;
class PeerPluginRequest;
class TorrentPluginConfiguration;

namespace libtorrent {
    class alert;
    class session_impl;
}

namespace joystream {
namespace extension {

class Plugin : public libtorrent::plugin {

public:

    class Status {

    public:

        // Default constructor
        Status();

        // Constructor from members
        Status(quint64 totalReceivedSinceStart, quint64 totalSentSinceStart, quint64 _totalCurrentlyLockedInChannels);

        // Getters and setters
        quint64 totalReceivedSinceStart() const;
        void setTotalReceivedSinceStart(quint64 totalReceivedSinceStart);

        quint64 totalSentSinceStart() const;
        void setTotalSentSinceStart(quint64 totalSentSinceStart);

        quint64 totalCurrentlyLockedInChannels() const;
        void setTotalCurrentlyLockedInChannels(quint64 totalCurrentlyLockedInChannels);

    private:

        // Amount of funds (satoshies) received since start
        quint64 _totalReceivedSinceStart;

        // Amount of funds (satoshies) sent since start
        quint64 _totalSentSinceStart;

        // Amount of funds (satoshies) presently locked
        // in channels started during this session.
        // Obviosuly does not include change in channels!
        quint64 _totalCurrentlyLockedInChannels;
    };

    // Constructor
    Plugin();

    // Destructor
    ~Plugin();

    /**
     * All virtual functions below should ONLY
     * be called by libtorrent network thread,
     * never by other threads, as this causes synchronization
     * failures.
     */
    virtual void added(libtorrent::aux::session_impl * session);
    virtual void on_alert(libtorrent::alert const * a);
    virtual void on_tick();
    //virtual bool on_optimistic_unchoke(std::vector<peer_connection_handle> & /* peers */);
    virtual void save_state(libtorrent::entry & stateEntry) const;
    //virtual void load_state(const libtorrent::bdecode_node & state);
    virtual void load_state(libtorrent::lazy_entry const & stateEntry);

    // Return status of plugin
    Status status() const;

    // Setter routines which update status information
    quint64 registerReceivedFunds(quint64 value);
    quint64 registerSentFunds(quint64 value);
    quint64 registerLockedInChannelsFunds(quint64 value);

    quint64 registerUnLockedFromChannelFunds(quint64 value);


    /**
     * Synchronized routines called from controller by Qt thread.
     *
     * In all of these routines, plugin takes ownership of request object.
     */
    void submitPluginRequest(PluginRequest * pluginRequest);
    void submitTorrentPluginRequest(TorrentPluginRequest * torrentPluginRequest);
    void submitPeerPluginRequest(PeerPluginRequest * peerPluginRequest);

private:

    // Libtorrent session.
    // Is set by added() libtorrent hook, not constructor
    libtorrent::aux::session_impl * _session;

    QMap<libtorrent::sha1_hash, boost::weak_ptr<BuyerTorrentPlugin> > _buyerPlugins;
    QMap<libtorrent::sha1_hash, boost::weak_ptr<SellerTorrentPlugin> > _sellerPlugins;

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

    // Processing routines
    void processesRequests();
    void processPluginRequest(const PluginRequest * pluginRequest);
    void processTorrentPluginRequest(const TorrentPluginRequest * torrentPluginRequest);

    // Removes torrent plugin
    // 1) Remove plugin from torrentPlugins_ map
    // 2) Deletes peer_plugin object
    // 3) Notifies controller
    void removeTorrentPlugin(const libtorrent::sha1_hash & info_hash);

    // Start plugin
    //bool startTorrentPlugin(const libtorrent::sha1_hash & infoHash, const TorrentPlugin::Configuration * configuration);

    bool startBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration & configuration, const Coin::UnspentP2PKHOutput & utxo);
    bool startSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration);

    // Send alert to session object
    void sendAlertToSession(const libtorrent::alert & alert);
};

}
}

#endif
