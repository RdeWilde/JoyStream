#include "SellerTorrentPlugin.hpp"
#include "SellerPeerPlugin.hpp"

#include <libtorrent/error_code.hpp>
#include <libtorrent/peer_connection.hpp>
#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/socket_io.hpp> // print_endpoint

#include <QLoggingCategory>

SellerTorrentPlugin::SellerTorrentPlugin(Plugin * plugin, libtorrent::torrent * torrent, QLoggingCategory & category, bool pluginOn, const TorrentPluginConfiguration & torrentPluginParameters, const SellerTorrentPluginConfiguration * sellerTorrentPluginParameters)
    : TorrentPlugin(plugin, torrent, category, pluginOn, torrentPluginParameters)
    , _sellerTorrentPluginParameters(*sellerTorrentPluginParameters)
{ }

const TorrentPluginConfiguration & SellerTorrentPlugin::getTorrentPluginConfiguration() {
    return _sellerTorrentPluginParameters;
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

    qCDebug(category_) << "New connection from" << endPointString.c_str();

    // Check if this peer should be accepted, if not
    // a null is returned, hence plugin is not installed
    if(!TorrentPlugin::installPluginOnNewConnection(peerConnection)) {
        qCDebug(category_) << "Rejected connection from peer, peer plugin not installed.";
        return boost::shared_ptr<libtorrent::peer_plugin>();
    }

    // Create seller peer
    libtorrent::bt_peer_connection * bittorrentPeerConnection = static_cast<libtorrent::bt_peer_connection*>(peerConnection);
    PeerPlugin * peerPlugin = new SellerPeerPlugin(this, bittorrentPeerConnection, category_);

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

    qCDebug(category_) << "Seller #" << peerPlugins_.size() << endPointString.c_str() << "added to " << this->torrent_->name().c_str();

    // Emit peer added signal
    // Should not be here, should be when a payment channel actually starts
    //emit peerAdded(peerPlugin->getPeerPluginId());

    // Return pointer to plugin as required
    return boost::shared_ptr<libtorrent::peer_plugin>(peerPlugin);
}

void SellerTorrentPlugin::tick() {

    qCDebug(category_) << "SellerTorrentPlugin.tick()";

    // Call base tick routine
    TorrentPlugin::tick();

}
