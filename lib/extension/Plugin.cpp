#include "Plugin.hpp"
#include "TorrentPlugin.hpp"
#include "TorrentPluginConfiguration.hpp"
#include "controller/Controller.hpp" // needed for connecting
//#include "TorrentPluginStatus.hpp" // needed for connecting
#include "PluginRequest/PluginRequest.hpp"
#include "TorrentPluginRequest/TorrentPluginRequest.hpp"
#include "PeerPluginRequest/PeerPluginRequest.hpp"
#include "PeerPlugin.hpp"

/*
#include <QMetaType>
Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)
*/
// #include <libtorrent/socket.hpp> // tcp::endpoint

Plugin::Plugin(Controller * controller, QLoggingCategory & category)
    : _controller(controller)
    , _session(NULL)
    , _category(category) {
}

Plugin::~Plugin() {

    // No need to explicltly delete TorrentPlugin objects, since libtorrent has shared_ptr?

    // Delete TorrentPlugins!
    for(std::map<libtorrent::sha1_hash, TorrentPlugin *>::iterator i = _torrentPlugins.begin(),
            end(_torrentPlugins.end()); i != end; i++)
        delete i->second;
}

/*
Controller * Plugin::getController() {
    return _controller;
}
*/

boost::shared_ptr<libtorrent::torrent_plugin> Plugin::new_torrent(libtorrent::torrent * newTorrent, void * userData) {

    // Create the appropriate torrent plugin depending on if we have full file
    TorrentPlugin * torrentPlugin = new TorrentPlugin(this, newTorrent, _category, static_cast<TorrentPluginConfiguration *>(userData));

    // Add to collection
    _torrentPlugins.insert(std::make_pair(newTorrent->info_hash(), torrentPlugin));

    /*
    // Connect torrent plugin signal
    //qRegisterMetaType<TorrentPluginStatus>();
    QObject::connect(torrentPlugin,
                     SIGNAL(updateTorrentPluginStatus(TorrentPluginStatus)),
                     controller_,
                     SLOT(updateTorrentPluginStatus(TorrentPluginStatus)));

    //qRegisterMetaType<libtorrent::tcp::endpoint>();

    QObject::connect(torrentPlugin,
                     SIGNAL(peerAdded(libtorrent::tcp::endpoint)),
                     controller_,
                     SLOT(extensionPeerAdded(libtorrent::tcp::endpoint)));

    QObject::connect(torrentPlugin,
                     SIGNAL(peerRemoved(libtorrent::tcp::endpoint)),
                     controller_,
                     SLOT(removePeer(libtorrent::tcp::endpoint)));
    */

    // Diagnostic
    qCDebug(_category) << "Torrent #" << _torrentPlugins.size() << " added.";

    // Return
    return boost::shared_ptr<libtorrent::torrent_plugin>(torrentPlugin);
}

void Plugin::added(libtorrent::aux::session_impl * session) {
    _session = session;
}

void Plugin::on_alert(libtorrent::alert const * a) {

}

void Plugin::on_tick() {

    // Process requests from controller
    processesRequests();


}

bool Plugin::on_optimistic_unchoke(std::vector<libtorrent::policy::peer*> & peers) {
    return false;
}

void Plugin::save_state(libtorrent::entry & stateEntry) const {

}

void Plugin::load_state(libtorrent::lazy_entry const & stateEntry) {

}

void Plugin::removeTorrentPlugin(const libtorrent::sha1_hash & info_hash) {

}

void Plugin::sendAlertToSession(const libtorrent::alert & _alert) {
    _session->post_alert(_alert);
}

void Plugin::submitPluginRequest(PluginRequest * pluginRequest) {

    // Synchronized adding to queue
    _pluginRequestQueueMutex.lock();
    _pluginRequestQueue.push(pluginRequest);
    _pluginRequestQueueMutex.unlock();
}

void Plugin::submitTorrentPluginRequest(TorrentPluginRequest * torrentPluginRequest) {

    // Synchronized adding to queue
    _torrentPluginRequestQueueMutex.lock();
    _torrentPluginRequestQueue.push(torrentPluginRequest);
    _torrentPluginRequestQueueMutex.unlock();
}

void Plugin::submitPeerPluginRequest(PeerPluginRequest * peerPluginRequest) {

    // Synchronized adding to queue
    _peerPluginRequestQueueMutex.lock();
    _peerPluginRequestQueue.push(peerPluginRequest);
    _peerPluginRequestQueueMutex.unlock();
}

void Plugin::processesRequests() {

    // Iterate _pluginRequestQueue
    while (!_pluginRequestQueue.empty()) {

        // get request
        PluginRequest * pluginRequest = _pluginRequestQueue.front();

        // process
        processPluginRequest(pluginRequest);

        // remove from queue
        _pluginRequestQueue.pop();

        // delete
        delete pluginRequest;
    }

    // Iterate _torrentPluginRequestQueue
    while (!_torrentPluginRequestQueue.empty()) {

        // get request
        TorrentPluginRequest * torrentPluginRequest = _torrentPluginRequestQueue.front();

        // find corresponding torrent plugin
        std::map<libtorrent::sha1_hash, TorrentPlugin *>::iterator i = _torrentPlugins.find(torrentPluginRequest->_info_hash);

        // process it if it exists
        if(i != _torrentPlugins.end())
            (i->second)->processTorrentPluginRequest(torrentPluginRequest);
        else
            qCDebug(_category) << "Discarded request for torrent plugin which does not exist.";

        // remove from queue
        _torrentPluginRequestQueue.pop();

        // delete
        delete torrentPluginRequest;
    }

    // Iterate _peerPluginRequestQueue
    while (!_peerPluginRequestQueue.empty()) {

        // get request
        PeerPluginRequest * peerPluginRequest = _peerPluginRequestQueue.front();

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
                qCDebug(_category) << "Discarded request for peer plugin which does not exist.";

        } else
            qCDebug(_category) << "Discarded request for torrent plugin which does not exist.";

        // remove from queue
        _peerPluginRequestQueue.pop();

        // delete
        delete peerPluginRequest;
    }
}

void Plugin::processPluginRequest(const PluginRequest * pluginRequest) {

    qCDebug(_category) << "processPluginRequest";

    switch(pluginRequest->getPluginRequestType()) {

    }
}

