
#include "TorrentPlugin.hpp"
#include "BuyerPeerPlugin.hpp" // needed since we construct object
#include "SellerPeerPlugin.hpp" // needed since we construct object
#include "PeerPluginStatus.hpp" // signal parameter
#include "controller/Controller.hpp" // needed to connect

#include <libtorrent/error_code.hpp>
#include <libtorrent/peer_connection.hpp>
#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/socket_io.hpp> // print_endpoint

#include <QLoggingCategory>

TorrentPlugin::TorrentPlugin(Plugin * plugin, libtorrent::torrent * torrent, QLoggingCategory & category, bool pluginOn)
    : plugin_(plugin)
    , torrent_(torrent)
    , category_(category)
    , tokensReceived_(0)
    , tokensSent_(0)
    , pluginOn_(pluginOn)
    , enableBanningSets(true) {
}

TorrentPlugin::~TorrentPlugin() {
    // Lets log, so we understand when libtorrent disposes of shared pointer
    qCDebug(category_) << "~TorrentPlugin() called.";
}

boost::shared_ptr<libtorrent::peer_plugin> TorrentPlugin::new_connection(libtorrent::peer_connection * peerConnection) {

    /**
     * Libtorrent docs (http://libtorrent.org/reference-Plugins.html#peer_plugin):
     * The peer_connection will be valid as long as the shared_ptr is being held by the
     * torrent object. So, it is generally a good idea to not keep a shared_ptr to
     * your own peer_plugin. If you want to keep references to it, use weak_ptr.
     */

    // Get end point to look up sets
    const libtorrent::tcp::endpoint & endPoint = peerConnection->remote();
    std::string endPointString = libtorrent::print_endpoint(endPoint);

    // If we are using banning sets, then check this peer
    if(enableBanningSets) {

        // Check if we know from before that peer does not have
        if(peersWithoutExtension_.find(endPoint) != peersWithoutExtension_.end()) {

            qCDebug(category_) << "Rejected connection from peer" << endPointString.c_str() << "which is known to not have extension.";
            return boost::shared_ptr<libtorrent::peer_plugin>(); // Return without plugin, hence plugin is not installed
        }

        // Check if peer is banned due to irregular behaviour
        if(irregularPeer_.find(endPoint) != irregularPeer_.end()) {

            qCDebug(category_) << "Rejected connection from peer" << endPointString.c_str() << "which has been banned due to irregular behaviour.";
            return boost::shared_ptr<libtorrent::peer_plugin>(); // Return without plugin, hence plugin is not installed
        }
    }

    // Check that this is indeed a bittorrent client, and not a HTTP or URL seed, if not, then dont isntall
    if(peerConnection->type() != libtorrent::peer_connection::bittorrent_connection) {

        qCDebug(category_) << "New peer" << endPointString.c_str() << "connection without plugin, was not BitTorrent client.";
        return boost::shared_ptr<libtorrent::peer_plugin>(); // Return without plugin, hence plugin is not installed
    }

    // Create new peer, buyer or seller, depending
    PeerPlugin * peerPlugin;
    libtorrent::bt_peer_connection * bittorrentPeerConnection = static_cast<libtorrent::bt_peer_connection*>(peerConnection);

    if(torrent_->bytes_left() > 0) {
        peerPlugin = new BuyerPeerPlugin(this, bittorrentPeerConnection, category_);
    } else {
        peerPlugin = new SellerPeerPlugin(this, bittorrentPeerConnection, category_);
    }

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

    qCDebug(category_) << "#" << peerPlugins_.size() << endPointString.c_str() << "added to " << this->torrent_->name().c_str();

    // Emit peer added signal
    // Should not be here, should be when a payment channel actually starts
    //emit peerAdded(peerPlugin->getPeerPluginId());

    // Return pointer to plugin as required
    return boost::shared_ptr<libtorrent::peer_plugin>(peerPlugin);
}

void TorrentPlugin::on_piece_pass(int index) {

}

void TorrentPlugin::on_piece_failed(int index) {

}

void TorrentPlugin::tick() {

    qCDebug(category_) << "TorrentPlugin.tick()";

    // Send status signal
    sendTorrentPluginStatusSignal();
}

bool TorrentPlugin::on_resume() {
    return false;
}

bool TorrentPlugin::on_pause() {
    return false;
}

void TorrentPlugin::on_files_checked() {

}

void TorrentPlugin::on_state(int s) {

}

void TorrentPlugin::on_add_peer(libtorrent::tcp::endpoint const & endPoint, int src, int flags) {

    std::string endPointString = libtorrent::print_endpoint(endPoint);

    qCDebug(category_) << "Peer list extended with peer" << endPointString.c_str() << ": " << endPoint.port();

    // Check if we know from before that peer does not have
    if(peersWithoutExtension_.find(endPoint) != peersWithoutExtension_.end()) {

        qCDebug(category_) << "Not connecting to peer" << endPointString.c_str() << "which is known to not have extension.";
        return;
    }

    // Check if peer is banned due to irregular behaviour
    if(irregularPeer_.find(endPoint) != irregularPeer_.end()) {

        qCDebug(category_) << "Not connecting to peer" << endPointString.c_str() << "which has been banned due to irregular behaviour.";
        return;
    }

    // Try to connect to peer
    // Who owns this? I am allocatig on heap because I think connect_to_peer() requires persistent object?
    // ask on mailinglist.
    /*
    libtorrent::policy::peer peerPolicy = new libtorrent::policy::peer();

    torrent_->co

            connect_to_peer(peerPolicy,true);
    */
}

bool TorrentPlugin::addToPeersWithoutExtensionSet(const libtorrent::tcp::endpoint & endPoint) {

    // Attempt to insert
    std::pair<std::set<libtorrent::tcp::endpoint>::iterator, bool> insertResult = peersWithoutExtension_.insert(endPoint);

    // Return whether object was actually inserted
    return insertResult.second;
}

bool TorrentPlugin::addToIrregularPeersSet(const libtorrent::tcp::endpoint & endPoint) {

    // Attempt to insert
    std::pair<std::set<libtorrent::tcp::endpoint>::iterator, bool> insertResult = irregularPeer_.insert(endPoint);

    // Return whether object was actually inserted
    return insertResult.second;
}

void TorrentPlugin::removePeerPlugin(PeerPlugin * plugin) {

    // Find iterator reference to plugin
    std::map<libtorrent::tcp::endpoint, PeerPlugin *>::iterator & mapIterator = peerPlugins_.find(plugin->getEndPoint());

    // Did we find match?
    if(mapIterator == peerPlugins_.end()) {
        qCDebug(category_) << "Could not find peer for removal.";
        return;
    }

    // Remove
    peerPlugins_.erase(mapIterator);

    // Delete object: Do we do this, or does libtorrent? and when is this safe?
    //delete mapIterator->second;

    // Emit peer added signal
    //emit peerRemoved(torrent_->info_hash(), mapIterator->first);
}

/*
Plugin * TorrentPlugin::getPlugin() {

    // Returns plugin
    return plugin_;
}

libtorrent::torrent * TorrentPlugin::getTorrent() {
    return torrent_;
}
*/

const libtorrent::sha1_hash & TorrentPlugin::getInfoHash() const {
    return torrent_->info_hash();
}

void TorrentPlugin::sendTorrentPluginStatusSignal() {

    int numberOfPeers = peerPlugins_.size();

    int numberOfPeersWithExtension = 0;
    for(std::map<libtorrent::tcp::endpoint, PeerPlugin *>::iterator i = peerPlugins_.begin(),
        end(peerPlugins_.end()); i != end; i++) {

        if((i->second)->getPeerBEP43SupportedStatus() == PeerPlugin::supported)
            numberOfPeersWithExtension++;
    }

    // Emit status signal
    libtorrent::sha1_hash info_hash = torrent_->info_hash();
    TorrentPluginStatus status(info_hash, numberOfPeers, numberOfPeersWithExtension, pluginOn_, tokensReceived_, tokensSent_);

    //emit torrentPluginStatusUpdated(status);
}
