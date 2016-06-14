/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_PLUGIN_HPP
#define JOYSTREAM_EXTENSION_PLUGIN_HPP

#include <extension/Status.hpp>
#include <extension/TorrentPlugin.hpp>

#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/peer.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/lazy_entry.hpp>
#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/aux_/session_impl.hpp>

#include <boost/weak_ptr.hpp>

#include <mutex>

namespace libtorrent {
    class alert;
    class session_impl;
}

namespace joystream {
namespace extension {
namespace request {
    class Request;
    class PluginRequest;
    class TorrentPluginRequest;
    class PeerPluginRequest;
}

class TorrentPlugin;

class Plugin : public libtorrent::plugin {

public:

    Plugin();

    ~Plugin();

    /**
     * All virtual functions below should only
     * be called by libtorrent network thread, never by other threads,
     * as this causes synchronization failures.
     */
    virtual void added(libtorrent::aux::session_impl * session);
    virtual void on_alert(libtorrent::alert const * a);
    virtual void on_tick();
    //virtual bool on_optimistic_unchoke(std::vector<peer_connection_handle> & /* peers */);
    virtual void save_state(libtorrent::entry & stateEntry) const;
    //virtual void load_state(const libtorrent::bdecode_node & state);
    virtual void load_state(libtorrent::lazy_entry const & stateEntry);

    // Return status of plugin
    status::Plugin status() const;

    /**
     * Synchornized submittal of request to corresponding request queue,
     * where each one is guaranteed to be serviced in FIFO.
     * Synchronized routines called from controller by Qt thread.
     * Takes ownership of request object.
     */
    void submit(const request::Request *);

private:

    // Libtorrent session.
    // NB: Is set by added() libtorrent callback, not constructor
    libtorrent::aux::session_impl * _session;

    // Has this plugin been added to session.
    // Do not use the _session pointer before this.
    bool _addedToSession;

    // Maps torrent hash to corresponding plugin
    std::map<libtorrent::sha1_hash, boost::weak_ptr<TorrentPlugin> > _plugins;

    //// Requests

    // Request queue and synchronization lock
    std::deque<request::Request *> _requestQueue;
    std::mutex _requestQueueMutex;

    // Process request queue in thread safe wya
    void processesRequests();

    // Process corresponding request type
    void process(const request::Request *);
    void processPluginRequest(const request::PluginRequest *);
    void processTorrentPluginRequest(const request::TorrentPluginRequest *);
    void processPeerPluginRequest(const request::PeerPluginRequest *);

    /**
    // Removes torrent plugin
    // 1) Remove plugin from torrentPlugins_ map
    // 2) Deletes peer_plugin object
    // 3) Notifies controller
    void removeTorrentPlugin(const libtorrent::sha1_hash & info_hash);

    // Start plugin
    //bool startTorrentPlugin(const libtorrent::sha1_hash & infoHash, const TorrentPlugin::Configuration * configuration);
    bool startBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration & configuration, const Coin::UnspentP2PKHOutput & utxo);
    bool startSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration);
    */

    // Send alert to session object
    void sendAlertToSession(const libtorrent::alert & alert);
};

}
}

#endif // JOYSTREAM_EXTENSION_PLUGIN_HPP
