/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/Plugin.hpp>
#include <extension/TorrentPlugin.hpp>
#include <extension/PeerPlugin.hpp>
#include <extension/Request.hpp>
#include <extension/Alert.hpp>
#include <extension/Status.hpp>

#include <boost/shared_ptr.hpp>

namespace joystream {
namespace extension {

Plugin::Plugin(const TransactionBroadcaster broadcaster,
               uint minimumMessageId)
    : _session(nullptr)
    , _broadcaster(broadcaster)
    , _minimumMessageId(minimumMessageId)
    , _addedToSession(false) {
}

Plugin::~Plugin() {
    std::clog << "~Plugin.";
}

boost::uint32_t Plugin::implemented_features() {
    return tick_feature;
}

boost::shared_ptr<libtorrent::torrent_plugin> Plugin::new_torrent(libtorrent::torrent_handle const & h, void*) {

    assert(_plugins.count(h.info_hash()) == 0);

    // Create a torrent plugin
    boost::shared_ptr<libtorrent::torrent_plugin> plugin(new TorrentPlugin(this,
                                                                           h,
                                                                           _broadcaster,
                                                                           _minimumMessageId,
                                                                           TorrentPlugin::Policy(),
                                                                           TorrentPlugin::LibtorrentInteraction::None));

    // Storing weak reference to plugin
    _plugins[h.info_hash()] = boost::static_pointer_cast<TorrentPlugin>(plugin);

    return plugin;
}

void Plugin::added(libtorrent::session_handle h) {

    std::clog << "Plugin added to session." << std::endl;

    _session = h.native_handle();
    _addedToSession = true;
}

void Plugin::on_alert(libtorrent::alert const * a) {

    if(libtorrent::read_piece_alert const * p = libtorrent::alert_cast<libtorrent::read_piece_alert>(a)) {

        // Get info hash for torrent from which this read piece comes from
        const libtorrent::sha1_hash infoHash = p->handle.info_hash();

        // Piece is not relevant unless we have a seller for given torrent
        auto it = _plugins.find(infoHash);

        if(it == _plugins.cend())
            return;

        boost::shared_ptr<TorrentPlugin> plugin = it->second.lock();
        assert(plugin);

        plugin->pieceRead(p);
    }

    if(libtorrent::torrent_removed_alert const * p = libtorrent::alert_cast<libtorrent::torrent_removed_alert>(a)) {
        const libtorrent::sha1_hash infoHash = p->handle.info_hash();
        _plugins.erase(infoHash);
    }
}

void Plugin::on_tick() {

    // Only do processing if plugin has been added to session
    if(!_addedToSession)
        return;

    // Process requests from controller
    processesRequestQueue();
}

void Plugin::save_state(libtorrent::entry &) const {
}

void Plugin::load_state(const libtorrent::bdecode_node &) {
}

status::Plugin Plugin::status() const {

    status::Plugin status;

    // Get state of each plugin
    for(auto mapping : _plugins) {

        boost::shared_ptr<TorrentPlugin> plugin = mapping.second.lock();

        assert(plugin);

        status.plugins.insert(std::make_pair(mapping.first, plugin->status()));
    }

    return status;
}

void Plugin::processesRequestQueue() {

    detail::RequestVariantVisitor visitor(this);

    // Synchronized dispatching of requests
    _requestQueueMutex.lock();

    while(!_requestQueue.empty()) {

        detail::RequestVariant v = _requestQueue.front();
        _requestQueue.pop_front();

        // Unlock queue mutex for actual processing of request
        _requestQueueMutex.unlock();

        // Process by applying visitor
        //boost::apply_visitor(visitor, v);
        v.apply_visitor(visitor);

        // Relock for checking loop condition
        _requestQueueMutex.lock();
    }

    _requestQueueMutex.unlock();
}

}
}
