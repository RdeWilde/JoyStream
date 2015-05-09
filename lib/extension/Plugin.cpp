#include "Plugin.hpp"
#include "TorrentPlugin.hpp"
#include "SellerTorrentPlugin.hpp"
#include "BuyerTorrentPlugin.hpp"
//#include "TorrentPluginConfiguration.hpp"
#include "Request/PluginRequest.hpp"
#include "Request/TorrentPluginRequest.hpp"
#include "Request/PeerPluginRequest.hpp"
#include "Request/StartBuyerTorrentPlugin.hpp"
#include "Request/StartSellerTorrentPlugin.hpp"
//#include "Request/StartTorrentPlugin.hpp"
#include "PeerPlugin.hpp"

#include "Alert/PluginStatusAlert.hpp"
//#include "Alert/TorrentPluginStartedAlert.hpp"
#include "Alert/StartedSellerTorrentPlugin.hpp"
#include "Alert/StartedBuyerTorrentPlugin.hpp"

#include "PluginMode.hpp"

#include "BitCoin/Wallet.hpp"

#include <boost/shared_ptr.hpp>

#include <QNetworkReply>
#include <QLoggingCategory>


/**
 * Plugin::Status
 */

Plugin::Status::Status()
    : _totalReceivedSinceStart(0)
    , _totalSentSinceStart(0)
    , _totalCurrentlyLockedInChannels(0) {
}

Plugin::Status::Status(quint64 totalReceivedSinceStart, quint64 totalSentSinceStart, quint64 totalCurrentlyLockedInChannels)
    : _totalReceivedSinceStart(totalReceivedSinceStart)
    , _totalSentSinceStart(totalSentSinceStart)
    , _totalCurrentlyLockedInChannels(totalCurrentlyLockedInChannels){
}

quint64 Plugin::Status::totalCurrentlyLockedInChannels() const {
    return _totalCurrentlyLockedInChannels;
}

void Plugin::Status::setTotalCurrentlyLockedInChannels(quint64 totalCurrentlyLockedInChannels) {
    _totalCurrentlyLockedInChannels = totalCurrentlyLockedInChannels;
}

quint64 Plugin::Status::totalSentSinceStart() const {
    return _totalSentSinceStart;
}

void Plugin::Status::setTotalSentSinceStart(quint64 totalSentSinceStart) {
    _totalSentSinceStart = totalSentSinceStart;
}

quint64 Plugin::Status::totalReceivedSinceStart() const
{
    return _totalReceivedSinceStart;
}

void Plugin::Status::setTotalReceivedSinceStart(quint64 totalReceivedSinceStart) {
    _totalReceivedSinceStart = totalReceivedSinceStart;
}

/**
 * Plugin
 */

Plugin::Plugin(Controller * controller, Wallet * wallet, QNetworkAccessManager & manager, QString bitcoindAccount, QLoggingCategory & category)
    : _controller(controller)
    , _wallet(wallet)
    /**
    , _btcClient("127.0.0.1"
                 ,8332
                 ,"bitcoinrpc"
                 ,"DDKVyZDNros2cKvkk5KpGmJWGazzYMezoWTeKaXcqxEj"
                 ,bitcoindAccount
                 ,manager)
                     */
    , _category(category)
    //, _getBalanceReply(NULL)
    , _addedToSession(false)
    , _totalReceivedSinceStart(0)
    , _totalSentSinceStart(0)
    , _totalCurrentlyLockedInChannels(0) {
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

void Plugin::added(libtorrent::aux::session_impl * session) {

    qCDebug(_category) << "Plugin added to session.";

    _session = session;
    _addedToSession = true;
}

void Plugin::on_alert(libtorrent::alert const * a) {

    if(libtorrent::read_piece_alert const * p = libtorrent::alert_cast<libtorrent::read_piece_alert>(a)) {

        // Get info hash for torrent from which this read piece comes from
        const libtorrent::sha1_hash infoHash = p->handle.info_hash();

        Q_ASSERT(_sellerPlugins.contains(infoHash));

        // Send alert to plugin
        _sellerPlugins[infoHash]->pieceRead(p);

        qCDebug(_category) << "Read piece alert.";
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

bool Plugin::on_optimistic_unchoke(std::vector<libtorrent::policy::peer*> & peers) {
    return false;
}

void Plugin::save_state(libtorrent::entry & stateEntry) const {

}

void Plugin::load_state(libtorrent::lazy_entry const & stateEntry) {

}

Plugin::Status Plugin::status() const {
    return Plugin::Status(_totalReceivedSinceStart, _totalSentSinceStart, _totalCurrentlyLockedInChannels);
}

quint64 Plugin::registerReceivedFunds(quint64 value) {

    _totalReceivedSinceStart += value;
    return _totalReceivedSinceStart;
}

quint64 Plugin::registerSentFunds(quint64 value) {

    _totalSentSinceStart += value;
    return _totalSentSinceStart;
}

quint64 Plugin::registerLockedInChannelsFunds(quint64 value) {

    _totalCurrentlyLockedInChannels += value;
    return _totalCurrentlyLockedInChannels;
}

quint64 Plugin::registerUnLockedFromChannelFunds(quint64 value) {

    _totalCurrentlyLockedInChannels -= value;
    return _totalCurrentlyLockedInChannels;
}

void Plugin::removeTorrentPlugin(const libtorrent::sha1_hash & info_hash) {
    qCDebug(_category) << "Plugin::removeTorrentPlugin: NOT IMPLEMENTED!!";
}

void Plugin::sendAlertToSession(const libtorrent::alert & alert) {
    //_session->m_alerts.post_alert(alert);
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

        qCDebug(_category) << "Cannot process torrent plugin request.";

        // Removes the head request in the queue and returns it
        TorrentPluginRequest * torrentPluginRequest = _torrentPluginRequestQueue.dequeue();

        /**
        // find corresponding torrent plugin
        std::map<libtorrent::sha1_hash, TorrentPlugin *>::iterator i = _torrentPlugins.find(torrentPluginRequest->_info_hash);

        // process it if it exists
        if(i != _torrentPlugins.end())
            (i->second)->processTorrentPluginRequest(torrentPluginRequest);
        else
            qCDebug(_category) << "Discarded request for torrent plugin which does not exist.";
        */

        // delete
        delete torrentPluginRequest;

    }


    // Iterate _peerPluginRequestQueue
    while (!_peerPluginRequestQueue.empty()) {

        qCDebug(_category) << "Cannot process peer plugin request.";

        // Removes the head request in the queue and returns it
        PeerPluginRequest * peerPluginRequest = _peerPluginRequestQueue.dequeue();

        /**
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
        */

        // delete
        delete peerPluginRequest;
    }

}

void Plugin::processPluginRequest(const PluginRequest * pluginRequest) {

    qCDebug(_category) << "processPluginRequest";

    switch(pluginRequest->getPluginRequestType()) {

    /**
        case PluginRequestType::StartTorrentPlugin:  {

            qCDebug(_category) << "Starting torrent plugin";

            const StartTorrentPlugin * p = reinterpret_cast<const StartTorrentPlugin *>(pluginRequest);
            startTorrentPlugin(p->infoHash(), p->configuration());

            // Delete configuration
            // enable later
            //delete p->configuration();
        }

        break;
    */


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

        Q_ASSERT(false);

    }
}

/**
bool Plugin::startTorrentPlugin(const libtorrent::sha1_hash & infoHash, const TorrentPlugin::Configuration & configuration) {

    // Torrent should never have a plugin installed
    Q_ASSERT(!_plugins.contains(infoHash));

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

                qCDebug(_category) << "Observer not implemented yet.";
        }

        // Install plugin on torrent
        sharedTorrentPtr->add_extension(sharedPluginPtr);

        // Remember plugin
        _plugins[infoHash] = boost::dynamic_pointer_cast<TorrentPlugin>(sharedPluginPtr);

        // Return success indication
        return true;

    } else {

        qCDebug(_category) << "Torrent deleted, cannot install seller torrent pluin.";
        return false;
    }
}
*/

bool Plugin::startBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration & configuration, const UnspentP2PKHOutput & utxo) {

    // Check that torrent does not already have a plugin installed
    if(_sellerPlugins.contains(infoHash) || _buyerPlugins.contains(infoHash)) {
        qCDebug(_category) << "Torrent already has plugin installed, remove first.";
        Q_ASSERT(false);
        return false;
    }

    // Find torrent
    boost::weak_ptr<libtorrent::torrent> weakTorrentPtr = _session->find_torrent(infoHash);

    // Get shared torrent pointer
    if(boost::shared_ptr<libtorrent::torrent> sharedTorrentPtr = weakTorrentPtr.lock()) {

        // Create plugin with given configuration
        boost::shared_ptr<libtorrent::torrent_plugin> sharedPluginPtr(new BuyerTorrentPlugin(this, sharedTorrentPtr, _wallet, configuration, utxo, _category));

        // Install plugin on torrent
        sharedTorrentPtr->add_extension(sharedPluginPtr);

        // Remember plugin
        _buyerPlugins[infoHash] = boost::dynamic_pointer_cast<BuyerTorrentPlugin>(sharedPluginPtr); // sharedPluginPtr

        // Notify controller
        sendAlertToSession(StartedBuyerTorrentPlugin(infoHash, configuration, utxo));

        // Return success indication
        return true;

    } else {

        qCDebug(_category) << "Torrent deleted, cannot install buyer torrent pluin.";
        Q_ASSERT(false);
        return false;
    }
}

bool Plugin::startSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration) {

    // Check that torrent does not already have a plugin installed
    if(_sellerPlugins.contains(infoHash) || _buyerPlugins.contains(infoHash)) {

        qCDebug(_category) << "Torrent already has plugin installed, remove first.";
        Q_ASSERT(false);
        return false;
    }

    // Find torrent
    boost::weak_ptr<libtorrent::torrent> weakTorrentPtr = _session->find_torrent(infoHash);

    // Get shared torrent pointer
    if(boost::shared_ptr<libtorrent::torrent> sharedTorrentPtr = weakTorrentPtr.lock()) {

        // Create plugin with given configuration
        boost::shared_ptr<libtorrent::torrent_plugin> sharedPluginPtr(new SellerTorrentPlugin(this, sharedTorrentPtr, _wallet, configuration, _category));

        // Install plugin on torrent
        sharedTorrentPtr->add_extension(sharedPluginPtr);

        // Remember plugin
        _sellerPlugins[infoHash] = boost::dynamic_pointer_cast<SellerTorrentPlugin>(sharedPluginPtr); //sharedPluginPtr;

        // Notify controller
        sendAlertToSession(StartedSellerTorrentPlugin(infoHash, configuration));

        // Return success indication
        return true;

    } else {

        qCDebug(_category) << "Torrent deleted, cannot install seller torrent pluin.";
        Q_ASSERT(false);
        return false;
    }
}

/**
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
*/
