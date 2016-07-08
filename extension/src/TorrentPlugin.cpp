/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/TorrentPlugin.hpp>
#include <extension/Plugin.hpp>
#include <extension/Request.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/peer_connection_handle.hpp>
#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/socket_io.hpp> // print_endpoint

namespace joystream {
namespace extension {

TorrentPlugin::TorrentPlugin(Plugin * plugin,
                             const libtorrent::torrent_handle & torrent,
                             uint minimumMessageId,
                             const Policy & policy,
                             LibtorrentInteraction libtorrentInteraction)
    : _plugin(plugin)
    , _torrent(torrent)
    , _minimumMessageId(minimumMessageId)
    , _policy(policy)
    , _libtorrentInteraction(libtorrentInteraction) {
}

TorrentPlugin::~TorrentPlugin() {
    std::clog << "~TorrentPlugin()";
}

boost::shared_ptr<libtorrent::peer_plugin> TorrentPlugin::new_connection(const libtorrent::peer_connection_handle & connection) {

    // You cannot disconnect this peer here, e.g. by using peer_connection::disconnect().
    // This is because, at this point (new_connection), the connection has not been
    // added to a torrent level peer list, and the disconnection asserts that the peer has
    // to be in this list. Disconnects must be done later.

    // Get end point
    libtorrent::tcp::endpoint endPoint = connection.remote();

    std::clog << "New"
              << (connection.is_outgoing() ? "outgoing" : "incoming")
              << "connection with"
              << libtorrent::print_endpoint(endPoint)
              << std::endl; // << "on " << _torrent->name().c_str();

    // Screen connection
    libtorrent::error_code ec;
    bool allow = false; // <-- drop when ec can be used

    // ec <-- later put actual error in here, dont just log

    //if(connection.type() != libtorrent::peer_connection::bittorrent_connection)
    //    std::clog << "Peer was not BitTorrent client, likely web seed." << std::endl;
    if(_peers.count(endPoint))
        std::clog << "Peer already added." << std::endl;
    else if(_sentMalformedExtendedMessage.count(endPoint) && _policy.banPeersWithPastMalformedExtendedMessage)
        std::clog << "Peer has previously sent malformed extended message." << std::endl;
    else if(_misbehavedPeers.count(endPoint) && _policy.banPeersWithPastMisbehavior)
        std::clog << "Peer has previously misbehaved." << std::endl;
    else
        allow = true;

    // If connection should not be accepted,
    if(!allow) {

        libtorrent::peer_connection * c = connection.native_handle().get();

        // ***** experiment in order to ask Arvid ****
        c->disconnect(ec, libtorrent::operation_t::op_bittorrent);
        // ***** experiment in order to ask Arvid ****

        if(_peerScheduledForDeletionForGivenError.count(c))
            std::clog << "Peer already scheduled for deletion on next tick, no plugin installed." << std::endl;
        else {
            std::clog << "Peer scheduled for deletion on next tick, no plugin installed." << std::endl;

            _peerScheduledForDeletionForGivenError[c] = ec;
        }

        // and don't install plugin
        return boost::shared_ptr<PeerPlugin>(nullptr);

    }

    std::clog << "Installed seller plugin #" << _peers.size() << std::endl;

    // Create a new peer plugin
    boost::shared_ptr<PeerPlugin> plugin(new PeerPlugin(this, connection, _policy.peerPolicy, _minimumMessageId));

    // Add to collection
    _peers[endPoint] = boost::weak_ptr<PeerPlugin>(plugin);

    // Return pointer to plugin as required
    return plugin;
}

void TorrentPlugin::on_piece_pass(int index) {

    // Make sure we are in correct mode, as mode changed may have occured
    if(_session.mode() == protocol_session::SessionMode::buying) {

        auto it = _outstandingFullPieceArrivedCalls.find(index);

        // If this validation is not due to us
        if(it == _outstandingFullPieceArrivedCalls.cend()) {

            // then just tell session about it
            _session.pieceDownloaded(index);

        } else {

            // if its due to us, then tell session about endpoint and piece
            _session.validPieceReceivedOnConnection(it->second, index);

            // and remove call
            _outstandingFullPieceArrivedCalls.erase(it);
        }
    }
}

void TorrentPlugin::on_piece_failed(int index) {

    // Make sure we are in correct mode, as mode changed may have occured
    if(_session.mode() == protocol_session::SessionMode::buying) {

        auto it = _outstandingFullPieceArrivedCalls.find(index);

        // If this validation is not due to us
        if(it == _outstandingFullPieceArrivedCalls.cend()) {

            // then there is nothing to do

        } else {

            // if its due to us, then tell session about endpoint and piece
            _session.invalidPieceReceivedOnConnection(it->second, index);

            // and remove call
            _outstandingFullPieceArrivedCalls.erase(it);
        }
    }
}

void TorrentPlugin::tick() {

    // Disconnect peers scheduled for instant deletion
    for(auto mapping : _peerScheduledForDeletionForGivenError) {

        std::clog << "Completing delayed disconnection of peer "
                  << libtorrent::print_endpoint(mapping.first->peer_info_struct()->ip())
                  << "due to error: "
                  << mapping.second.message()
                  << std::endl;

        // Disconnect with given error
        mapping.first->disconnect(mapping.second, libtorrent::operation_t::op_bittorrent);
    }

    _peerScheduledForDeletionForGivenError.clear();

    // Asynch processing in session
    _session.tick();
}

bool TorrentPlugin::on_resume() {
    // false: let the standard handler handle this
    return false;
}

bool TorrentPlugin::on_pause() {
    // false: let the standard handler handle this
    return false;
}

void TorrentPlugin::on_files_checked() {
    // nothing to do
}

void TorrentPlugin::on_state(int) {
    // nothing to do
}

void TorrentPlugin::on_add_peer(const libtorrent::tcp::endpoint & endPoint, int /*src*/, int /*flags*/) {

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

void TorrentPlugin::pieceRead(const libtorrent::read_piece_alert * alert) {

    // There should be at least one peer registered for this piece
    auto it = _outstandingLoadPieceForBuyerCalls.find(alert->piece);

    if(it == _outstandingLoadPieceForBuyerCalls.cend()) {

        std::clog << "Ignoring piece read, must be for some other purpose." << std::endl;
        return;
    }

    // Make a callback for each peer registered
    const std::set<libtorrent::tcp::endpoint> & peers = it->second;

    // Iterate peers
    for(auto endPoint : peers) {

        assert(_peers.count(endPoint));

        // Make sure reading worked
        if(alert->ec) {

            std::clog << "Failed reading piece" << alert->piece << "for" << libtorrent::print_address(endPoint.address()).c_str();
            assert(false);

        } else {

            std::clog << "Read piece" << alert->piece << "for" << libtorrent::print_address(endPoint.address()).c_str();

            // tell session
            _session.pieceLoaded(endPoint, protocol_wire::PieceData(alert->buffer, alert->size), alert->piece);
        }
    }

    // Remove all peers registered for this piece
    _outstandingLoadPieceForBuyerCalls.erase(it);
}

void TorrentPlugin::start() {

    auto initialState = sessionState();

    // Start session
    _session.start();

    // If session was initially stopped (not paused), then initiate extended handshake
    if(initialState == protocol_session::SessionState::stopped)
        forEachBitTorrentConnection([](libtorrent::bt_peer_connection *c) -> void { c->write_extensions(); });

}

void TorrentPlugin::stop() {

    // Setup peers to send uninstall handshakes on next call from libtorrent (add_handshake)
    for(auto mapping : _peers) {

         boost::shared_ptr<PeerPlugin> peerPlugin = mapping.second.lock();

         assert(peerPlugin);

         peerPlugin->setSendUninstallMappingOnNextExtendedHandshake(true);
    }

    // Stop session
    // NB: This will not cause disconnect of underlying peers,
    // as we don't initate it in callback from session.
    _session.stop();

    // Start handshake
    forEachBitTorrentConnection([](libtorrent::bt_peer_connection *c) -> void { c->write_extensions(); });
}

void TorrentPlugin::pause() {
    _session.pause();
}

void TorrentPlugin::updateTerms(const protocol_wire::SellerTerms & terms) {
    _session.updateTerms(terms);
}

void TorrentPlugin::updateTerms(const protocol_wire::BuyerTerms & terms) {
    _session.updateTerms(terms);
}

void TorrentPlugin::toObserveMode() {

    // Clear relevant mappings
    // NB: We are doing clearing regardless of whether operation is successful!
    if(_session.mode() == protocol_session::SessionMode::selling)
        _outstandingLoadPieceForBuyerCalls.clear();
    else if(_session.mode() == protocol_session::SessionMode::buying)
        _outstandingFullPieceArrivedCalls.clear();

    _session.toObserveMode(removeConnection());
}

void TorrentPlugin::toSellMode(const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                               const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                               const protocol_session::SellingPolicy & policy,
                               const protocol_wire::SellerTerms & terms) {

    // Should have been cleared before
    assert(_outstandingLoadPieceForBuyerCalls.empty());

    // Clear relevant mappings
    // NB: We are doing clearing regardless of whether operation is successful!
    if(_session.mode() == protocol_session::SessionMode::buying)
        _outstandingFullPieceArrivedCalls.clear();

    // Get maximum number of pieces
    int maxPieceIndex = torrent()->picker().num_pieces() - 1;

    _session.toSellMode(removeConnection(),
                        generateKeyPairsCallbackHandler,
                        generateP2PKHAddressesCallbackHandler,
                        loadPieceForBuyer(),
                        claimLastPayment(),
                        anchorAnnounced(),
                        policy,
                        terms,
                        maxPieceIndex);
}

void TorrentPlugin::toBuyMode(const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                              const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                              const Coin::UnspentP2PKHOutput & funding,
                              const protocol_session::BuyingPolicy & policy,
                              const protocol_wire::BuyerTerms & terms) {

    // Should have been cleared before
    assert(_outstandingFullPieceArrivedCalls.empty());

    // Clear relevant mappings
    // NB: We are doing clearing regardless of whether operation is successful!
    if(_session.mode() == protocol_session::SessionMode::selling)
        _outstandingLoadPieceForBuyerCalls.clear();

    _session.toBuyMode(removeConnection(),
                       generateKeyPairsCallbackHandler,
                       generateP2PKHAddressesCallbackHandler,
                       broadcastTransaction(),
                       fullPieceArrived(),
                       funding,
                       policy,
                       terms,
                       torrentPieceInformation(torrent()->picker()));
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

        status.peers.insert(std::make_pair(mapping.first, plugin->status()));
    }

    return status;
}

TorrentPlugin::LibtorrentInteraction TorrentPlugin::libtorrentInteraction() const {
    return _libtorrentInteraction;
}

libtorrent::alert_manager & TorrentPlugin::alert_manager() const {
    return torrent()->alerts();
}

PeerPlugin * TorrentPlugin::peer(const libtorrent::tcp::endpoint & endPoint) {

    auto it = _peers.find(endPoint);

    // peer must be present
    assert(it != _peers.cend());

    // Get plugin reference
    boost::shared_ptr<PeerPlugin> peerPlugin = it->second.lock();
    assert(peerPlugin);

    return peerPlugin.get();
}

libtorrent::torrent * TorrentPlugin::torrent() const {

    boost::shared_ptr<libtorrent::torrent> torrent = _torrent.native_handle();
    assert(torrent);

    return torrent.get();
}

protocol_session::TorrentPieceInformation TorrentPlugin::torrentPieceInformation(const libtorrent::piece_picker & picker) const {

    // Build
    protocol_session::TorrentPieceInformation information;

    // Proper size, but drop later
    //size = getTorrent()->block_size() * picker.blocks_in_piece() or picker.blocks_in_last_piece();

    for(int i = 0; i < picker.num_pieces();i++)
        information.push_back(protocol_session::PieceInformation(0, picker.is_piece_finished(i)));

    return information;
}

void TorrentPlugin::forEachBitTorrentConnection(const std::function<void(libtorrent::bt_peer_connection *)> & h) {

    for(auto mapping : _peers) {

         boost::shared_ptr<PeerPlugin> plugin = mapping.second.lock();

         assert(plugin);

         // Get connection reference
         boost::shared_ptr<libtorrent::peer_connection> nativeConnection = plugin->connection().native_handle();

         // If connection is a BitTorrent connection, then apply handler
         if(nativeConnection->type() == libtorrent::peer_connection::bittorrent_connection)
             h(static_cast<libtorrent::bt_peer_connection *>(nativeConnection.get()));
    }

}

void TorrentPlugin::setLibtorrentInteraction(LibtorrentInteraction e) {

    // Send messages for starting to prevent uploading
    if(e == LibtorrentInteraction::BlockUploading ||
       e == LibtorrentInteraction::BlockUploadingAndDownloading) {

        // For each peer: sending (once) CHOCKED message in order to discourage inbound requests.
        forEachBitTorrentConnection([](libtorrent::bt_peer_connection *c) -> void { c->write_choke(); });
    }

    // Send messages for starting to prevent uploading
    if(e == LibtorrentInteraction::BlockDownloading ||
       e == LibtorrentInteraction::BlockUploadingAndDownloading) {

        // For each peer: sending (once) NOT-INTERESTED and CHOCKED message in order to discourage unchocking.
        forEachBitTorrentConnection([](libtorrent::bt_peer_connection *c) -> void { c->write_not_interested(); c->write_choke(); });
    }

    _libtorrentInteraction = e;
}

protocol_session::SessionState TorrentPlugin::sessionState() const {
    return _session.state();
}

void TorrentPlugin::addToSession(const libtorrent::tcp::endpoint & endPoint) {

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

void TorrentPlugin::removeFromSession(const libtorrent::tcp::endpoint & endPoint) {

    if(_session.hasConnection(endPoint))
        _session.removeConnection(endPoint);
}

void TorrentPlugin::drop(const libtorrent::tcp::endpoint & endPoint, const libtorrent::error_code & ec)  {

    if(ec) {
        // log string version: std::clog << "Malformed handshake received: m key not mapping to dictionary.";
        // insert into _sentMalformedExtendedMessage
        // insert into _misbehavedPeers
    }

    // Get plugin
    PeerPlugin * peerPlugin = peer(endPoint);

    // Mark as undead
    // NB: must be done before closing connection, due to on_disconnect callback from libtorrent
    assert(!peerPlugin->undead());
    peerPlugin->setUndead(true);

    // Initiate closing connection
    peerPlugin->connection().disconnect(ec, libtorrent::operation_t::op_bittorrent);

    // Remove from session if present
    removeFromSession(endPoint);

    // Remove from map
    auto it = _peers.find(endPoint);
    _peers.erase(it);
}

void TorrentPlugin::processExtendedMessage(const libtorrent::tcp::endpoint & endPoint, const joystream::protocol_wire::ExtendedMessagePayload & extendedMessage){

    // Should have been checked by peer plugin
    assert(_session.hasConnection(endPoint));

    // Have session process message
    _session.processMessageOnConnection(endPoint, extendedMessage);
}

protocol_session::RemovedConnectionCallbackHandler<libtorrent::tcp::endpoint> TorrentPlugin::removeConnection() {

    return [this](const libtorrent::tcp::endpoint & endPoint, protocol_session::DisconnectCause cause) {

        // If the client was cause, then no further processing is required.
        // The callback is then a result of the stupid convention that Session::removeConnection()/stop()
        // triggers callback.
        if(cause == protocol_session::DisconnectCause::client)
            return;
        else // all other reasons are considered misbehaviour
            _misbehavedPeers.insert(endPoint);

        assert(this->_peers.count(endPoint));

        // *** Record cause for some purpose? ***

        // Disconnect connection
        libtorrent::error_code ec; // <--- what to put here as cause
        this->drop(endPoint, ec);
    };
}

protocol_session::BroadcastTransaction TorrentPlugin::broadcastTransaction() {

    // Get alert manager and handle for torrent
    libtorrent::torrent * t = torrent();
    libtorrent::alert_manager & manager = t->alerts();
    libtorrent::torrent_handle h = t->get_handle();

    return [&manager, h](const Coin::Transaction & tx) -> bool {

        manager.emplace_alert<alert::BroadcastTransaction>(h, tx);

        return true; // remove later, there is a github issue
    };
}

protocol_session::FullPieceArrived<libtorrent::tcp::endpoint> TorrentPlugin::fullPieceArrived() {

    return [this](const libtorrent::tcp::endpoint & endPoint, const protocol_wire::PieceData & pieceData, int index) -> void {

        // Make sure outstanding calls exist for this index
        assert(!_outstandingFullPieceArrivedCalls.count(index));

        _outstandingFullPieceArrivedCalls[index] = endPoint;

        // Tell libtorrent to validate piece
        // last argument is a flag which presently seems to only test
        // flags & torrent::overwrite_existing, which seems to be whether
        // the piece should be overwritten if it is already present
        //
        // libtorrent::torrent_plugin::on_piece_pass()
        // libtorrent::torrent_plugin::on_piece_failed()
        // processes result of checking

        torrent()->add_piece(index, pieceData.piece().get(), 0);
    };
}

protocol_session::LoadPieceForBuyer<libtorrent::tcp::endpoint> TorrentPlugin::loadPieceForBuyer() {

    return [this](const libtorrent::tcp::endpoint & endPoint, unsigned int index) -> void {

        // Get reference to, possibly new - and hence empty, set of calls for given piece index
        std::set<libtorrent::tcp::endpoint> & callSet = this->_outstandingLoadPieceForBuyerCalls[index];

        // was there no previous calls for this piece?
        if(callSet.empty()) {

            std::clog << "["
                      << _outstandingLoadPieceForBuyerCalls[index].size()
                      << "] Requested piece"
                      << index
                      << "by"
                      << libtorrent::print_address(endPoint.address()).c_str()
                      << std::endl;

            // then we make first call
            torrent()->read_piece(index);

        } else {

            // otherwise we dont need to make a new call, a response will come from libtorrent
            std::clog << "["
                      << _outstandingLoadPieceForBuyerCalls[index].size()
                      << "] Skipping requested piece"
                      << index
                      << "by"
                      << libtorrent::print_address(endPoint.address()).c_str()
                      << std::endl;

        }

        // and in any case, remember to notify this endpoint
        // when piece is loaded
        callSet.insert(endPoint);
    };
}

protocol_session::ClaimLastPayment<libtorrent::tcp::endpoint> TorrentPlugin::claimLastPayment() {

    // Get alert manager and handle for torrent
    libtorrent::torrent * t = torrent();
    libtorrent::alert_manager & manager = t->alerts();
    libtorrent::torrent_handle h = t->get_handle();

    return [&manager, h](const libtorrent::tcp::endpoint &, const joystream::paymentchannel::Payee & payee) {

        Coin::Transaction tx = payee.lastPaymentTransaction();

        manager.emplace_alert<alert::BroadcastTransaction>(h, tx);
    };
}

protocol_session::AnchorAnnounced<libtorrent::tcp::endpoint> TorrentPlugin::anchorAnnounced() {

    // Get alert manager and handle for torrent
    libtorrent::torrent * t = torrent();
    libtorrent::alert_manager & manager = t->alerts();
    libtorrent::torrent_handle h = t->get_handle();

    return [&manager, h](const libtorrent::tcp::endpoint & endPoint, quint64 value, const Coin::typesafeOutPoint & anchor, const Coin::PublicKey & contractPk, const Coin::PubKeyHash & finalPkHash) {

        manager.emplace_alert<alert::AnchorAnnounced>(h, endPoint, value, anchor, contractPk, finalPkHash);
    };
}

}
}
