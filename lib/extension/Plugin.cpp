#include "Plugin.hpp"
//#include "TorrentPlugin.hpp"
#include "SellerTorrentPlugin.hpp"
#include "BuyerTorrentPlugin.hpp"
//#include "TorrentPluginConfiguration.hpp"
#include "Request/PluginRequest.hpp"
#include "Request/TorrentPluginRequest.hpp"
#include "Request/PeerPluginRequest.hpp"
//#include "Request/StartBuyerTorrentPlugin.hpp"
//#include "Request/StartSellerTorrentPlugin.hpp"
#include "Request/StartTorrentPlugin.hpp"
#include "PeerPlugin.hpp"
#include "Alert/PluginStatusAlert.hpp"

#include "PluginMode.hpp"

#include "BitCoin/Wallet.hpp"

#include <boost/shared_ptr.hpp>

#include <QNetworkReply>
#include <QLoggingCategory>

Plugin::Plugin(Controller * controller, Wallet * wallet, QNetworkAccessManager & manager, QString bitcoindAccount, QLoggingCategory & category)
    : _controller(controller)
    , _wallet(wallet)
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

    /**
    // Delete TorrentPlugins!
    for(std::map<libtorrent::sha1_hash, TorrentPlugin *>::iterator i = _torrentPlugins.begin(),
            end(_torrentPlugins.end()); i != end; i++)
        delete i->second;
    */
}

/**
boost::shared_ptr<libtorrent::torrent_plugin> Plugin::new_torrent(libtorrent::torrent * newTorrent, void * userData) {

    // No longer used
    // Create the appropriate torrent plugin depending on if we have full file
    TorrentPlugin * torrentPlugin = new TorrentPlugin(this, newTorrent, _category, static_cast<TorrentPluginConfiguration *>(userData));

    // Add to collection
    _torrentPlugins[newTorrent->info_hash()] = torrentPlugin;

    // Diagnostic
    qCDebug(_category) << "Torrent #" << _torrentPlugins.size() << " added.";

    // Return
    return boost::shared_ptr<libtorrent::torrent_plugin>(torrentPlugin);
}
*/

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

    /**
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
    */
}

void Plugin::processPluginRequest(const PluginRequest * pluginRequest) {

    qCDebug(_category) << "processPluginRequest";

    switch(pluginRequest->getPluginRequestType()) {

        case PluginRequestType::StartTorrentPlugin:  {

            qCDebug(_category) << "Starting torrent plugin";

            const StartTorrentPlugin * p = reinterpret_cast<const StartTorrentPlugin *>(pluginRequest);
            startTorrentPlugin(p->infoHash(), p->configuration());

            // Delete configuration
            // enable later
            //delete p->configuration();
        }

        break;

        /**
        case PluginRequestType::StartBuyerTorrentPlugin: {

                const StartBuyerTorrentPlugin * p = reinterpret_cast<const StartBuyerTorrentPlugin *>(pluginRequest);
                startBuyerTorrentPlugin(p->infoHash(), p->configuration());
            }

            break;
        case PluginRequestType::StartSellerTorrentPlugin: {

                const StartSellerTorrentPlugin * p = reinterpret_cast<const StartSellerTorrentPlugin *>(pluginRequest);
                startSellerTorrentPlugin(p->infoHash(), p->configuration());
        }
            break;
        */
    }
}

bool Plugin::startTorrentPlugin(const libtorrent::sha1_hash & infoHash, const TorrentPlugin::Configuration * configuration) {

    // Torrent should never have a plugin installed
    Q_ASSERT(!_plugins.contains(infoHash));

    // Get shared pointer to session
    if(boost::shared_ptr<libtorrent::aux::session_impl> sharedSessionPtr = _session.lock()) {

        // Find torrent
        boost::weak_ptr<libtorrent::torrent> weakTorrentPtr = sharedSessionPtr->find_torrent(infoHash);

        // Get shared torrent pointer
        if(boost::shared_ptr<libtorrent::torrent> sharedTorrentPtr = weakTorrentPtr.lock()) {

            // Shared pointer used to point to plugin
            boost::shared_ptr<libtorrent::torrent_plugin> sharedPluginPtr;

            // Create corresponding torrent plugin
            switch(configuration->pluginMode()) {

                case PluginMode::Buyer:

                    sharedPluginPtr.reset(new BuyerTorrentPlugin(this,
                                                                 weakTorrentPtr,
                                                                 _wallet,
                                                                 *(reinterpret_cast<const BuyerTorrentPlugin::Configuration *>(configuration)),
                                                                 _category));
                    break;

                case PluginMode::Seller:

                    sharedPluginPtr.reset(new SellerTorrentPlugin(this, weakTorrentPtr, *(reinterpret_cast<const SellerTorrentPlugin::Configuration *>(configuration)), _category));
                    break;

                case PluginMode::Observer:

                    qCDebug(_category) << "Not implemented yet.";
            }

            // Install plugin on torrent
            sharedTorrentPtr->add_extension(sharedPluginPtr);

            // Remember plugin
            _plugins[infoHash] = boost::weak_ptr<libtorrent::torrent_plugin>(sharedPluginPtr);

            // Return success indication
            return true;

        } else {

            qCDebug(_category) << "Torrent deleted, cannot install seller torrent pluin.";
            return false;
        }

    } else {

        qCDebug(_category) << "Session deleted, cannot install seller torrent pluin.";
        return false;
    }
}

/**
bool Plugin::startBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration & configuration) {

    // Check that torrent does not already have a plugin installed
    if(_plugins.contains(infoHash)) {

        qCDebug(_category) << "Torrent already has plugin installed, remove first.";
        return false;
    }

    // Get shared pointer to session
    if(boost::shared_ptr<libtorrent::aux::session_impl> sharedSessionPtr = _session.lock()) {

        // Find torrent
        boost::weak_ptr<libtorrent::torrent> weakTorrentPtr = sharedSessionPtr->find_torrent(infoHash);

        // Get shared torrent pointer
        if(boost::shared_ptr<libtorrent::torrent> sharedTorrentPtr = weakTorrentPtr.lock()) {

            // Create plugin with given configuration
            boost::shared_ptr<libtorrent::torrent_plugin> sharedPluginPtr(new BuyerTorrentPlugin(this, weakTorrentPtr, configuration, _category));

            // Install plugin on torrent
            sharedTorrentPtr->add_extension(sharedPluginPtr);

            // Remember plugin
            _plugins[infoHash] = boost::weak_ptr<libtorrent::torrent_plugin>(sharedPluginPtr);

            // Return success indication
            return true;

        } else {

            qCDebug(_category) << "Torrent deleted, cannot install buyer torrent pluin.";
            return false;
        }

    } else {

        qCDebug(_category) << "Session deleted, cannot install buyer torrent pluin.";
        return false;
    }
}

bool Plugin::startSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration) {

    // Check that torrent does not already have a plugin installed
    if(_plugins.contains(infoHash)) {

        qCDebug(_category) << "Torrent already has plugin installed, remove first.";
        return false;
    }

    // Get shared pointer to session
    if(boost::shared_ptr<libtorrent::aux::session_impl> sharedSessionPtr = _session.lock()) {

        // Find torrent
        boost::weak_ptr<libtorrent::torrent> weakTorrentPtr = sharedSessionPtr->find_torrent(infoHash);

        // Get shared torrent pointer
        if(boost::shared_ptr<libtorrent::torrent> sharedTorrentPtr = weakTorrentPtr.lock()) {

            // Create plugin with given configuration
            boost::shared_ptr<libtorrent::torrent_plugin> sharedPluginPtr(new SellerTorrentPlugin(this, weakTorrentPtr, configuration, _category));

            // Install plugin on torrent
            sharedTorrentPtr->add_extension(sharedPluginPtr);

            // Remember plugin
            _plugins[infoHash] = boost::weak_ptr<libtorrent::torrent_plugin>(sharedPluginPtr);

            // Return success indication
            return true;

        } else {

            qCDebug(_category) << "Torrent deleted, cannot install seller torrent pluin.";
            return false;
        }

    } else {

        qCDebug(_category) << "Session deleted, cannot install seller torrent pluin.";
        return false;
    }
}
*/

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
