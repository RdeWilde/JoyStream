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

boost::shared_ptr<extension::Plugin> Plugin::create(uint minimumMessageId,
                                                      libtorrent::session_handle * handle) {

    extension::Plugin * p = new extension::Plugin(minimumMessageId,
                                                  &handle->native_handle()->alerts(),
                                                  handle->native_handle(),
                                                  false);

    return boost::shared_ptr<extension::Plugin>(p);
}

Plugin::Plugin(uint minimumMessageId,
               libtorrent::alert_manager * alertManager,
               libtorrent::aux::session_impl * session,
               bool addedToSession)
    : _alertManager(alertManager)
    , _session(session)
    , _minimumMessageId(minimumMessageId)
    , _addedToSession(addedToSession)
    , _nextRequestIdentifier(0) {
}

Plugin::~Plugin() {
    std::clog << "~Plugin.";
}

boost::uint32_t Plugin::implemented_features() {
    return tick_feature;
}

boost::shared_ptr<libtorrent::torrent_plugin> Plugin::new_torrent(libtorrent::torrent_handle const & h, void*) {

    assert(_torrentPlugins.count(h.info_hash()) == 0);

    // Create a torrent plugin
    TorrentPlugin * rawTorrentPlugin = new TorrentPlugin(this,
                                                         h,
                                                         _minimumMessageId,
                                                         _alertManager,
                                                         TorrentPlugin::Policy(),
                                                         TorrentPlugin::LibtorrentInteraction::None);

    boost::shared_ptr<libtorrent::torrent_plugin> plugin(rawTorrentPlugin);

    // Storing weak reference to plugin
    _torrentPlugins[h.info_hash()] = boost::static_pointer_cast<TorrentPlugin>(plugin);

    // Send alert notification
    _alertManager->emplace_alert<alert::TorrentPluginAdded>(h, rawTorrentPlugin->status());

    return plugin;
}

void Plugin::added(libtorrent::session_handle h) {

    std::clog << "Plugin added to session." << std::endl;

    _session = h.native_handle();
    _alertManager = &h.native_handle()->alerts();
    _addedToSession = true;
}

void Plugin::on_alert(libtorrent::alert const * a) {

    if(libtorrent::read_piece_alert const * p = libtorrent::alert_cast<libtorrent::read_piece_alert>(a)) {

        // Get info hash for torrent from which this read piece comes from
        const libtorrent::sha1_hash infoHash = p->handle.info_hash();

        // Piece is not relevant unless we have a seller for given torrent
        auto it = _torrentPlugins.find(infoHash);

        if(it == _torrentPlugins.cend())
            return;

        boost::shared_ptr<TorrentPlugin> plugin = it->second.lock();
        assert(plugin);

        plugin->pieceRead(p);
    }

    if(libtorrent::torrent_removed_alert const * p = libtorrent::alert_cast<libtorrent::torrent_removed_alert>(a)) {
        const libtorrent::sha1_hash infoHash = p->handle.info_hash();
        _torrentPlugins.erase(infoHash);
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

void Plugin::processesRequestQueue() {

    while(true) {

        detail::RequestFrame frame;

        {
            // Acquire lock to request queu
            std::lock_guard<std::mutex> lock(_requestQueueMutex);

            // If queue is empty, then we are done
            if(_requestQueue.empty())
                return;

            // Copy request at front of queue
            frame = _requestQueue.front();

            // Remove front of queue
            _requestQueue.pop_front();
        }

        // Create visitor for request
        detail::RequestVariantVisitor visitor(&_torrentPlugins, _session, _alertManager, frame.first);

        // Process by applying visitor
        frame.second.apply_visitor(visitor);

    }
}

}
}
