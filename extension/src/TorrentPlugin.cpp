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
                             const Policy & policy)
    : _plugin(plugin)
    , _torrent(torrent)
    , _bep10ClientIdentifier(bep10ClientIdentifier)
    , _policy(policy) {
}

TorrentPlugin::~TorrentPlugin() {

    // Lets log, so we understand when libtorrent disposes of shared pointer
    std::clog << "~TorrentPlugin() called.";
}

boost::shared_ptr<libtorrent::peer_plugin> TorrentPlugin::new_connection(libtorrent::peer_connection * connection) {

    /**
     * NB:
     * You cannot disconnect this peer here, e.g. by using peer_connection::disconnect().
     * This is because, at this point (new_connection), the connection has not been
     * added to a torrent level peer list, and the disconnection asserts that the peer has
     * to be in this list. Disconnects must be done later.
     */

    // Get end point
    libtorrent::tcp::endpoint endPoint = connection->remote();

    std::clog << "New"
              << (connection->is_outgoing() ? "outgoing" : "incoming")
              << "connection with"
              << libtorrent::print_endpoint(endPoint).c_str()
              << std::endl; // << "on " << _torrent->name().c_str();

    bool accept = false;

    if(connection->type() != libtorrent::peer_connection::bittorrent_connection)
        std::clog << "Peer was not BitTorrent client, likely web seed." << std::endl;
    else if(_peers.count(endPoint))
        std::clog << "Peer already added." << std::endl;
    else if(_extensionless.count(endPoint) && _policy.banPeersWithoutExtension)
        std::clog << "Peer is known to not have extension, which is incompatible with policy." << std::endl;
    else if(_sentMalformedExtendedMessage.count(endPoint) && _policy.banPeersWithPastMalformedExtendedMessage)
        std::clog << "Peer has previously sent malformed extended message." << std::endl;
    else
        accept = true;

    // If connection should not be accepted,
    if(false) {

        std::clog << "Instant disconnect, without installing peer plugin." << std::endl;

        // then disconnect
        libtorrent::error_code ec; // <-- set this to something?
        connection->disconnect(ec);

        // and don't install plugin
        return boost::shared_ptr<PeerPlugin>(nullptr);
    }

    std::clog << "Installed seller plugin #" << _peers.size() << std::endl;

    // Create a new peer plugin
    PeerPlugin * peerPlugin = new PeerPlugin(this,
                                             static_cast<libtorrent::bt_peer_connection*>(connection),
                                             _policy.peerPolicy,
                                             _bep10ClientIdentifier);

    /**
    // Alert that peer was added
    sendTorrentPluginAlert(SellerPeerAddedAlert(_torrent->info_hash(),
                                               endPoint,
                                               peerPlugin->status()));
    */

    // Create shared pointer
    boost::shared_ptr<PeerPlugin> shared(peerPlugin);

    // Add to collection
    _peers[endPoint] = boost::weak_ptr<PeerPlugin>(shared);

    /**
    // Add to collection
    if(peerPlugin->scheduledForDeletingInNextTorrentPluginTick())
        _peersScheduledForDeletion.append(sharedPeerPluginPtr);
    else
        _peers[endPoint] = boost::weak_ptr<SellerPeerPlugin>(sharedPeerPluginPtr);
    */

    // Return pointer to plugin as required
    return shared;
}

void TorrentPlugin::on_piece_pass(int) {

    if(_session.mode() == protocol_session::SessionMode::buying) {
        // tell session about this? but how to figure out
        //_session.validPieceReceivedOnConnection(index??);
    }
}

void TorrentPlugin::on_piece_failed(int) {

}

void TorrentPlugin::tick() {

    // Send status update to controller
    //sendTorrentPluginAlert(this->status());
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

    std::clog << "Peer list extended with peer" << endPointString.c_str() << ": " << endPoint.port();

    /**
    // Check if we know from before that peer does not have
    if(_withoutExtension.find(endPoint) != _withoutExtension.end()) {

        std::clog << "Not connecting to peer" << endPointString.c_str() << "which is known to not have extension.";
        return;
    }

    // Check if peer is banned due to irregular behaviour
    if(_irregularPeer.find(endPoint) != _irregularPeer.end()) {

        std::clog << "Not connecting to peer" << endPointString.c_str() << "which has been banned due to irregular behaviour.";
        return;
    }

    // Try to connect to peer
    // Who owns this? I am allocatig on heap because I think connect_to_peer() requires persistent object?
    // ask on mailinglist.

    //libtorrent::policy::peer peerPolicy = new libtorrent::policy::peer();

    //torrent_->connect_to_peer(peerPolicy,true);
    */
}

void TorrentPlugin::readPiece(PeerPlugin * peer, int piece) {

    /**
    // There should never be queued multiple reads by same peer of same piece
    assert(!_outstandingPieceRequests[piece].contains(peer));

    // Register read piece request if it has not already been requested
    if(_outstandingPieceRequests[piece].empty()) {

        std::clog << "[" << _outstandingPieceRequests[piece].size() <<"]Requested piece" << piece << "by" << libtorrent::print_address(peer->endPoint().address()).c_str();
        _torrent->read_piece(piece);
    } else
        std::clog << "[" << _outstandingPieceRequests[piece].size() <<"]Skipping requested piece" << piece << "by" << libtorrent::print_address(peer->endPoint().address()).c_str();

    // Register this peer as a subscriber to a piece read request of this piece
    _outstandingPieceRequests[piece].insert(peer);
    */
}

void TorrentPlugin::pieceRead(const libtorrent::read_piece_alert * alert) {

    /**
    // There should be at least one peer registered for this piece
    //Q_ASSERT(!_outstandingPieceRequests[alert->piece].empty());
    if(_outstandingPieceRequests[alert->piece].empty()) {

        std::clog << "Ignoring piece read, must be for streaming server.";
        return;
    }

    // Make a callback for each peer registered
    const std::set<libtorrent::tcp::endpoint> & peers = _outstandingPieceRequests[alert->piece];

    // Iterate peers
    for(QSet<SellerPeerPlugin *>::const_iterator i = peers.constBegin(),
        end(peers.constEnd()); i != end;i++) {

        // Get peer pointer
        SellerPeerPlugin * peerPlugin = *i;

        // Notify peer plugin of result
        if(alert->ec) {

            std::clog << "Failed reading piece" << alert->piece << "for" << libtorrent::print_address(peerPlugin->endPoint().address()).c_str();

            (*i)->pieceReadFailed(alert->piece);
        } else {

            std::clog << "Read piece" << alert->piece << "for" << libtorrent::print_address(peerPlugin->endPoint().address()).c_str();
            (*i)->pieceRead(alert->piece, alert->buffer, alert->size);
        }
    }

    // Remove all peers registered for this piece
    _outstandingPieceRequests.remove(alert->piece);
    */
}

void TorrentPlugin::sendTorrentPluginAlert(const alert::TorrentPluginAlert & alert) {

    boost::shared_ptr<libtorrent::torrent> torrent = _torrent.lock();

    assert(torrent);

    torrent->alerts().post_alert(alert);
    //_torrent.alerts().emplace_alert(alert);
}

PeerPlugin * TorrentPlugin::getRawPlugin(const libtorrent::tcp::endpoint & endPoint) {

    auto it = _peers.find(endPoint);

    // peer must be present
    assert(it != _peers.cend());

    // Get plugin reference
    boost::shared_ptr<PeerPlugin> peerPlugin = it->second.lock();
    assert(peerPlugin);

    return peerPlugin.get();
}

void TorrentPlugin::addPeerToSession(const libtorrent::tcp::endpoint & endPoint) {

    // we must know peer
    auto it = _peers.find(endPoint);
    assert(it != _peers.cend());

    // but it must not already be added in session
    assert(!_session.hasConnection(endPoint));

    // Create callback which asserts presence of plugin
    boost::weak_ptr<PeerPlugin> wPeerPlugin = it->second;

    protocol_session::SendMessageOnConnection send = [wPeerPlugin] (const protocol_wire::ExtendedMessagePayload * m) -> void {

        boost::shared_ptr<PeerPlugin> plugin;
        assert(plugin = wPeerPlugin.lock());
        plugin->send(m);
        delete m;
    };

    // add peer to sesion
    _session.addConnection(endPoint, send);
}

void TorrentPlugin::disconnectPeer(const libtorrent::tcp::endpoint & endPoint) {

    PeerPlugin * peerPlugin = getRawPlugin(endPoint);

    // Disconnect connection
    libtorrent::error_code ec;
    peerPlugin->disconnect(ec);

    // Remove from session, if present
    if(_session.hasConnection(endPoint))
        _session.removeConnection(endPoint); // <-- will cause callback to TorrentPlugin::removeConnection

    // Remove from map
    auto it = _peers.find(endPoint);
    _peers.erase(it);
}

void TorrentPlugin::processExtendedMessage(const libtorrent::tcp::endpoint & endPoint, const joystream::protocol_wire::ExtendedMessagePayload & extendedMessage) {

    PeerPlugin * peerPlugin = getRawPlugin(endPoint);

    assert(_session.hasConnection(endPoint));

    // Have session process message
    _session.processMessageOnConnection(endPoint, extendedMessage);
}

void TorrentPlugin::removeConnection(const libtorrent::tcp::endpoint & endPoint, protocol_session::DisconnectCause) {

    // if not peer not in peers map, then just return: may be due to
    // disconnectPeer() call at some earlier point
    auto it = _peers.find(endPoint);
    if(it == _peers.cend())
        return;

    // if cause needs to be recorded, record

    // Disconnect peer
    disconnectPeer(endPoint);
}

//
std::vector<Coin::KeyPair> TorrentPlugin::generateKeyPairsCallbackHandler(int) {

}

//
std::vector<Coin::P2PKHAddress> TorrentPlugin::generateP2PKHAddressesCallbackHandler(int) {

}

//// Buying hooks

//
bool TorrentPlugin::broadcastTransaction(const Coin::Transaction &) {
    // sendTorrentPluginAlert(broadcast transaction)
}

//
void TorrentPlugin::fullPieceArrived(const libtorrent::tcp::endpoint &, const protocol_wire::PieceData &, int) {

}

//// Selling hooks

//
void TorrentPlugin::loadPieceForBuyer(const libtorrent::tcp::endpoint &, unsigned int) {

}

//
void TorrentPlugin::claimLastPayment(const libtorrent::tcp::endpoint &, const joystream::paymentchannel::Payee &) {
    // sendTorrentPluginAlert(settlement transaction: use same as broadcast transaction)
}

// Buyer with given connection id announced anchor
void TorrentPlugin::anchorAnnounced(const libtorrent::tcp::endpoint &, quint64, const Coin::typesafeOutPoint &, const Coin::PublicKey &, const Coin::PubKeyHash &) {
    // sendTorrentPluginAlert(start double spend detection, send an alert)
}


}
}
