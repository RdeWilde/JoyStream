
#include "TorrentPlugin.hpp"
#include "BuyerPeerPlugin.hpp" // needed since we construct object
#include "SellerPeerPlugin.hpp" // needed since we construct object

#include <libtorrent/error_code.hpp>
#include <libtorrent/peer_connection.hpp>
#include <libtorrent/bt_peer_connection.hpp>

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
    // No need to explicltly delete PeerPlugin, since libtorrent has shared_ptr
}

boost::shared_ptr<libtorrent::peer_plugin> TorrentPlugin::new_connection(libtorrent::peer_connection * peerConnection) {

    /*
     * Libtorrent docs (http://libtorrent.org/reference-Plugins.html#peer_plugin):
     * The peer_connection will be valid as long as the shared_ptr is being held by the
     * torrent object. So, it is generally a good idea to not keep a shared_ptr to
     * your own peer_plugin. If you want to keep references to it, use weak_ptr.
     */

    // Check that this is indeed a bittorrent client, and not a HTTP or URL seed, if not, then dont isntall
    if(peerConnection->type() != libtorrent::peer_connection::bittorrent_connection) {

        qCDebug(category_) << "New peer connection without plugin, was not BitTorrent client. ";

        // Return without plugin
        return boost::shared_ptr<libtorrent::peer_plugin>();
    }

    libtorrent::bt_peer_connection * bittorrentPeerConnection = static_cast<libtorrent::bt_peer_connection*>(peerConnection);

    // Create new peer, buyer or seller, depending
    PeerPlugin * peerPlugin;
    if(torrent_->bytes_left() > 0) {
        peerPlugin = new BuyerPeerPlugin(this, bittorrentPeerConnection, category_);
    } else {
        peerPlugin = new SellerPeerPlugin(this, bittorrentPeerConnection, category_);
    }

    // Add to collection
    peerPlugins.push_back(peerPlugin);

    // Notify
    const boost::asio::ip::tcp::endpoint & endPoint = bittorrentPeerConnection->remote();

    libtorrent::error_code ec;
    std::string addrString = endPoint.address().to_string(ec);

    if(ec) {
        std::string m = ec.message();
        qCCritical(category_) << m.c_str();

        // Return without plugin
        return boost::shared_ptr<libtorrent::peer_plugin>();
    }

    qCDebug(category_) << "Peer #" << peerPlugins.size() << "[" << addrString.c_str() << ":" << endPoint.port() << "] added to " << this->torrent_->name().c_str();

    //(libtorrent::to_hex(torrent_->info_hash().to_string())).c_str() << ".";

    // Return pointer as required
    return boost::shared_ptr<libtorrent::peer_plugin>(peerPlugin);
}

void TorrentPlugin::on_piece_pass(int index) {

}

void TorrentPlugin::on_piece_failed(int index) {

}

void TorrentPlugin::tick() {

    //qCDebug(category_) << "Peer.tick()";

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

void TorrentPlugin::on_add_peer(libtorrent::tcp::endpoint const & tcpEndPoint, int src, int flags) {
    qCDebug(category_) << "Peer added to list " << tcpEndPoint.address().to_string().c_str() << ": " << tcpEndPoint.port();
}

// Returns plugin
Plugin * TorrentPlugin::getPlugin() {
    return plugin_;
}

libtorrent::torrent * TorrentPlugin::getTorrent() {
    return torrent_;
}

const libtorrent::sha1_hash & TorrentPlugin::getInfoHash() const {
    return torrent_->info_hash();
}

void TorrentPlugin::sendTorrentPluginStatusSignal() {

    int numberOfPeers = peerPlugins.size();

    int numberOfPeersWithExtension = 0;
    for(std::vector<PeerPlugin *>::iterator i = peerPlugins.begin(),
        end(peerPlugins.end()); i != end; i++) {

        if((*i)->getPeerBEP43SupportedStatus() == PeerPlugin::supported)
            numberOfPeersWithExtension++;
    }

    // Emit status
    libtorrent::sha1_hash hash = torrent_->info_hash();
    TorrentPluginStatus status(hash, numberOfPeers, numberOfPeersWithExtension, pluginOn_, tokensReceived_, tokensSent_);

    emit updateTorrentPluginStatus(status);
}
