/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/SellerTorrentPlugin.hpp>
#include <core/extension/Plugin.hpp>
#include <core/extension/PluginMode.hpp>
#include <core/extension/SellerPeerPlugin.hpp>
#include <wallet/Wallet.hpp>

#include <libtorrent/error_code.hpp>
#include <libtorrent/peer_connection.hpp>
#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/socket_io.hpp> // print_endpoint
#include <libtorrent/alert_types.hpp>

#include <QLoggingCategory>

/**
 * @brief SellerTorrentPlugin::Status
 */

SellerTorrentPlugin::Status::Status() {
}

SellerTorrentPlugin::Status::Status(quint64 minPrice,
                                    quint32 minLock,
                                    quint64 minFeePerByte,
                                    quint32 maxNumberOfSellers,
                                    quint32 maxContractConfirmationDelay,
                                    qint64 balance,
                                    const QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status> & peerPluginStatuses)
    : _minPrice(minPrice)
    , _minLock(minLock)
    , _minFeePerByte(minFeePerByte)
    , _maxNumberOfSellers(maxNumberOfSellers)
    , _maxContractConfirmationDelay(maxContractConfirmationDelay)
    , _balance(balance)
    , _peerPluginStatuses(peerPluginStatuses) {
}

quint64 SellerTorrentPlugin::Status::minPrice() const {
    return _minPrice;
}

void SellerTorrentPlugin::Status::setMinPrice(quint64 minPrice) {
    _minPrice = minPrice;
}

quint32 SellerTorrentPlugin::Status::minLock() const {
    return _minLock;
}

void SellerTorrentPlugin::Status::setMinLock(quint32 minLock) {
    _minLock = minLock;
}

quint32 SellerTorrentPlugin::Status::maxNumberOfSellers() const {
    return _maxNumberOfSellers;
}

void SellerTorrentPlugin::Status::setMaxNumberOfSellers(quint32 maxNumberOfSellers) {
    _maxNumberOfSellers = maxNumberOfSellers;
}

quint64 SellerTorrentPlugin::Status::minFeePerByte() const {
    return _minFeePerByte;
}

void SellerTorrentPlugin::Status::setMinFeePerByte(quint64 minFeePerByte) {
    _minFeePerByte = minFeePerByte;
}

quint32 SellerTorrentPlugin::Status::maxContractConfirmationDelay() const {
    return _maxContractConfirmationDelay;
}

void SellerTorrentPlugin::Status::setMaxContractConfirmationDelay(quint32 maxContractConfirmationDelay) {
    _maxContractConfirmationDelay = maxContractConfirmationDelay;
}

qint64 SellerTorrentPlugin::Status::balance() const {
    return _balance;
}

void SellerTorrentPlugin::Status::setBalance(qint64 balance) {
    _balance = balance;
}

QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status> SellerTorrentPlugin::Status::peerPluginStatuses() const {
    return _peerPluginStatuses;
}

void SellerTorrentPlugin::Status::setPeerPluginStatuses(const QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status> & peerStatuses) {
    _peerPluginStatuses = peerStatuses;
}

/**
 * @brief SellerTorrentPlugin::Configuration
 */

SellerTorrentPlugin::Configuration::Configuration() {

}

SellerTorrentPlugin::Configuration::Configuration(bool enableBanningSets,
                                                  quint64 minPrice,
                                                  quint32 minLock,
                                                  quint64 minFeePerByte,
                                                  quint32 maxNumberOfSellers,
                                                  quint32 maxContractConfirmationDelay)
    : TorrentPlugin::Configuration(enableBanningSets)
    , _minPrice(minPrice)
    , _minLock(minLock)
    , _minFeePerByte(minFeePerByte)
    , _maxNumberOfSellers(maxNumberOfSellers)
    , _maxContractConfirmationDelay(maxContractConfirmationDelay) {
}

SellerTorrentPlugin::Configuration::Configuration(const libtorrent::entry::dictionary_type & dictionaryEntry) {
}

quint64 SellerTorrentPlugin::Configuration::minPrice() const {
    return _minPrice;
}

void SellerTorrentPlugin::Configuration::setMinPrice(quint64 minPrice) {
    _minPrice = minPrice;
}

quint32 SellerTorrentPlugin::Configuration::minLock() const {
    return _minLock;
}

void SellerTorrentPlugin::Configuration::setMinLock(quint32 minLock) {
    _minLock = minLock;
}

quint64 SellerTorrentPlugin::Configuration::minFeePerByte() const {
    return _minFeePerByte;
}

void SellerTorrentPlugin::Configuration::setMinFeePerByte(quint64 minFeePerByte) {
    _minFeePerByte = minFeePerByte;
}

quint32 SellerTorrentPlugin::Configuration::maxNumberOfSellers() const {
    return _maxNumberOfSellers;
}

void SellerTorrentPlugin::Configuration::setMaxNumberOfSellers(quint32 minNumberOfSellers) {
    _maxNumberOfSellers = minNumberOfSellers;
}

quint32 SellerTorrentPlugin::Configuration::maxContractConfirmationDelay() const {
    return _maxContractConfirmationDelay;
}

void SellerTorrentPlugin::Configuration::setMaxContractConfirmationDelay(quint32 maxContractConfirmationDelay) {
    _maxContractConfirmationDelay = maxContractConfirmationDelay;
}

PluginMode SellerTorrentPlugin::Configuration::pluginMode() const {
    return PluginMode::Seller;
}

/**
 * SellerTorrentPlugin
 */

#include <core/extension/Alert/SellerTorrentPluginStatusAlert.hpp>
#include <core/extension/Alert/SellerPeerAddedAlert.hpp>
#include <core/extension/Alert/SellerPeerPluginRemovedAlert.hpp>

#include <wallet/Wallet.hpp>
#include <CoinCore/CoinNodeData.h>

SellerTorrentPlugin::SellerTorrentPlugin(Plugin * plugin,
                                         const boost::shared_ptr<libtorrent::torrent> & torrent,
                                         Manager * wallet,
                                         const SellerTorrentPlugin::Configuration & configuration,
                                         QLoggingCategory & category)
    : TorrentPlugin(plugin, torrent, configuration, category)
    , _wallet(wallet)
    , _minPrice(configuration.minPrice())
    , _minLock(configuration.minLock())
    , _minFeePerByte(configuration.minFeePerByte())
    , _maxNumberOfSellers(configuration.maxNumberOfSellers())
    , _maxContractConfirmationDelay(configuration.maxContractConfirmationDelay())
    , _balance(0) {
}

boost::shared_ptr<libtorrent::peer_plugin> SellerTorrentPlugin::new_connection(libtorrent::peer_connection * peerConnection) {

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
        peerPlugin = createRegularSellerPeerPlugin(bittorrentPeerConnection);

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

SellerPeerPlugin * SellerTorrentPlugin::createRegularSellerPeerPlugin(libtorrent::bt_peer_connection * connection) {

    // Get fresh key pairs for seller side of contract
    //QList<Wallet::Entry> contractKeysEntry = _wallet->generateNewKeys(1, Wallet::Purpose::SellerInContractOutput).values();
    QList<Coin::KeyPair> contractKeysEntry = _wallet->issueKeyPairs(1);
    Coin::KeyPair payeeContractKeys = contractKeysEntry.front();

    QList<Coin::KeyPair> paymentKeysEntry = _wallet->issueKeyPairs(1);
    Coin::KeyPair payeePaymentKeys = paymentKeysEntry.front();

    return new SellerPeerPlugin(this,
                                connection,
                                false,
                                Payee::Configuration(Payee::State::waiting_for_payor_information,
                                                     0,
                                                     Coin::Signature(),
                                                     _minLock,
                                                     _minPrice,
                                                     _maxNumberOfSellers,
                                                     payeeContractKeys,
                                                     payeePaymentKeys,
                                                     Coin::typesafeOutPoint(),
                                                     Coin::PublicKey(),
                                                     Coin::PublicKey(),
                                                     0),
                                _torrent->torrent_file().num_pieces(),
                                _category);
}

SellerPeerPlugin * SellerTorrentPlugin::createSellerPeerPluginScheduledForDeletion(libtorrent::bt_peer_connection * connection) {

    return new SellerPeerPlugin(this,
                                connection,
                                true,
                                Payee::Configuration(),
                                0,
                                _category);
}

void SellerTorrentPlugin::on_piece_pass(int index) {

}

void SellerTorrentPlugin::on_piece_failed(int index) {

}

void SellerTorrentPlugin::tick() {

    // Send status to controller
    sendTorrentPluginAlert(SellerTorrentPluginStatusAlert(_torrent->info_hash(), status()));

    // Delete peer plugins
    int toBeDeleted = _peersScheduledForDeletion.size();
    int actuallyDeleted = deleteAndDisconnectPeers();

    Q_ASSERT(toBeDeleted == actuallyDeleted);
    Q_ASSERT(_peersScheduledForDeletion.empty());

    if(actuallyDeleted > 0)
        qCDebug(_category) << "Disconnected and deleted" << actuallyDeleted << "peer plugins.";
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

/**
void SellerTorrentPlugin::async_read(const libtorrent::peer_request & r,
                                     const boost::function<void(int, libtorrent::disk_io_job const&)> & handler,
                                     int cache_line_size,
                                     int cache_expiry) {

    _torrent->filesystem().async_read(r, handler, cache_line_size, cache_expiry);
}
*/

void SellerTorrentPlugin::readPiece(SellerPeerPlugin * peer, int piece) {

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

void SellerTorrentPlugin::pieceRead(const libtorrent::read_piece_alert * alert) {

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

/**
quint64 SellerTorrentPlugin::totalReceivedSinceStart() const {

    quint64 total = 0;

    for(QMap<libtorrent::tcp::endpoint, boost::shared_ptr<SellerPeerPlugin> >::const_iterator
        i = _peers.constBegin(),
        end = _peers.constEnd();
        i != end;i++) {

        // Get peer plugin
        const boost::shared_ptr<SellerPeerPlugin> & peer = *i;

        // Count how much has been received
        total += peer->totalReceivedSinceStart();
    }

    return total;
}
*/

void SellerTorrentPlugin::on_peer_plugin_disconnect(SellerPeerPlugin * peerPlugin, libtorrent::error_code const & ec) {

    // Get endpoint
    libtorrent::tcp::endpoint endPoint = peerPlugin->endPoint();

    qCDebug(_category) << "on_disconnect ["<< (peerPlugin->connection()->is_outgoing() ? "outgoing" : "incoming") << "]:" << ec.message().c_str();

    if(!peerPlugin->scheduledForDeletingInNextTorrentPluginTick()) {

        // Scheduled for deletion <=> must NOT be in peers map
        Q_ASSERT(_peers.contains(endPoint));

        // Remove peer plugin from map
        boost::weak_ptr<SellerPeerPlugin> weakPtr = _peers.take(endPoint);

        // ASSERT: is same plugin
        Q_ASSERT(!weakPtr.expired());
        boost::shared_ptr<SellerPeerPlugin> sharedPtr = weakPtr.lock();
        Q_ASSERT(sharedPtr.get() == peerPlugin);

        // Schedule for prompt deletion
        peerPlugin->setScheduledForDeletingInNextTorrentPluginTick(true);

        // Save error_code which
        peerPlugin->setDeletionErrorCode(ec);

        // Place in deletion list
        _peersScheduledForDeletion.append(weakPtr);

        // Send alert for peer plugin removal
        sendTorrentPluginAlert(SellerPeerPluginRemovedAlert(_torrent->info_hash(), sharedPtr->endPoint()));

        // Try to clo
        SellerPeerPlugin::ClientState clientState = peerPlugin->clientState();

        if(clientState == SellerPeerPlugin::ClientState::awaiting_payment ||
           clientState == SellerPeerPlugin::ClientState::awaiting_piece_request_after_payment ||
           clientState == SellerPeerPlugin::ClientState::reading_piece_from_disk) {

            qCDebug(_category) << "Trying to claim peer plugin payment";

            // Make ONE stab at claiming payment
            Coin::Transaction tx = peerPlugin->lastPaymentTransaction();

            /**
             * Try to broadcast!!
             * In the future, keep track of how this turned out, and also rebroadcast,
             * and also track IP of peers which coincide with failed claims.
             * Detect double spends, etc.
             */
            _wallet->broadcast(tx);

            // Alter state
            peerPlugin->setClientState(SellerPeerPlugin::ClientState::trying_to_claim_last_payment);
        }

    } else {

        /**
         * SLOW ASSERT:
         * _peers does not contain peer pluing
         */
        if(_peers.contains(endPoint)) {

            boost::weak_ptr<SellerPeerPlugin> weakPtr = _peers[endPoint];

            // ASSERT: is NOT same plugin
            Q_ASSERT(!weakPtr.expired());
            boost::shared_ptr<SellerPeerPlugin> sharedPtr = weakPtr.lock();
            Q_ASSERT(sharedPtr.get() != peerPlugin);

        }

        /**
         * SLOW ASSERT:
         * _peersScheduledForDeletion does contain peer plugin
         */
        bool present = false;
        for(QList<boost::weak_ptr<SellerPeerPlugin> >::const_iterator
            i = _peersScheduledForDeletion.constBegin(),
            end = _peersScheduledForDeletion.constEnd();
            i != end;i++) {

            // Get weak ptr
            const boost::weak_ptr<SellerPeerPlugin> weakPtr = *i;

            if(!weakPtr.expired()) {

                const boost::shared_ptr<SellerPeerPlugin> sharedPtr = weakPtr.lock();

                if(sharedPtr.get() == peerPlugin) {
                    present = true;
                    break;
                }
            }
        }

        Q_ASSERT(present);
    }
}

qint64 SellerTorrentPlugin::addToBalance(quint64 revenue) {

    // Notify plugin
    _plugin->registerReceivedFunds(revenue);

    // Add to balance
    _balance += revenue;

    return _balance;
}

// Creates status for plugin
SellerTorrentPlugin::Status SellerTorrentPlugin::status() const {

    // Create map for peer plugin statuses
    QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status> peerStatuses;

    for(QMap<libtorrent::tcp::endpoint, boost::weak_ptr<SellerPeerPlugin> >::const_iterator
        i = _peers.constBegin(),
        end = _peers.constEnd();
        i != end;i++) {

        // Get peer
        const boost::weak_ptr<SellerPeerPlugin> weakPtr = i.value();

        // Make sure peer plugin is still valid
        if(const boost::shared_ptr<SellerPeerPlugin> sharedPtr = weakPtr.lock()) {

            // Compute status and save in map
            peerStatuses[i.key()] = sharedPtr->status();

        } else {

            // Peer entry will instead be deleted in delete routine,
            // doing it here undermines the const correctness of routine.
        }
    }

    // Compute base level status
    //TorrentPlugin::Status s = TorrentPlugin::status();

    // Return map of statuses
    return SellerTorrentPlugin::Status(_minPrice,
                                       _minLock,
                                       _minFeePerByte,
                                       _maxNumberOfSellers,
                                       _maxContractConfirmationDelay,
                                       _balance,
                                       peerStatuses);
}

// Creates configuratin for plugin
SellerTorrentPlugin::Configuration SellerTorrentPlugin::configuration() const {

    return SellerTorrentPlugin::Configuration(enableBanningSets(),
                                              _minPrice,
                                              _minLock,
                                              _minFeePerByte,
                                              _maxNumberOfSellers,
                                              _maxContractConfirmationDelay);
}

QList<libtorrent::tcp::endpoint> SellerTorrentPlugin::endPoints() const {
    return _peers.keys();
}

/**
const PeerPlugin * SellerTorrentPlugin::peerPlugin(const libtorrent::tcp::endpoint & endPoint) const {

    if(_peers.contains(endPoint)) {

        boost::shared_ptr<SellerPeerPlugin> p = _peers[endPoint];
        return p.get();

    }
        return NULL;
}
*/

/**
int SellerTorrentPlugin::disk_async_read_piece(SellerPeerPlugin * peer) {

    // Get torrent file
    libtorrent::torrent_info torrentFile = _torrent->torrent_file();

    // Get total number of pieces
    int totalNumberOfPieces = torrentFile.num_pieces();
    Q_ASSERT(peer->totalNumberOfPieces() == totalNumberOfPieces);

    // Get piece which should be read
    int piece = peer->lastRequestedFullPiece();

    // Get size of given piece
    int pieceSize = torrentFile.piece_size(piece);

    // Get block size
    libtorrent::torrent_status status;
    _torrent->status(&status, 0);
    int block_size = status.block_size;

    // Assert some piece invariants which caller should maintain
    Q_ASSERT(peer->completedAsyncReads().size() == 0);
    Q_ASSERT(peer->numberOfAsyncReadsCompleted() == 0);
    Q_ASSERT(peer->clientState() == SellerPeerPlugin::ClientState::reading_piece_from_disk);
    Q_ASSERT(piece >= 0);
    Q_ASSERT(piece < totalNumberOfPieces);

    // Calculate number of blocks in the piece
    int numberOfBlocksInPiece = qCeil(((double)pieceSize) / block_size);

    // Create space in peer for the future callbacks
    peer->setCompletedAsyncReads(QVector<libtorrent::disk_io_job>(numberOfBlocksInPiece));

    // Tell peer how large this piece is, is required
    peer->setLastRequestedFullPieceSize(pieceSize);

    // Generate one request per block
    libtorrent::piece_manager & manager = _torrent->filesystem();

    for(int block = 0;block < numberOfBlocksInPiece;block++) {

        // Build request
        libtorrent::peer_request request;
        request.piece = piece;
        request.start = block*block_size;

        // take into account that last block may be smaller
        request.length = (block < numberOfBlocksInPiece - 1) ? block_size : pieceSize - request.start;

        // Make request of piece manager
        manager.async_read(request, boost::bind(&SellerPeerPlugin::disk_async_read_handler, peer, block, _1, _2));

        qCDebug(_category) << "async_read block " << block;
    }

    return numberOfBlocksInPiece;
}
*/

/**
boost::shared_ptr<libtorrent::peer_plugin> SellerTorrentPlugin::peerPlugin(const libtorrent::tcp::endpoint & endPoint) const {

    if(_peers.contains(endPoint)) {
        return _peers[endPoint];
    else
        return boost::shared_ptr<libtorrent::peer_plugin>(NULL);

}
*/

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

quint64 SellerTorrentPlugin::minPrice() const {
    return _minPrice;
}

void SellerTorrentPlugin::setMinPrice(quint64 minPrice) {
    _minPrice = minPrice;
}

quint32 SellerTorrentPlugin::minLock() const {
    return _minLock;
}

void SellerTorrentPlugin::setMinLock(quint32 minLock) {
    _minLock = minLock;
}

quint64 SellerTorrentPlugin::minFeePerByte() const {
    return _minFeePerByte;
}

void SellerTorrentPlugin::setMinFeePerByte(quint64 minFeePerByte) {
    _minFeePerByte = minFeePerByte;
}

quint32 SellerTorrentPlugin::maxNumberOfSellers() const {
    return _maxNumberOfSellers;
}

void SellerTorrentPlugin::setMaxNumberOfSellers(quint32 minNumberOfSellers) {
    _maxNumberOfSellers = minNumberOfSellers;
}

quint32 SellerTorrentPlugin::maxContractConfirmationDelay() const {
    return _maxContractConfirmationDelay;
}

void SellerTorrentPlugin::setMaxContractConfirmationDelay(quint32 maxContractConfirmationDelay) {
    _maxContractConfirmationDelay = maxContractConfirmationDelay;
}

/**
// ** NEEDS TO BE ABSTRACTED TO PARENT CLASS **
int SellerTorrentPlugin::deleteAndDisconnectPeers() {

    // Deletion counter
    int count = 0;

    // Iterate peers
    for(QList<boost::weak_ptr<SellerPeerPlugin> >::iterator
        i = _peersScheduledForDeletion.begin(),
        end = _peersScheduledForDeletion.end();
        i != end;) {  // We do not uconditionally increment iterator (i++), since we may erase in loop

        boost::weak_ptr<SellerPeerPlugin> weakPtr = i.value();

        // Check if plugin object still exists
        if(boost::shared_ptr<SellerPeerPlugin> sharedPtr = weakPtr.lock()) {

            // Check if peer plugin is installed
            if(sharedPtr->scheduledForDeletingInNextTorrentPluginTick()) {

                // Disconnect connection
                sharedPtr->close_connection();

                // SEND ALERT, but notice that this peer may never actually have
                // been announced if it was never accepted in new_connection.

                // Delete plugin from map
                i = _peers.erase(i);

                // Count removal FROM MAP, object may have been deleted by libtorrent
                count++;
            } else
                i++;

        } else {

            // Delete plugin from map
            i = _peers.erase(i);

            // Count removal FROM MAP, object may have been deleted by libtorrent
            count++;
        }
    }

    return count;
}
*/

int SellerTorrentPlugin::deleteAndDisconnectPeers() {

    // Iterate peers and try to disconnect all which are still valid
    for(QList<boost::weak_ptr<SellerPeerPlugin> >::iterator
        i = _peersScheduledForDeletion.begin(),
        end = _peersScheduledForDeletion.end();
        i != end;i++) {

        // Get weak pointer
        boost::weak_ptr<SellerPeerPlugin> weakPtr = *i;

        // Check if plugin object still exists
        if(boost::shared_ptr<SellerPeerPlugin> sharedPtr = weakPtr.lock()) {

            Q_ASSERT(sharedPtr->scheduledForDeletingInNextTorrentPluginTick());

            // Disconnect connection
            sharedPtr->close_connection();
        }
    }

    // Get list count being cleared
    int count = _peersScheduledForDeletion.size();

    // Clear list
    _peersScheduledForDeletion.clear();

    // Return size
    return count;
}
