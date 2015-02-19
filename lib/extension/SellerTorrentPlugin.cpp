#include "SellerTorrentPlugin.hpp"
#include "PluginMode.hpp"
#include "SellerPeerPlugin.hpp"

#include <libtorrent/error_code.hpp>
#include <libtorrent/peer_connection.hpp>
#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/socket_io.hpp> // print_endpoint

#include <QLoggingCategory>

SellerTorrentPlugin::SellerTorrentPlugin(Plugin * plugin,
                                         const boost::weak_ptr<libtorrent::torrent> & torrent,
                                         const SellerTorrentPlugin::Configuration & configuration,
                                         QLoggingCategory & category)
    : TorrentPlugin(plugin, torrent, configuration, category)
    //, _configuration(configuration)
{
    // do something with this => configuration
}

boost::shared_ptr<libtorrent::peer_plugin> SellerTorrentPlugin::new_connection(libtorrent::peer_connection * peerConnection) {

    /**
     * Libtorrent docs (http://libtorrent.org/reference-Plugins.html#peer_plugin):
     * The peer_connection will be valid as long as the shared_ptr is being held by the
     * torrent object. So, it is generally a good idea to not keep a shared_ptr to
     * your own peer_plugin. If you want to keep references to it, use weak_ptr.
     */

    // Get end point to look up sets
    const libtorrent::tcp::endpoint & endPoint = peerConnection->remote();
    std::string endPointString = libtorrent::print_endpoint(endPoint);

    qCDebug(_category) << "New connection from" << endPointString.c_str();

    // Check if this peer should be accepted, if not
    // a null is returned, hence plugin is not installed
    if(!TorrentPlugin::isPeerWellBehaved(peerConnection)) {
        qCDebug(_category) << "Rejected connection from peer, peer plugin not installed.";
        return boost::shared_ptr<libtorrent::peer_plugin>();
    }

    // Create seller peer
    libtorrent::bt_peer_connection * bittorrentPeerConnection = static_cast<libtorrent::bt_peer_connection*>(peerConnection);

    // Create shared pointer to new seller peer plugin
    boost::shared_ptr<libtorrent::peer_plugin> sharedPeerPluginPtr(new SellerPeerPlugin(this, bittorrentPeerConnection, _category));

    // Create weak pointer to the same seller, and save in _peerPlugins map
    boost::weak_ptr<libtorrent::peer_plugin> weakPeerPluginPtr(sharedPeerPluginPtr);

    // Add to collection
    _peerPlugins[endPoint] = weakPeerPluginPtr;

    qCDebug(_category) << "Seller #" << _peerPlugins.size() << endPointString.c_str() << "added to " << _torrent->name().c_str();

    // Emit peer added signal
    // Should not be here, should be when a payment channel actually starts
    //emit peerAdded(peerPlugin->getPeerPluginId());

    // Return pointer to plugin as required
    return sharedPeerPluginPtr;
}

void SellerTorrentPlugin::on_piece_pass(int index) {

}

void SellerTorrentPlugin::on_piece_failed(int index) {

}

void SellerTorrentPlugin::tick() {

    qCDebug(_category) << "SellerTorrentPlugin.tick()";

    /*
    // No processing is done before a successful extended handshake
    if(_peerBitSwaprBEPSupportedStatus != BEPSupportStatus::supported)
        return;
    */

    // Call base tick routine
    TorrentPlugin::tick();

}


bool SellerTorrentPlugin::on_resume() {
    return false;
}

bool SellerTorrentPlugin::on_pause() {
    return false;
}

void SellerTorrentPlugin::on_files_checked() {

}

void SellerTorrentPlugin::on_state(int s) {

}

void SellerTorrentPlugin::on_add_peer(libtorrent::tcp::endpoint const & endPoint, int src, int flags) {

    std::string endPointString = libtorrent::print_endpoint(endPoint);

    qCDebug(_category) << "Peer list extended with peer" << endPointString.c_str() << ": " << endPoint.port();

    // Check if we know from before that peer does not have
    if(_peersWithoutExtension.find(endPoint) != _peersWithoutExtension.end()) {

        qCDebug(_category) << "Not connecting to peer" << endPointString.c_str() << "which is known to not have extension.";
        return;
    }

    // Check if peer is banned due to irregular behaviour
    if(_irregularPeer.find(endPoint) != _irregularPeer.end()) {

        qCDebug(_category) << "Not connecting to peer" << endPointString.c_str() << "which has been banned due to irregular behaviour.";
        return;
    }

    // Try to connect to peer
    // Who owns this? I am allocatig on heap because I think connect_to_peer() requires persistent object?
    // ask on mailinglist.
    /*
    libtorrent::policy::peer peerPolicy = new libtorrent::policy::peer();

    torrent_->connect_to_peer(peerPolicy,true);
    */
}

/*
void SellerTorrentPlugin::startSell() {

    // Iterate peer plugins and set their configuration
    for(std::map<libtorrent::tcp::endpoint, PeerPlugin *>::iterator i = _peerPlugins.begin(),
            end(_peerPlugins.end()); i != end;i++) {

        // Get peer plugin
        PeerPlugin * peerPlugin = i->second;

        // Start plugin
        // THIS IS WRONG, REMOVE
        // Convert to minimum refund lock time, w.r.t Coordinated Univesal Time, which is what nLockTime uses, i.e. POSIX time
        //QDateTime minLock = QDateTime(QDate::currentDate(), _torrentPluginConfiguration->_minLock, Qt::UTC);
        //peerPlugin->startPlugin(Sell(_torrentPluginConfiguration->_minPrice, minLock));

    }
}
*/

/*
void SellerTorrentPlugin::removePeerPlugin(PeerPlugin * plugin) {

    qCDebug(_category) << "TorrentPlugin::removePeerPlugin(): NOT IMPLEMENTED.";


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

}
*/

PluginMode SellerTorrentPlugin::pluginMode() const {
    return PluginMode::Seller;
}
