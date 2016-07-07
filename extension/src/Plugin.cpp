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

Plugin::Plugin(const std::string & bep10ClientIdentifier,
               uint minimumMessageId)
    : _session(nullptr)
    , _bep10ClientIdentifier(bep10ClientIdentifier)
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
                                                                           _bep10ClientIdentifier,
                                                                           _minimumMessageId,
                                                                           TorrentPlugin::Policy(),
                                                                           TorrentPlugin::LibtorrentInteraction::None));


    // Storing weak reference to plugin
    _plugins[h.info_hash()] = boost::static_pointer_cast<TorrentPlugin>(plugin);

    return plugin;
}

void Plugin::added(libtorrent::session_handle h) {

    std::clog << "Plugin added to session.";

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
}

void Plugin::on_tick() {

    // Only do processing if plugin has been added to session
    if(!_addedToSession)
        return;

    // Process requests from controller
    processesRequestQueue();
}

/**
bool Plugin::on_optimistic_unchoke(std::vector<libtorrent::policy::peer*> & peers) {
    return false;
}
*/

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



/**
void Plugin::submit(const request::Request * r) {

    // Synchronized adding to back of queue
    _requestQueueMutex.lock();
    _requestQueue.push_back(r);
    _requestQueueMutex.unlock();
}

template<class T>
void Plugin::submit(const RequestVariant & v) {

    // Synchronized adding to back of queue
    _requestQueueMutex.lock();
    _requestQueue.push_back(v);
    _requestQueueMutex.unlock();
}
*/

void Plugin::processesRequestQueue() {
    
    // Synchronized dispatching of requests
    _requestQueueMutex.lock();
    
    while(!_requestQueue.empty()) {

        detail::RequestVariant v = _requestQueue.front();
        _requestQueue.pop_front();

        // Unlock queue mutex for actual processing of request
        _requestQueueMutex.unlock();

        // Process by applying visitor
        boost::apply_visitor(detail::RequestVariantVisitor(this), v);

        // Relock for checking loop condition
        _requestQueueMutex.lock();
    }
}

void Plugin::sendStatusAlert() {
    _session->alerts().emplace_alert<alert::PluginStatus>(status());
}

/**
void Plugin::processRequest(const RequestVariant & v) {

    if(typeid(const request::Start *) == v.type())
        processTorrentPluginRequest<request::Start>(boost::get<const request::Start *>(v));
    else if(typeid(const request::Stop *) == v.type())
        processTorrentPluginRequest<request::Stop>(boost::get<const request::Stop *>(v));
    else if (typeid(const request::Pause *) == v.type())
        processTorrentPluginRequest<request::Pause>(boost::get<const request::Pause *>(v));
    else if (typeid(const request::UpdateBuyerTerms *) == v.type())
        processTorrentPluginRequest<request::UpdateBuyerTerms>(boost::get<const request::UpdateBuyerTerms *>(v));
    else if (typeid(const request::UpdateSellerTerms *) == v.type())
        processTorrentPluginRequest<request::UpdateSellerTerms>(boost::get<const request::UpdateSellerTerms *>(v));
    else if (typeid(const request::ToObserveMode *) == v.type())
        processTorrentPluginRequest<request::ToObserveMode>(boost::get<const request::ToObserveMode *>(v));
    else if (typeid(const request::ToSellMode *) == v.type())
        processTorrentPluginRequest<request::ToSellMode>(boost::get<const request::ToSellMode *>(v));
    else if (typeid(const request::ToBuyMode *) == v.type())
        processTorrentPluginRequest<request::ToBuyMode>(boost::get<const request::ToBuyMode *>(v));
}


template<class T>
boost::shared_ptr<TorrentPlugin> Plugin::processMissingTorrent(const request::T * r) {

    // Make sure there is a torrent plugin for this torrent
    auto it = _plugins.find(r->infoHash);

    // If there is no torrent plugin, then tell client
    if(it == _plugins.cend()) {

        _session->alerts().emplace_alert<alert::SubroutineResult<request::T::Result>>(request::T::Result(r, request::MissingTorrent()));
        return boost::shared_ptr<TorrentPlugin>();

    } else {

        // Make sure the plugin is still valid
        boost::shared_ptr<TorrentPlugin> plugin = it->second.lock();

        assert(plugin);

        return plugin;
    }
}

void Plugin::process(const request::Start * r) {

    boost::shared_ptr<TorrentPlugin> plugin = processMissingTorrent<Start>(r);

    // Notify libtorrent client
    if(plugin) {

        // Stop torrent plugin
        request::Start::Result result = plugin->start(r);

        // Return result to libtorrent client
        sendSubRoutineResult<Start>(result);
    }
}

void Plugin::process(const request::Stop * r) {

    boost::shared_ptr<TorrentPlugin> plugin = processMissingTorrent<Stop>(r);

    if(plugin) {

        // Stop torrent plugin
        request::Stop::Result result = plugin->stop(r);

        // Return result to libtorrent client
        sendSubRoutineResult<Stop>(result);
    }
}

void Plugin::process(const request::Pause * r) {

    boost::shared_ptr<TorrentPlugin> plugin = processMissingTorrent<Pause>(r);

    if(plugin) {

        // Pause torrent plugin
        request::Stop::Result result = plugin->pause(r);

        // Return result to libtorrent client
        sendSubRoutineResult<Pause>(result);
    }
}

void Plugin::process(const request::UpdateBuyerTerms * r) {

    boost::shared_ptr<TorrentPlugin> plugin = processMissingTorrentPlugin<UpdateBuyerTerms>(r);

    if(plugin) {

        // Update buyer terms
        request::UpdateBuyerTerms::Result = plugin->updateBuyerTerms(r);

        // Return result to libtorrent lient
        sendSubRoutineResult<UpdateBuyerTerms>(result);
    }
}

void Plugin::process(const request::UpdateSellerTerms *) {

    boost::shared_ptr<TorrentPlugin> plugin = processMissingTorrentPlugin<UpdateSellerTerms>(r);

    if(plugin) {

        // Update buyer terms
        request::UpdateBuyerTerms::Result = plugin->updateSellerTerms(r);

        // Return result to libtorrent lient
        sendSubRoutineResult<UpdateSellerTerms>(result);
    }
}

void Plugin::process(const request::ToObserveMode *) {

    // Clear relevant mappings
    // NB: We are doing clearing regardless of whether operation is successful!
    if(_session.mode() == protocol_session::SessionMode::selling)
        _outstandingLoadPieceForBuyerCalls.clear();
    else if(_session.mode() == protocol_session::SessionMode::buying)
        _outstandingFullPieceArrivedCalls.clear();

    manager.emplace_alert<alert::SubroutineResult<request::ToObserveMode::Response>>(h, request::ToObserveMode::Response(toObserveModeRequest, toObserveMode()));

    //manager.emplace_alert<alert::RequestResult<request::ToObserveMode::Response>>(h, toObserveModeRequest, toObserveMode());

}

void Plugin::process(const request::ToSellMode *) {

    // Should have been cleared before
    assert(_outstandingLoadPieceForBuyerCalls.empty());

    // Clear relevant mappings
    // NB: We are doing clearing regardless of whether operation is successful!
    if(_session.mode() == protocol_session::SessionMode::buying)
        _outstandingFullPieceArrivedCalls.clear();


    manager.emplace_alert<alert::SubroutineResult<request::ToSellMode::Response>>(h, request::ToSellMode::Response(toSellModeRequest, toSellMode(toSellModeRequest->generateKeyPairsCallbackHandler,
                                                                                                             toSellModeRequest->generateP2PKHAddressesCallbackHandler,
                                                                                                             toSellModeRequest->sellingPolicy,
                                                                                                             toSellModeRequest->terms)));

}

void Plugin::process(const request::ToBuyMode *) {

    // Should have been cleared before
    assert(_outstandingFullPieceArrivedCalls.empty());

    // Clear relevant mappings
    // NB: We are doing clearing regardless of whether operation is successful!
    if(_session.mode() == protocol_session::SessionMode::selling)
        _outstandingLoadPieceForBuyerCalls.clear();


    manager.emplace_alert<alert::SubroutineResult<request::ToBuyMode::Response>>(h, request::ToBuyMode::Response(toBuyModeRequest, toBuyMode(toBuyModeRequest->generateKeyPairsCallbackHandler,
                                                                                                           toBuyModeRequest->generateP2PKHAddressesCallbackHandler,
                                                                                                           toBuyModeRequest->funding,
                                                                                                           toBuyModeRequest->policy,
                                                                                                           toBuyModeRequest->terms)));
}

template<class T>
void Plugin::sendSubRoutineResult(const request::T::Result & result) {
    _session->alerts().emplace_alert<alert::SubroutineResult<request::T::Result>>(result);
}
*/

}
}
