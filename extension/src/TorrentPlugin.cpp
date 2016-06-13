/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/TorrentPlugin.hpp>
#include <extension/Plugin.hpp>
#include <extension/Alert.hpp>
#include <extension/Request.hpp>

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

    if(_session.mode() == protocol_session::SessionMode::buying) {

        // tell session about this? but how to figure out
        //_session.invalidPieceReceivedOnConnection(index??);
    }
}

void TorrentPlugin::tick() {
    _session.tick();
}

bool TorrentPlugin::on_resume() {
    // deal with this later
}

bool TorrentPlugin::on_pause() {
    // deal with this later
}

void TorrentPlugin::on_files_checked() {
    // nothing to do
}

void TorrentPlugin::on_state(int s) {
    // nothing to do
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

void TorrentPlugin::handle(const request::TorrentPluginRequest * r) {

    assert(r->infoHash == _infoHash);

    try {

        if(dynamic_cast<const request::Start *>(r))
            _session.start();
        else if (dynamic_cast<const request::Stop *>(r))
            _session.stop();
        else if (dynamic_cast<const request::Pause *>(r))
            _session.pause();
        else if (const request::UpdateBuyerTerms * updateBuyerTerms = dynamic_cast<const request::UpdateBuyerTerms *>(r))
            _session.updateTerms(updateBuyerTerms->terms);
        else if (const request::UpdateSellerTerms * updateSellerTerms = dynamic_cast<const request::UpdateSellerTerms *>(r))
            _session.updateTerms(updateSellerTerms->terms);
        else if (dynamic_cast<const request::ToObserveMode *>(r)) {

            // Make sure to clear
            if(_session.mode() == protocol_session::SessionMode::selling)
                _outstandingReadPieceRequests.clear();

            _session.toObserveMode(removeConnection());

        } else if (const request::ToSellMode * toSellMode = dynamic_cast<const request::ToSellMode *>(r)) {

            assert(_outstandingReadPieceRequests.empty());

            // Get maximum number of pieces
            int maxPieceIndex = getTorrent()->picker().num_pieces() - 1;

            // Go to mode
            _session.toSellMode(removeConnection(),
                                toSellMode->generateKeyPairsCallbackHandler,
                                toSellMode->generateP2PKHAddressesCallbackHandler,
                                loadPieceForBuyer(),
                                claimLastPayment(),
                                anchorAnnounced(),
                                toSellMode->sellingPolicy,
                                toSellMode->terms,
                                maxPieceIndex);

        } else if (const request::ToBuyMode * toBuyMode = dynamic_cast<const request::ToBuyMode *>(r)) {

            // Make sure to clear
            if(_session.mode() == protocol_session::SessionMode::selling)
                _outstandingReadPieceRequests.clear();

            _session.toBuyMode(removeConnection(),
                               toBuyMode->generateKeyPairsCallbackHandler,
                               toBuyMode->generateP2PKHAddressesCallbackHandler,
                               broadcastTransaction(),
                               fullPieceArrived(),
                               toBuyMode->funding,
                               toBuyMode->policy,
                               toBuyMode->terms,
                               torrentPieceInformation());

        } else if (const request::ChangeDownloadLocation * changeDownloadLocation = dynamic_cast<const request::ChangeDownloadLocation *>(r))
            assert(false);
        else
            assert(false);

    } catch (const protocol_session::exception::StateIncompatibleOperation & e) {
        sendTorrentPluginAlert(alert::RequestResult<protocol_session::exception::StateIncompatibleOperation>(e));
    } catch (const protocol_session::exception::SessionModeNotSetException & e) {
        sendTorrentPluginAlert(alert::RequestResult<protocol_session::exception::SessionModeNotSetException>(e));
    } catch (const protocol_session::exception::ConnectionAlreadyAddedException<libtorrent::tcp::endpoint> & e) {
        sendTorrentPluginAlert(alert::RequestResult<protocol_session::exception::ConnectionAlreadyAddedException<libtorrent::tcp::endpoint>>(e));
    } catch (const protocol_session::exception::InvalidPieceIndexException & e) {
        sendTorrentPluginAlert(alert::RequestResult<protocol_session::exception::InvalidPieceIndexException>(e));
    } catch (const protocol_session::exception::SessionAlreadyInThisMode & e) {
        sendTorrentPluginAlert(alert::RequestResult<protocol_session::exception::SessionAlreadyInThisMode>(e));
    } catch (const protocol_session::exception::ModeIncompatibleOperation & e) {
        sendTorrentPluginAlert(alert::RequestResult<protocol_session::exception::ModeIncompatibleOperation>(e));
    } catch (const protocol_session::exception::IncorrectPieceIndex & e) {
        sendTorrentPluginAlert(alert::RequestResult<protocol_session::exception::IncorrectPieceIndex>(e));
    }

    delete r;
}

void TorrentPlugin::pieceRead(const libtorrent::read_piece_alert * alert) {


    // There should be at least one peer registered for this piece
    //Q_ASSERT(!_outstandingPieceRequests[alert->piece].empty());
    if(_outstandingReadPieceRequests[alert->piece].empty()) {

        std::clog << "Ignoring piece read, must be for streaming server.";
        return;
    }

    // Make a callback for each peer registered
    const std::set<libtorrent::tcp::endpoint> & peers = _outstandingReadPieceRequests[alert->piece];

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
    _outstandingReadPieceRequests.remove(alert->piece);
}

status::TorrentPlugin TorrentPlugin::status() const {

    status::TorrentPlugin status;

    status.infoHash = _infoHash;
    status.session = _session.status();

    // Get state of peer plugins
    for(auto mapping : _peers) {

        // Get shared_ptr reference to peer pluging
        boost::shared_ptr<PeerPlugin> plugin = mapping.second.lock();

        assert(plugin);

        status.peers.insert(mapping.first, plugin->status());
    }

    return status;
}

void TorrentPlugin::sendTorrentPluginAlert(const libtorrent::alert & alert) {

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

libtorrent::torrent * TorrentPlugin::getTorrent()  {

    boost::shared_ptr<libtorrent::torrent> torrent = _torrent.lock();
    assert(torrent);

    return torrent.get();
}

protocol_session::TorrentPieceInformation TorrentPlugin::torrentPieceInformation() const {

    libtorrent::piece_picker & picker = getTorrent()->picker();

    // Build
    protocol_session::TorrentPieceInformation information;

    // Proper size, but drop later
    //size = getTorrent()->block_size() * picker.blocks_in_piece() or picker.blocks_in_last_piece();

    for(int i = 0; i < picker.num_pieces();i++)
        information.push_back(protocol_session::PieceInformation(0, picker.is_piece_finished(i));

    return information;
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

protocol_session::RemovedConnectionCallbackHandler<libtorrent::tcp::endpoint> TorrentPlugin::removeConnection() const {

    return [this](const libtorrent::tcp::endpoint & endPoint, protocol_session::DisconnectCause) {

        // if not peer not in peers map, then just return: may be due to
        // disconnectPeer() call at some earlier point
        auto it = this->_peers.find(endPoint);
        if(it == this->_peers.cend())
            return;

        // if cause needs to be recorded, record

        // Disconnect peer
        this->disconnectPeer(endPoint);
    };
}

protocol_session::BroadcastTransaction TorrentPlugin::broadcastTransaction() const {

    return [this](const Coin::Transaction & tx) -> bool {
        this->sendTorrentPluginAlert(alert::BroadcastTransaction(tx));
    };
}

protocol_session::FullPieceArrived<libtorrent::tcp::endpoint> TorrentPlugin::fullPieceArrived() const {

    return [this](const libtorrent::tcp::endpoint & endPoint, const protocol_wire::PieceData & pieceData, int index) -> void {

    };
}

protocol_session::LoadPieceForBuyer<libtorrent::tcp::endpoint> TorrentPlugin::loadPieceForBuyer() const {

    return [this](const libtorrent::tcp::endpoint & endPoint, unsigned int index) -> void {

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

    };
}

protocol_session::ClaimLastPayment<libtorrent::tcp::endpoint> TorrentPlugin::claimLastPayment() const {

    return [this](const libtorrent::tcp::endpoint & endPoint, const joystream::paymentchannel::Payee &) {
        // sendTorrentPluginAlert(settlement transaction: use same as broadcast transaction)
    };
}

protocol_session::AnchorAnnounced<libtorrent::tcp::endpoint> TorrentPlugin::anchorAnnounced() const {
    // sendTorrentPluginAlert(start double spend detection, send an alert)
    // start checking for double spends??
    return [this](const libtorrent::tcp::endpoint & endPoint, quint64 value, const Coin::typesafeOutPoint & anchor, const Coin::PublicKey & contractPk, const Coin::PubKeyHash & finalPkHash) {
        // sendTorrentPluginAlert(settlement transaction: use same as broadcast transaction)
    };
}

}
}
