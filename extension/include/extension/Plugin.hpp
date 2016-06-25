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
#include <boost/variant.hpp>
#include <mutex>

namespace libtorrent {
    class alert;
    class session_impl;
}

namespace joystream {
namespace extension {

// Forward declaration for detail types
class Plugin;

namespace detail {

// Variant used to allow a single request queue
typedef boost::variant<request::Start,
                       request::Stop,
                       request::Pause,
                       request::UpdateBuyerTerms,
                       request::UpdateSellerTerms,
                       request::ToObserveMode,
                       request::ToSellMode,
                       request::ToBuyMode> RequestVariant;

// Variant visitor
class RequestVariantVisitor : public boost::static_visitor<>
{
public:

    RequestVariantVisitor(Plugin * plugin)
        : _plugin(plugin) {
    }

    template<class T>
    void operator()(const T & r) const;

private:

    Plugin * _plugin;
};

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
     * Synchornized submittal of request, request
     * object is not owned by plugin, is returned
     * in response for associating responses to initial
     * requests. Deeper association can be done by subclassing
     * requests to have custome identification data.
     */

    template<class T>
    void submit(const T &);

private:

    friend class detail::RequestVariantVisitor;

    // Libtorrent session.
    // NB: Is set by added() libtorrent callback, not constructor
    libtorrent::aux::session_impl * _session;

    // Has this plugin been added to session.
    // Do not use the _session pointer before this.
    bool _addedToSession;

    // Maps torrent hash to corresponding plugin
    std::map<libtorrent::sha1_hash, boost::weak_ptr<TorrentPlugin> > _plugins;

    //// Requests

    // Request queue
    std::deque<detail::RequestVariant> _requestQueue;

    // Synchronization lock fo _requestQueue, coordinating
    // public ::submit() and private ::processesRequestQueue()
    std::mutex _requestQueueMutex;

    // Process all requests in queue until empty. Access to queue
    // synchronized with ::submit()
    void processesRequestQueue();

    // Process a particular request in queue
    //void processRequest(const RequestVariant &);

    // Process a particular request
    template<class T>
    void processTorrentPluginRequest(const T &);
};

/// These routines are templated, and therefore inlined

template<class T>
void Plugin::submit(const T & r) {

    // Put request in container variant
    detail::RequestVariant v;
    v = r;

    // Synchronized adding to back of queue
    _requestQueueMutex.lock();
    _requestQueue.push_back(v);
    _requestQueueMutex.unlock();
}

}
}

#include <extension/Alert.hpp>

namespace joystream {
namespace extension {

template<class T>
void Plugin::processTorrentPluginRequest(const T & r) {

    // Make sure there is a torrent plugin for this torrent
    auto it = _plugins.find(r.infoHash);

    // Result to be returned to libtorrent client
    typename T::Result result;

    // Check that there is indeed a torrent corresponding to request
    if(it == _plugins.cend())
        result = typename T::Result(std::make_exception_ptr(request::MissingTorrent()));
    else {

        // Since there is, get a full reference to it
        boost::shared_ptr<TorrentPlugin> plugin = it->second.lock();
        assert(plugin);

        try {
            plugin->process<T>(r);
        } catch (...) {
            result = typename T::Result(std::current_exception());
        }
    }

    // Send result to libtorrent client
    _session->alerts().emplace_alert<alert::RequestResult<T>>(result, r.handler);
}

namespace detail {

template<class T>
void RequestVariantVisitor::operator()(const T & r) const {
    _plugin->processTorrentPluginRequest<T>(r);
}

}

}
}

#endif // JOYSTREAM_EXTENSION_PLUGIN_HPP
