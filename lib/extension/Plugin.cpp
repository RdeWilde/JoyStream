#include "Plugin.hpp"
#include "TorrentPlugin.hpp"
#include "TorrentPluginConfiguration.hpp"
#include "Request/PluginRequest.hpp"
#include "Request/TorrentPluginRequest.hpp"
#include "Request/PeerPluginRequest.hpp"
#include "PeerPlugin.hpp"
#include "Alert/PluginStatusAlert.hpp"

#include <boost/shared_ptr.hpp>

#include <QNetworkReply>
#include <QLoggingCategory>

Plugin::Plugin(Controller * controller, QNetworkAccessManager & manager, QString bitcoindAccount, QLoggingCategory & category)
    : _controller(controller)
    , _btcClient("127.0.0.1"
                 ,8332
                 ,"bitcoinrpc"
                 ,"DDKVyZDNros2cKvkk5KpGmJWGazzYMezoWTeKaXcqxEj"
                 ,bitcoindAccount
                 ,manager)
    , _category(category)
    , _getBalanceReply(NULL)
    , _addedToSession(false) {
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
    _torrentPlugins[newTorrent->info_hash()] = torrentPlugin;

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

void Plugin::added(boost::weak_ptr<libtorrent::aux::session_impl> session) {

    qCDebug(_category) << "Plugin added to session.";

    _session = session;
    _addedToSession = true;
}

void Plugin::on_alert(libtorrent::alert const * a) {

}

void Plugin::on_tick() {

    // Only do processing if plugin has been added to session
    if(!_addedToSession)
        return;

    // Process requests from controller
    processesRequests();

    //
    processStatus();

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

void Plugin::sendAlertToSession(const libtorrent::alert & alert) {

    if(boost::shared_ptr<libtorrent::aux::session_impl> s = _session.lock())
        s->m_alerts.post_alert(alert);
    else
        qCDebug(_category) << "Session deleted.";
}

void Plugin::submitPluginRequest(PluginRequest * pluginRequest) {

    // Synchronized adding to queue
    _pluginRequestQueueMutex.lock();
    _pluginRequestQueue.enqueue(pluginRequest);
    _pluginRequestQueueMutex.unlock();
}

void Plugin::submitTorrentPluginRequest(TorrentPluginRequest * torrentPluginRequest) {

    // Synchronized adding to queue
    _torrentPluginRequestQueueMutex.lock();
    _torrentPluginRequestQueue.enqueue(torrentPluginRequest);
    _torrentPluginRequestQueueMutex.unlock();
}

void Plugin::submitPeerPluginRequest(PeerPluginRequest * peerPluginRequest) {

    // Synchronized adding to queue
    _peerPluginRequestQueueMutex.lock();
    _peerPluginRequestQueue.enqueue(peerPluginRequest);
    _peerPluginRequestQueueMutex.unlock();
}

void Plugin::processesRequests() {

    // Iterate _pluginRequestQueue
    while (!_pluginRequestQueue.empty()) {

        // Removes the head request in the queue and returns it
        PluginRequest * pluginRequest = _pluginRequestQueue.dequeue();

        // process
        processPluginRequest(pluginRequest);

        // delete
        delete pluginRequest;
    }

    // Iterate _torrentPluginRequestQueue
    while (!_torrentPluginRequestQueue.empty()) {

        // Removes the head request in the queue and returns it
        TorrentPluginRequest * torrentPluginRequest = _torrentPluginRequestQueue.dequeue();

        // find corresponding torrent plugin
        std::map<libtorrent::sha1_hash, TorrentPlugin *>::iterator i = _torrentPlugins.find(torrentPluginRequest->_info_hash);

        // process it if it exists
        if(i != _torrentPlugins.end())
            (i->second)->processTorrentPluginRequest(torrentPluginRequest);
        else
            qCDebug(_category) << "Discarded request for torrent plugin which does not exist.";

        // delete
        delete torrentPluginRequest;
    }

    // Iterate _peerPluginRequestQueue
    while (!_peerPluginRequestQueue.empty()) {

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
                qCDebug(_category) << "Discarded request for peer plugin which does not exist.";

        } else
            qCDebug(_category) << "Discarded request for torrent plugin which does not exist.";

        // delete
        delete peerPluginRequest;
    }
}

void Plugin::processPluginRequest(const PluginRequest * pluginRequest) {

    qCDebug(_category) << "processPluginRequest";

    switch(pluginRequest->getPluginRequestType()) {

    }
}

void Plugin::processStatus() {

    if(_getBalanceReply == NULL)
        _getBalanceReply = _btcClient.getBalance();
    else if(_getBalanceReply->isFinished()) {

        // If the reply is valid, we get balance
        double balance = BitCoindRPC::Client::getBalance(_getBalanceReply);

        // Delete reply and reset reply
        //delete _getBalanceReply;
        _getBalanceReply = NULL;

        // Create and send plugin status alert
        sendAlertToSession(PluginStatusAlert(balance));
    }
}

