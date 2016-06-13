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

template<class T>
Plugin<T>::Plugin()
    : _addedToSession(false) {
}

template<class T>
Plugin<T>::~Plugin() {
    std::log << "~Plugin.";
}

template<class T>
void Plugin<T>::added(libtorrent::aux::session_impl * session) {

    std::log << "Plugin added to session.";

    _session = session;
    _addedToSession = true;
}

template<class T>
void Plugin<T>::on_alert(libtorrent::alert const * a) {

    if(libtorrent::read_piece_alert const * p = libtorrent::alert_cast<libtorrent::read_piece_alert>(a)) {

        // Get info hash for torrent from which this read piece comes from
        const libtorrent::sha1_hash infoHash = p->handle.info_hash();

        // Piece is not relevant unless we have a seller for given torrent
        if(!_sellerPlugins.contains(infoHash))
            return;

        // Send alert to plugin
        if(boost::shared_ptr<SellerTorrentPlugin> sharedPtr = _sellerPlugins[infoHash].lock())
            sharedPtr->pieceRead(p);
        else {

            std::clog << "Could not process read piece alert, since seller torrent plugin has expired.";
            assert(false);
        }
    }

}

template<class T>
void Plugin<T>::on_tick() {

    // Only do processing if plugin has been added to session
    if(!_addedToSession)
        return;

    // Process requests from controller
    processesRequests();

    // Send status
    sendAlertToSession(PluginStatusAlert(status()));
}

/**
bool Plugin::on_optimistic_unchoke(std::vector<libtorrent::policy::peer*> & peers) {
    return false;
}
*/

template<class T>
void Plugin<T>::save_state(libtorrent::entry & stateEntry) const {

}

template<class T>
void Plugin::load_state(libtorrent::lazy_entry const & stateEntry) {
//void Plugin::load_state(const libtorrent::bdecode_node & state) {
}

template<class T>
status::Plugin Plugin<T>::status() const {

    status::Plugin status;

    // Get state of each plugin
    for(auto mapping : _plugins) {

        boost::shared_ptr<TorrentPlugin> plugin = mapping.second.lock();

        assert(plugin);

        status.plugins.insert(make_pair(mapping.first, plugin->status()));
    }

    return status;
}

template<class T>
void Plugin<T>::submit(const request::Request<T> * r) {

    // Synchronized adding to back of queue
    _requestQueueMutex.lock();
    _requestQueue.push_back(r);
    _requestQueueMutex.unlock();
}

template<class T>
void Plugin<T>::processesRequests() {
    
    // Synchronized dispatching of requests
    _requestQueueMutex.lock();
    
    while(!_requestQueue.empty()) {

        request::Request<T> * r = _requestQueue.front();
        _requestQueue.pop_front();

        // Unlock queue mutex for actual processing of request
        _requestQueueMutex.unlock();

        // Process
        processes(r);

        // Delete request
        delete r;

        // Relock for checking loop condition
        _requestQueueMutex.lock();
    }
}

template<class T>
void Plugin<T>::processes(const request::Request<T> * r) {

    switch(r->target()) {
        case request::RequestTarget::Plugin:
            processPluginRequest(static_cast<const request::PluginRequest<T> *>(r));
            break;
        case request::RequestTarget::TorrentPlugin:
            processTorrentPluginRequest(static_cast<const request::TorrentPluginRequest<T> *>(r));
            break;
        case request::RequestTarget::PeerPlugin:
            processPeerPluginRequest(static_cast<const request::PeerPluginRequest<T> *>(r));
            break;
        default:
            assert(false);
    }

}

template<class T>
void Plugin<T>::processPluginRequest(const request::PluginRequest<T> * r) {

    assert(r->target() == request::RequestTarget::Plugin);

    throw std::runtime_error("Plugin::processPluginRequest: not implemented");
}

template<class T>
void Plugin<T>::processTorrentPluginRequest(const request::TorrentPluginRequest<T> * r) {

    assert(r->target() == request::RequestTarget::TorrentPlugin);

    // Make sure there is a torrent plugin for this torrent
    auto it = _plugins.find(r->infoHash);

    // If there is no torrent plugin, then tell client
    if(it == _plugins.cend()) {
        sendAlertToSession(alert::RequestResult<T, request::TorrentPluginRequest<T>::MissingTorrentPlugin>(r->identifier, r->type()));
        return;
    }

    // Make sure the plugin is still valid
    //boost::shared_ptr<TorrentPlugin> plugin = it->second.lock();

    assert(plugin);

    // Have plugin handle request
    plugin->handle(r);
}

template<class T>
void Plugin<T>::processPeerPluginRequest(const request::PeerPluginRequest<T> * r) {

    assert(r->target() == request::RequestTarget::PeerPlugin);

    throw std::runtime_error("Plugin::processPeerPluginRequest: not implemented");
}

template<class T>
void Plugin<T>::sendAlertToSession(const libtorrent::alert & alert) {
    _session->m_alerts.post_alert(alert); // emplace_alert<listen_succeeded_alert>(*)
}

/**
void Plugin::removeTorrentPlugin(const libtorrent::sha1_hash & info_hash) {
    std::clog << "Plugin::removeTorrentPlugin: NOT IMPLEMENTED!!";
}

bool Plugin::startTorrentPlugin(const libtorrent::sha1_hash & infoHash, const TorrentPlugin::Configuration & configuration) {

    // Torrent should never have a plugin installed
    assert(!_plugins.contains(infoHash));

    // Find torrent
    boost::weak_ptr<libtorrent::torrent> weakTorrentPtr = sharedSessionPtr->find_torrent(infoHash);

    // Get shared torrent pointer
    if(boost::shared_ptr<libtorrent::torrent> sharedTorrentPtr = weakTorrentPtr.lock()) {

        // Shared pointer used to point to plugin
        boost::shared_ptr<libtorrent::torrent_plugin> sharedPluginPtr;

        // Create corresponding torrent plugin
        switch(configuration->pluginMode()) {

            case PluginMode::Buyer:

                sharedPluginPtr.reset(new BuyerTorrentPlugin(this, sharedTorrentPtr, _wallet, configuration, utxo, _category));
                break;

            case PluginMode::Seller:

                sharedPluginPtr.reset(new SellerTorrentPlugin(this, sharedTorrentPtr, configuration, _category));
                break;

            case PluginMode::Observer:

                std::clog << "Observer not implemented yet.";
        }

        // Install plugin on torrent
        sharedTorrentPtr->add_extension(sharedPluginPtr);

        // Remember plugin
        _plugins[infoHash] = boost::dynamic_pointer_cast<TorrentPlugin>(sharedPluginPtr);

        // Return success indication
        return true;

    } else {

        std::clog << "Torrent deleted, cannot install seller torrent pluin.";
        return false;
    }
}


bool Plugin::startBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration & configuration, const Coin::UnspentP2PKHOutput & utxo) {

    // Check that torrent does not already have a plugin installed
    if(_sellerPlugins.contains(infoHash) || _plugins.contains(infoHash)) {
        std::clog << "Torrent already has plugin installed, remove first.";
        assert(false);
        return false;
    }

    // Find torrent
    boost::weak_ptr<libtorrent::torrent> weakTorrentPtr = _session->find_torrent(infoHash);

    // Get shared torrent pointer
    if(boost::shared_ptr<libtorrent::torrent> sharedTorrentPtr = weakTorrentPtr.lock()) {

        // Create buyer torrent plugin
        BuyerTorrentPlugin * buyerPlugin = new BuyerTorrentPlugin(this, sharedTorrentPtr, _wallet, configuration, utxo, _category);

        // Create plugin with given configuration
        boost::shared_ptr<BuyerTorrentPlugin> sharedPluginPtr(buyerPlugin);

        // Install plugin on torrent
        sharedTorrentPtr->add_extension(sharedPluginPtr);

        // Remember plugin
        _plugins[infoHash] = boost::weak_ptr<BuyerTorrentPlugin>(sharedPluginPtr);

        // Notify controller
        sendAlertToSession(StartedBuyerTorrentPlugin(infoHash, configuration, utxo, buyerPlugin->status()));

        // Return success indication
        return true;

    } else {

        std::clog << "Torrent deleted, cannot install buyer torrent pluin.";
        assert(false);
        return false;
    }
}

bool Plugin::startSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration) {

    // Check that torrent does not already have a plugin installed
    if(_sellerPlugins.contains(infoHash) || _plugins.contains(infoHash)) {

        std::clog << "Torrent already has plugin installed, remove first.";
        assert(false);
        return false;
    }

    // Find torrent
    boost::weak_ptr<libtorrent::torrent> weakTorrentPtr = _session->find_torrent(infoHash);

    // Get shared torrent pointer
    if(boost::shared_ptr<libtorrent::torrent> sharedTorrentPtr = weakTorrentPtr.lock()) {

        // Create torrent plugin
        SellerTorrentPlugin * sellerPlugin = new SellerTorrentPlugin(this, sharedTorrentPtr, _wallet, configuration, _category);

        // Create plugin with given configuration
        boost::shared_ptr<SellerTorrentPlugin> sharedPluginPtr(sellerPlugin);

        // Install plugin on torrent
        sharedTorrentPtr->add_extension(sharedPluginPtr);

        // Remember plugin
        _sellerPlugins[infoHash] = boost::weak_ptr<SellerTorrentPlugin>(sharedPluginPtr);

        // Notify controller
        sendAlertToSession(StartedSellerTorrentPlugin(infoHash, configuration, sellerPlugin->status()));

        // Return success indication
        return true;

    } else {

        std::clog << "Torrent deleted, cannot install seller torrent pluin.";
        assert(false);
        return false;
    }
}

*/

}
}
