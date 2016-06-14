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
    if(accept) {

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

void TorrentPlugin::handle(const request::TorrentPluginRequest * r) {

    assert(r->infoHash == _infoHash);

    // Process request and wrap response in an alert

    libtorrent::alert * alert;

    if(auto startRequest = dynamic_cast<const request::Start *>(r))
        alert = new alert::RequestResult<request::Start::Response>(request::Start::Response(startRequest, start()));
    else if (auto stopRequest = dynamic_cast<const request::Stop *>(r))
        alert = new alert::RequestResult<request::Stop::Response>(request::Stop::Response(stopRequest, stop()));
    else if (auto pauseRequest = dynamic_cast<const request::Pause *>(r))
        alert = new alert::RequestResult<request::Pause::Response>(request::Pause::Response(pauseRequest, pause()));
    else if (auto updateBuyerTermsRequest = dynamic_cast<const request::UpdateBuyerTerms *>(r))
        alert = new alert::RequestResult<request::UpdateBuyerTerms::Response>(request::UpdateBuyerTerms::Response(updateBuyerTermsRequest, updateBuyerTerms(updateBuyerTermsRequest->terms)));
    else if (auto updateSellerTermsRequest = dynamic_cast<const request::UpdateSellerTerms *>(r))
        alert = new alert::RequestResult<request::UpdateSellerTerms::Response>(request::UpdateSellerTerms::Response(updateSellerTermsRequest, updateSellerTerms(updateSellerTermsRequest->terms)));
    else if (auto toObserveModeRequest = dynamic_cast<const request::ToObserveMode *>(r)) {

        // Clear relevant mappings
        // NB: We are doing clearing regardless of whether operation is successful!
        if(_session.mode() == protocol_session::SessionMode::selling)
            _outstandingLoadPieceForBuyerCalls.clear();
        else if(_session.mode() == protocol_session::SessionMode::buying)
            _outstandingFullPieceArrivedCalls.clear();

        alert = new alert::RequestResult<request::ToObserveMode::Response>(request::ToObserveMode::Response(toObserveModeRequest, toObserveMode()));

    } else if (const request::ToSellMode * toSellModeRequest = dynamic_cast<const request::ToSellMode *>(r)) {

        // Should have been cleared before
        assert(_outstandingLoadPieceForBuyerCalls.empty());

        // Clear relevant mappings
        // NB: We are doing clearing regardless of whether operation is successful!
        if(_session.mode() == protocol_session::SessionMode::buying)
            _outstandingFullPieceArrivedCalls.clear();

        alert = new alert::RequestResult<request::ToSellMode::Response>(request::ToSellMode::Response(toSellModeRequest, toSellMode(toSellModeRequest->generateKeyPairsCallbackHandler,
                                                                                                                                    toSellModeRequest->generateP2PKHAddressesCallbackHandler,
                                                                                                                                    toSellModeRequest->sellingPolicy,
                                                                                                                                    toSellModeRequest->terms)));

    } else if (const request::ToBuyMode * toBuyModeRequest = dynamic_cast<const request::ToBuyMode *>(r)) {

        // Should have been cleared before
        assert(_outstandingFullPieceArrivedCalls.empty());

        // Clear relevant mappings
        // NB: We are doing clearing regardless of whether operation is successful!
        if(_session.mode() == protocol_session::SessionMode::selling)
            _outstandingLoadPieceForBuyerCalls.clear();

        alert = new alert::RequestResult<request::ToBuyMode::Response>(request::ToBuyMode::Response(toBuyModeRequest, toBuyMode(toBuyModeRequest->generateKeyPairsCallbackHandler,
                                                                                                                                toBuyModeRequest->generateP2PKHAddressesCallbackHandler,
                                                                                                                                toBuyModeRequest->funding,
                                                                                                                                toBuyModeRequest->policy,
                                                                                                                                toBuyModeRequest->terms)));



    } else if (dynamic_cast<const request::ChangeDownloadLocation *>(r)) // const request::ChangeDownloadLocation * changeDownloadLocationRequest =
        assert(false);
    else
        assert(false);

    // Send alert
    sendTorrentPluginAlertPtr(alert);
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

void TorrentPlugin::sendTorrentPluginAlert(const libtorrent::alert & alert) {

    boost::shared_ptr<libtorrent::torrent> torrent = _torrent.lock();

    assert(torrent);

    torrent->alerts().post_alert(alert);
    //_torrent.alerts().emplace_alert(alert);
}

void TorrentPlugin::sendTorrentPluginAlertPtr(libtorrent::alert * alert) {

    boost::shared_ptr<libtorrent::torrent> torrent = _torrent.lock();

    assert(torrent);

    torrent->alerts().post_alert_ptr(alert);
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

protocol_session::TorrentPieceInformation TorrentPlugin::torrentPieceInformation(const libtorrent::piece_picker & picker) const {

    // Build
    protocol_session::TorrentPieceInformation information;

    // Proper size, but drop later
    //size = getTorrent()->block_size() * picker.blocks_in_piece() or picker.blocks_in_last_piece();

    for(int i = 0; i < picker.num_pieces();i++)
        information.push_back(protocol_session::PieceInformation(0, picker.is_piece_finished(i)));

    return information;
}

request::Start::Outcome TorrentPlugin::start() {

    try {
        _session.start();
    } catch (const protocol_session::exception::StateIncompatibleOperation &) {
        return request::Start::Outcome::StateIncompatibleOperation;
    } catch (const protocol_session::exception::SessionModeNotSetException &) {
        return request::Start::Outcome::SessionModeNotSet;
    }

    return request::Start::Outcome::Success;
}

request::Stop::Outcome TorrentPlugin::stop() {

    try {
        _session.stop();
    } catch (const protocol_session::exception::StateIncompatibleOperation &) {
        return request::Stop::Outcome::StateIncompatibleOperation;
    } catch (const protocol_session::exception::SessionModeNotSetException &) {
        return request::Stop::Outcome::SessionModeNotSet;
    }

    return request::Stop::Outcome::Success;
}

request::Pause::Outcome TorrentPlugin::pause() {

    try {
        _session.pause();
    } catch (const protocol_session::exception::StateIncompatibleOperation &) {
        return request::Pause::Outcome::StateIncompatibleOperation;
    } catch (const protocol_session::exception::SessionModeNotSetException &) {
        return request::Pause::Outcome::SessionModeNotSet;
    }

    return request::Pause::Outcome::Success;
}

request::UpdateBuyerTerms::Outcome TorrentPlugin::updateBuyerTerms(const protocol_wire::BuyerTerms & terms) {

    try {
        _session.updateTerms(terms);
    } catch(const protocol_session::exception::SessionModeNotSetException &) {
        return request::UpdateBuyerTerms::Outcome::SessionModeNotSet;
    } catch(const protocol_session::exception::ModeIncompatibleOperation &) {
        return request::UpdateBuyerTerms::Outcome::ModeIncompatibleOperation;
    }

    return request::UpdateBuyerTerms::Outcome::Success;
}

request::UpdateSellerTerms::Outcome TorrentPlugin::updateSellerTerms(const protocol_wire::SellerTerms & terms) {

    try {
        _session.updateTerms(terms);
    } catch(const protocol_session::exception::SessionModeNotSetException &) {
        return request::UpdateSellerTerms::Outcome::SessionModeNotSet;
    } catch(const protocol_session::exception::ModeIncompatibleOperation &) {
        return request::UpdateSellerTerms::Outcome::ModeIncompatibleOperation;
    }

    return request::UpdateSellerTerms::Outcome::Success;
}

request::ToObserveMode::Outcome TorrentPlugin::toObserveMode() {

    try {
        _session.toObserveMode(removeConnection());
    } catch(const protocol_session::exception::SessionAlreadyInThisMode &) {
        return request::ToObserveMode::Outcome::SessionAlreadyInThisMode;
    }

    return request::ToObserveMode::Outcome::Success;
}

request::ToSellMode::Outcome TorrentPlugin::toSellMode(const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                                       const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                                                       const protocol_session::SellingPolicy & sellingPolicy,
                                                       const protocol_wire::SellerTerms & terms) {

    // Get maximum number of pieces
    int maxPieceIndex = getTorrent()->picker().num_pieces() - 1;

    try {

        _session.toSellMode(removeConnection(),
                            generateKeyPairsCallbackHandler,
                            generateP2PKHAddressesCallbackHandler,
                            loadPieceForBuyer(),
                            claimLastPayment(),
                            anchorAnnounced(),
                            sellingPolicy,
                            terms,
                            maxPieceIndex);

    } catch (const protocol_session::exception::SessionAlreadyInThisMode &) {
        return request::ToSellMode::Outcome::SessionAlreadyInThisMode;
    }

    return request::ToSellMode::Outcome::Success;

}

request::ToBuyMode::Outcome TorrentPlugin::toBuyMode(const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                                     const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                                                     const Coin::UnspentP2PKHOutput & funding,
                                                     const protocol_session::BuyingPolicy & policy,
                                                     const protocol_wire::BuyerTerms & terms) {

    //

    try {

        _session.toBuyMode(removeConnection(),
                           generateKeyPairsCallbackHandler,
                           generateP2PKHAddressesCallbackHandler,
                           broadcastTransaction(),
                           fullPieceArrived(),
                           funding,
                           policy,
                           terms,
                           torrentPieceInformation(getTorrent()->picker()));

    } catch (const protocol_session::exception::SessionAlreadyInThisMode &) {
        return request::ToBuyMode::Outcome::SessionAlreadyInThisMode;
    }

    return request::ToBuyMode::Outcome::Success;

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

void TorrentPlugin::disconnectPeer(const libtorrent::tcp::endpoint & endPoint, const libtorrent::error_code & ec)  {

    // Get plugin
    PeerPlugin * peerPlugin = getRawPlugin(endPoint);

    // Disconnect connection
    peerPlugin->disconnect(ec);

    // Remove from session
     _session.removeConnection(endPoint);

    // Remove from map
    auto it = _peers.find(endPoint);
    _peers.erase(it);
}

void TorrentPlugin::processExtendedMessage(const libtorrent::tcp::endpoint & endPoint, const joystream::protocol_wire::ExtendedMessagePayload & extendedMessage) {

    //PeerPlugin * peerPlugin = getRawPlugin(endPoint);

    assert(_session.hasConnection(endPoint));

    // Have session process message
    _session.processMessageOnConnection(endPoint, extendedMessage);
}

protocol_session::RemovedConnectionCallbackHandler<libtorrent::tcp::endpoint> TorrentPlugin::removeConnection() {

    return [this](const libtorrent::tcp::endpoint & endPoint, protocol_session::DisconnectCause cause) {

        // If the client was cause, then no further processing is required.
        // The callback is then a result of the stupid convention that Session::removeConnection
        // triggers callback.
        if(cause == protocol_session::DisconnectCause::client)
            return;

        assert(this->_peers.count(endPoint));

        // *** Record cause for some purpose? ***

        // Disconnect connection
        libtorrent::error_code ec; // <--- what to put here as cause
        this->disconnectPeer(endPoint, ec);
    };
}

protocol_session::BroadcastTransaction TorrentPlugin::broadcastTransaction() {

    return [this](const Coin::Transaction & tx) -> bool {

        this->sendTorrentPluginAlert(alert::BroadcastTransaction(tx));

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

        getTorrent()->add_piece(index, pieceData.piece().get(), 0);
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
            getTorrent()->read_piece(index);

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

    return [this](const libtorrent::tcp::endpoint &, const joystream::paymentchannel::Payee & payee) {

        Coin::Transaction tx = payee.lastPaymentTransaction();

        this->sendTorrentPluginAlert(alert::BroadcastTransaction(tx));
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
