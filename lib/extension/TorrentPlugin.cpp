
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
    , pluginOn_(pluginOn) {
}

TorrentPlugin::~TorrentPlugin() {
    // Lets log, so we understand when libtorrent disposes of shared pointer
    qCDebug(category_) << "~TorrentPlugin() called.";
}

bool TorrentPlugin::installPluginOnNewConnection(libtorrent::peer_connection * peerConnection) {

    // Get endpoint of connection
    const libtorrent::tcp::endpoint & endPoint = peerConnection->remote();

    // If we are using banning sets, then check this peer
    if(torrentPluginParameters_._enableBanningSets) {

        // Check if we know from before that peer does not have
        if(peersWithoutExtension_.find(endPoint) != peersWithoutExtension_.end()) {
            qCDebug(category_) << "Peer is known to not have extension.";
            return false;
        }

        // Check if peer is banned due to irregular behaviour
        if(irregularPeer_.find(endPoint) != irregularPeer_.end()) {
            qCDebug(category_) << "Peer has been banned due to irregular behaviour.";
            return false;
        }
    }

    // Check that this is indeed a bittorrent client, and not a HTTP or URL seed, if not, then dont isntall
    if(peerConnection->type() != libtorrent::peer_connection::bittorrent_connection) {
        qCDebug(category_) << "Peer was not BitTorrent client.";
        return false;
    }

    // Otherwise install plugin
    return true;
}

void TorrentPlugin::on_piece_pass(int index) {

}

void TorrentPlugin::on_piece_failed(int index) {

}

void TorrentPlugin::tick() {

    //qCDebug(category_) << "TorrentPlugin.tick()";

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

        // Count as supporting plugin if extended handshake was successful
        if((i->second)->getPeerBEP43SupportedStatus() == PeerPlugin::supported)
            numberOfPeersWithExtension++;
    }

    // Emit status signal
    libtorrent::sha1_hash info_hash = torrent_->info_hash();
    TorrentPluginStatus status(info_hash, numberOfPeers, numberOfPeersWithExtension, pluginOn_, tokensReceived_, tokensSent_);

    //emit torrentPluginStatusUpdated(status);
}
