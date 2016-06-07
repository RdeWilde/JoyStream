/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/TorrentPlugin.hpp>
#include <extension/Plugin.hpp>
#include <extension/alert/TorrentPluginAlert.hpp>

#include <libtorrent/error_code.hpp>
#include <libtorrent/peer_connection.hpp>
#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/socket_io.hpp> // print_endpoint

namespace joystream {
namespace extension {

TorrentPlugin::TorrentPlugin(Plugin * plugin,
                             const boost::shared_ptr<libtorrent::torrent> & torrent,
                             const std::string & bep10ClientIdentifier,
                             const TorrentPluginConfiguration & configuration,
                             QLoggingCategory & category)
    : _plugin(plugin)
    , _torrent(torrent)
    , _bep10ClientIdentifier(bep10ClientIdentifier)
    , _enableBanningSets(configuration.enableBanningSets())
    , _category(category) {
}

TorrentPlugin::~TorrentPlugin() {

    // Lets log, so we understand when libtorrent disposes of shared pointer
    qCDebug(_category) << "~TorrentPlugin() called.";
}

boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection * connection) {

    /**
     * Libtorrent docs (http://libtorrent.org/reference-Plugins.html#peer_plugin):
     * The peer_connection will be valid as long as the shared_ptr is being held by the
     * torrent object. So, it is generally a good idea to not keep a shared_ptr to
     * your own peer_plugin. If you want to keep references to it, use weak_ptr.
     */

    /**
     * DISCONNECTING PEERS:
     * You cannot disconnect this peer here, e.g. by using peer_connection::disconnect().
     * This is because, at this point (new_connection), the connection has not been
     * added to a torrent level peer list, and the disconnection asserts that the peer has
     * to be in this list. Disconnects must be done later.
     */

    // Get end point
    const libtorrent::tcp::endpoint & endPoint = peerConnection->remote();

    // Print notification
    std::string endPointString = libtorrent::print_endpoint(endPoint);

    qCDebug(_category) << "New" << (peerConnection->is_outgoing() ? "outgoing" : "incoming") << "connection with" << endPointString.c_str(); // << "on " << _torrent->name().c_str();

    // Create bittorrent peer connection
    Q_ASSERT(peerConnection->type() == libtorrent::peer_connection::bittorrent_connection);
    libtorrent::bt_peer_connection * bittorrentPeerConnection = static_cast<libtorrent::bt_peer_connection*>(peerConnection);

    // Create shared pointer to new seller peer plugin
    SellerPeerPlugin * peerPlugin = NULL;

    // Check if this peer should be accepted, if not
    // a null is returned, hence plugin is not installed
    if(!TorrentPlugin::isPeerWellBehaved(peerConnection)) {

        qCDebug(_category) << "Peer is on ban list, scheduling peer for disconnection and plugin for deletion.";
        peerPlugin = createSellerPeerPluginScheduledForDeletion(bittorrentPeerConnection);

    } else if(_peers.contains(endPoint)) {

        qCDebug(_category) << "Already added peer, scheduling peer for disconnection and plugin for deletion.";
        peerPlugin = createSellerPeerPluginScheduledForDeletion(bittorrentPeerConnection);

    } else {

        qCDebug(_category) << "Installed seller plugin #" << _peers.size() << endPointString.c_str();

        // Create a new peer plugin
        std::vector<Coin::PrivateKey> payeeContractKeySet = _wallet->getKeys(1, false);
        std::vector<Coin::PrivateKey> paymentKeySet = _wallet->getKeys(1, true);

        Q_ASSERT(payeeContractKeySet.size() == 1);
        Q_ASSERT(paymentKeySet.size() == 1);

        peerPlugin = createRegularSellerPeerPlugin(payeeContractKeySet[0],
                                                   paymentKeySet[0],
                                                   bittorrentPeerConnection);

        // Alert that peer was added
        sendTorrentPluginAlert(SellerPeerAddedAlert(_torrent->info_hash(),
                                                   endPoint,
                                                   peerPlugin->status()));
    }

    Q_ASSERT(peerPlugin != NULL);

    // Create shared pointer
    boost::shared_ptr<SellerPeerPlugin> sharedPeerPluginPtr(peerPlugin);

    // Add to collection
    if(peerPlugin->scheduledForDeletingInNextTorrentPluginTick())
        _peersScheduledForDeletion.append(sharedPeerPluginPtr);
    else
        _peers[endPoint] = boost::weak_ptr<SellerPeerPlugin>(sharedPeerPluginPtr);

    // Return pointer to plugin as required
    return sharedPeerPluginPtr;
}

void TorrentPlugin::on_piece_pass(int index) {

}

void TorrentPlugin::on_piece_failed(int index) {

}

void TorrentPlugin::tick() {

}

bool TorrentPlugin::on_resume() {

}

bool TorrentPlugin::on_pause() {

}

void TorrentPlugin::on_files_checked() {

}

void TorrentPlugin::on_state(int s) {

}

void TorrentPlugin::on_add_peer(const libtorrent::tcp::endpoint & endPoint, int src, int flags) {

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

void TorrentPlugin::readPiece(SellerPeerPlugin * peer, int piece) {

    // There should never be queued multiple reads by same peer of same piece
    Q_ASSERT(!_outstandingPieceRequests[piece].contains(peer));

    // Register read piece request if it has not already been requested
    if(_outstandingPieceRequests[piece].empty()) {

        qCDebug(_category) << "[" << _outstandingPieceRequests[piece].size() <<"]Requested piece" << piece << "by" << libtorrent::print_address(peer->endPoint().address()).c_str();
        _torrent->read_piece(piece);
    } else
        qCDebug(_category) << "[" << _outstandingPieceRequests[piece].size() <<"]Skipping requested piece" << piece << "by" << libtorrent::print_address(peer->endPoint().address()).c_str();

    // Register this peer as a subscriber to a piece read request of this piece
    _outstandingPieceRequests[piece].insert(peer);
}

void TorrentPlugin::pieceRead(const libtorrent::read_piece_alert * alert) {

    // There should be at least one peer registered for this piece
    //Q_ASSERT(!_outstandingPieceRequests[alert->piece].empty());
    if(_outstandingPieceRequests[alert->piece].empty()) {

        qCDebug(_category) << "Ignoring piece read, must be for streaming server.";
        return;
    }

    // Make a callback for each peer registered
    const QSet<SellerPeerPlugin *> & peers = _outstandingPieceRequests[alert->piece];

    // Iterate peers
    for(QSet<SellerPeerPlugin *>::const_iterator i = peers.constBegin(),
        end(peers.constEnd()); i != end;i++) {

        // Get peer pointer
        SellerPeerPlugin * peerPlugin = *i;

        // Notify peer plugin of result
        if(alert->ec) {

            qCDebug(_category) << "Failed reading piece" << alert->piece << "for" << libtorrent::print_address(peerPlugin->endPoint().address()).c_str();

            (*i)->pieceReadFailed(alert->piece);
        } else {

            qCDebug(_category) << "Read piece" << alert->piece << "for" << libtorrent::print_address(peerPlugin->endPoint().address()).c_str();
            (*i)->pieceRead(alert->piece, alert->buffer, alert->size);
        }
    }

    // Remove all peers registered for this piece
    _outstandingPieceRequests.remove(alert->piece);
}

void TorrentPlugin::addToPeersWithoutExtensionSet(const libtorrent::tcp::endpoint & endPoint) {
    _TorrentPlugin::peersWithoutExtension.insert(endPoint);
}

void TorrentPlugin::addToIrregularPeersSet(const libtorrent::tcp::endpoint & endPoint) {
    _irregularPeer.insert(endPoint);
}

boost::shared_ptr<libtorrent::torrent> TorrentPlugin::torrent() const {
    return _torrent;
}

void TorrentPlugin::setTorrent(const boost::shared_ptr<libtorrent::torrent> &torrent) {
    _torrent = torrent;
}

bool TorrentPlugin::isPeerWellBehaved(libtorrent::peer_connection * connection) const {

    // Get endpoint of connection
    const libtorrent::tcp::endpoint & endPoint = connection->remote();

    // If we are using banning sets, then check this peer
    if(_enableBanningSets) {

        // Check if we know from before that peer does not have
        if(_peersWithoutExtension.contains(endPoint)) {
            qCDebug(_category) << "Peer is known to not have extension.";
            return false;
        }

        // Check if peer is banned due to irregular behaviour
        if(_irregularPeer.contains(endPoint)) {
            qCDebug(_category) << "Peer has been banned due to irregular behaviour.";
            return false;
        }
    }

    // Check that this is indeed a bittorrent client, and not a HTTP or URL seed, if not, then dont isntall
    if(connection->type() != libtorrent::peer_connection::bittorrent_connection) {
        qCDebug(_category) << "Peer was not BitTorrent client.";
        return false;
    }

    // Is well behaved
    return true;
}

void TorrentPlugin::sendTorrentPluginAlert(const alert::TorrentPluginAlert & alert) {
    _torrent->alerts().post_alert(alert);
    //_torrent->alerts().emplace_alert(alert);
}

bool TorrentPlugin::enableBanningSets() const
{
    return _enableBanningSets;
}

void TorrentPlugin::setEnableBanningSets(bool enableBanningSets)
{
    _enableBanningSets = enableBanningSets;
}

}
}
