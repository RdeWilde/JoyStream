#include "BuyerTorrentPlugin.hpp"
#include "BuyerPeerPlugin.hpp"

#include <libtorrent/error_code.hpp>
#include <libtorrent/peer_connection.hpp>
#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/socket_io.hpp> // print_endpoint

// Maximum allowable a peer may have in responding to given message (ms)
#define SIGN_REFUND_MAX_DELAY 5*1000

BuyerTorrentPlugin::BuyerTorrentPlugin(Plugin * plugin, libtorrent::torrent * torrent, QLoggingCategory & category, bool pluginOn, const BuyerTorrentPluginConfiguration * buyerTorrentPluginConfiguration)
    : TorrentPlugin(plugin, torrent, category, pluginOn, torrentPluginParameters)
    , _buyerTorrentPluginConfiguration(*buyerTorrentPluginConfiguration) {

}

boost::shared_ptr<libtorrent::peer_plugin> BuyerTorrentPlugin::new_connection(libtorrent::peer_connection * peerConnection) {

    /**
     * Libtorrent docs (http://libtorrent.org/reference-Plugins.html#peer_plugin):
     * The peer_connection will be valid as long as the shared_ptr is being held by the
     * torrent object. So, it is generally a good idea to not keep a shared_ptr to
     * your own peer_plugin. If you want to keep references to it, use weak_ptr.
     */

    // Get end point to look up sets
    const libtorrent::tcp::endpoint & endPoint = peerConnection->remote();
    std::string endPointString = libtorrent::print_endpoint(endPoint);

    qCDebug(category_) << "New connection from" << endPointString.c_str();

    // Check if this peer should be accepted, if not
    // a null is returned, hence plugin is not installed
    if(!TorrentPlugin::isPeerWellBehaved(peerConnection)) {
        qCDebug(category_) << "Rejected connection from peer, peer plugin not installed.";
        return boost::shared_ptr<libtorrent::peer_plugin>();
    }

    // Create seller peer
    libtorrent::bt_peer_connection * bittorrentPeerConnection = static_cast<libtorrent::bt_peer_connection*>(peerConnection);
    PeerPlugin * peerPlugin = new BuyerPeerPlugin(this, bittorrentPeerConnection, category_);

    // Add to collection
    peerPlugins_.insert(std::make_pair(endPoint, peerPlugin));

    /*
    // Connect peer plugin signal to controller slot
    Controller * controller = plugin_->getController();

    QObject::connect(peerPlugin,
                     SIGNAL(peerPluginStatusUpdated(const PeerPluginStatus&)),
                     controller,
                     SLOT(updatePeerPluginStatus(const PeerPluginStatus&)));
    */

    qCDebug(category_) << "Buyer #" << peerPlugins_.size() << endPointString.c_str() << "added to " << this->torrent_->name().c_str();

    // Emit peer added signal
    // Should not be here, should be when a payment channel actually starts
    //emit peerAdded(peerPlugin->getPeerPluginId());

    // Return pointer to plugin as required
    return boost::shared_ptr<libtorrent::peer_plugin>(peerPlugin);
}

void BuyerTorrentPlugin::tick() {

    qCDebug(category_) << "BuyerTorrentPlugin.tick()";

    // No processing is done before a successful extended handshake
    if(_peerBitSwaprBEPSupportedStatus != BEPSupportStatus::supported)
        return;

    // Call base tick routine
    TorrentPlugin::tick();
}



bool BuyerTorrentPlugin::on_resume() {
    return false;
}

bool BuyerTorrentPlugin::on_pause() {
    return false;
}

void BuyerTorrentPlugin::on_files_checked() {

}

void BuyerTorrentPlugin::on_state(int s) {

}

void BuyerTorrentPlugin::on_add_peer() {

}

void BuyerTorrentPlugin::startBuyer() {


    // Iterate peer plugins and set their configuration
    for(std::map<libtorrent::tcp::endpoint, PeerPlugin *>::iterator i = _peerPlugins.begin(),
            end(_peerPlugins.end()); i != end;i++) {

        // Get peer plugin
        PeerPlugin * peerPlugin = i->second;

        // Start plugin
        /**

        // THIS IS WRONG, REMOVE

        // Convert to maximum refund lock time, w.r.t Coordinated Univesal Time, which is what nLockTime uses, i.e. POSIX time
        QDateTime maxLock = QDateTime(QDate::currentDate(), _torrentPluginConfiguration->_maxLock, Qt::UTC);

        peerPlugin->startPlugin(Buy(_torrentPluginConfiguration->_maxPrice, maxLock));
        */
    }

    // Start clock for when picking sellers can begin
    _timeSincePluginStarted.start();

    // Setup space for plugins in contract
    _sellersInContract.fill(NULL, _torrentPluginConfiguration->_numSellers);
}

void BuyerTorrentPlugin::removePeerPlugin(PeerPlugin * plugin) {

    qCDebug(_category) << "TorrentPlugin::removePeerPlugin(): NOT IMPLEMENTED.";

    /*
     * SHOULD DEPEND ON MODE, AND ON SUB MODE STATE

    // Find iterator reference to plugin
    std::map<libtorrent::tcp::endpoint, PeerPlugin *>::iterator & mapIterator = _peerPlugins.find(plugin->getEndPoint());

    // Did we find match?
    if(mapIterator == _peerPlugins.end()) {
        qCDebug(_category) << "Could not find peer for removal.";
        return;
    }

    // Remove
    _peerPlugins.erase(mapIterator);

    // Delete object: Do we do this, or does libtorrent? and when is this safe?
    //delete mapIterator->second;

    // Emit peer added signal
    //emit peerRemoved(torrent_->info_hash(), mapIterator->first);
    */
}

const TorrentPluginConfiguration & BuyerTorrentPlugin::getTorrentPluginConfiguration() {
    return _buyerTorrentPluginConfiguration;
}


BuyerTorrentPluginState BuyerTorrentPlugin::state() const {
    return _state;
}
