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

Plugin::Plugin()
    : _addedToSession(false) {
}

Plugin::~Plugin() {
    std::log << "~Plugin.";
}

void Plugin::added(libtorrent::aux::session_impl * session) {

    std::log << "Plugin added to session.";

    _session = session;
    _addedToSession = true;
}

void Plugin::on_alert(libtorrent::alert const * a) {

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

void Plugin::on_tick() {

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

void Plugin::save_state(libtorrent::entry & stateEntry) const {

}

void Plugin::load_state(libtorrent::lazy_entry const & stateEntry) {
//void Plugin::load_state(const libtorrent::bdecode_node & state) {
}

status::Plugin Plugin::status() const {

    status::Plugin status;

    // Get state of each plugin
    for(auto mapping : _plugins) {

        boost::shared_ptr<TorrentPlugin> plugin = mapping.second.lock();

        assert(plugin);

        status.plugins.insert(make_pair(mapping.first, plugin->status()));
    }

    return status;
}

void Plugin::submit(const request::Request * r) {

    // Synchronized adding to back of queue
    _requestQueueMutex.lock();
    _requestQueue.push_back(r);
    _requestQueueMutex.unlock();
}

void Plugin::processesRequests() {
    
    // Synchronized dispatching of requests
    _requestQueueMutex.lock();
    
    while(!_requestQueue.empty()) {

        request::Request * r = _requestQueue.front();
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

void Plugin::processes(const request::Request * r) {

    switch(r->target()) {
        case request::RequestTarget::Plugin:
            processPluginRequest(static_cast<const request::PluginRequest *>(r));
            break;
        case request::RequestTarget::TorrentPlugin:
            processTorrentPluginRequest(static_cast<const request::TorrentPluginRequest *>(r));
            break;
        case request::RequestTarget::PeerPlugin:
            processPeerPluginRequest(static_cast<const request::PeerPluginRequest *>(r));
            break;
        default:
            assert(false);
    }

}

void Plugin::processPluginRequest(const request::PluginRequest * r) {

    assert(r->target() == request::RequestTarget::Plugin);

    throw std::runtime_error("Plugin::processPluginRequest: not implemented");

    /**
    switch(pluginRequest->getPluginRequestType()) {


        case PluginRequestType::StartTorrentPlugin:  {

            std::clog << "Starting torrent plugin";

            const StartTorrentPlugin * p = reinterpret_cast<const StartTorrentPlugin *>(pluginRequest);
            startTorrentPlugin(p->infoHash(), p->configuration());

            // Delete configuration
            // enable later
            //delete p->configuration();
        }

        break;


        case PluginRequestType::StartBuyerTorrentPlugin: {

                const StartBuyerTorrentPlugin * p = reinterpret_cast<const StartBuyerTorrentPlugin *>(pluginRequest);

                startBuyerTorrentPlugin(p->infoHash(), p->configuration(), p->utxo());
            }

            break;

        case PluginRequestType::StartSellerTorrentPlugin: {

                const StartSellerTorrentPlugin * p = reinterpret_cast<const StartSellerTorrentPlugin *>(pluginRequest);

                startSellerTorrentPlugin(p->infoHash(), p->configuration());
            }

            break;

        assert(false);

    }
    */
}

void Plugin::processTorrentPluginRequest(const request::TorrentPluginRequest * r) {

    assert(r->target() == request::RequestTarget::TorrentPlugin);

    // Make sure there is a torrent plugin for this torrent
    auto it = _plugins.find(r->infoHash);

    if(it == _plugins.cend())
        //throw exception

    // Make sure the plugin is still valid
    boost::shared_ptr<TorrentPlugin> plugin = it->second.lock();

    if(!plugin)
        //throw exception

    // Have plugin handle request
    plugin->handle(r);
}

void Plugin::processPeerPluginRequest(const request::PeerPluginRequest * r) {

    assert(r->target() == request::RequestTarget::PeerPlugin);

    throw std::runtime_error("Plugin::processPeerPluginRequest: not implemented");

    /**
    // Iterate _peerPluginRequestQueue
    _peerPluginRequestQueueMutex.lock();
    while (!_peerPluginRequestQueue.empty()) {

        std::clog << "Cannot process peer plugin request.";

        // Removes the head request in the queue and returns it
        PeerPluginRequest * peerPluginRequest = _peerPluginRequestQueue.dequeue();

        // find corresponding torrent plugin
        std::map<libtorrent::sha1_hash, TorrentPlugin *>::iterator i = _torrentPlugins.find(peerPluginRequest->_info_hash);

        // process it if it exists
        if(i != _torrentPlugins.end()) {

            // get torrent plugin
            TorrentPlugin * torrentPlugin = i->second;

            // get peer plugin
            PeerPlugin * peerPlugin = torrentPlugin->getPeerPlugin(peerPluginRequest->_endpoint);

            // process if match was found
            if(peerPlugin != NULL)
                peerPlugin->processPeerPluginRequest(peerPluginRequest);
            else
                std::clog << "Discarded request for peer plugin which does not exist.";

        } else
            std::clog << "Discarded request for torrent plugin which does not exist.";

        // delete
        delete peerPluginRequest;
    }
    _peerPluginRequestQueueMutex.unlock();
    */
}

void Plugin::sendAlertToSession(const libtorrent::alert & alert) {
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
