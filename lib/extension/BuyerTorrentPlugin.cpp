#include "BuyerTorrentPlugin.hpp"

/**
 * BuyerTorrentPlugin::Piece
 */

BuyerTorrentPlugin::Piece::Piece()
    : _index(-1) // safe value
    , _state(State::unassigned)
    , _peerPlugin(NULL) {
}

//BuyerTorrentPlugin::Piece::Piece(int index, int length, int numberOfBlocks, State state, BuyerPeerPlugin * peerPlugin)

BuyerTorrentPlugin::Piece::Piece(int index, State state, BuyerPeerPlugin * peerPlugin)
    : _index(index)
    //, _length(length)
    //, _numberOfBlocks(numberOfBlocks)
    , _state(state)
    , _peerPlugin(peerPlugin) {
}

int BuyerTorrentPlugin::Piece::index() const {
    return _index;
}

void BuyerTorrentPlugin::Piece::setIndex(int index) {
    _index = index;
}

/**
int BuyerTorrentPlugin::Piece::length() const {
    return _length;
}

void BuyerTorrentPlugin::Piece::setLength(int length) {
    _length = length;
}

int BuyerTorrentPlugin::Piece::numberOfBlocks() const {
    return _numberOfBlocks;
}

void BuyerTorrentPlugin::Piece::setNumberOfBlocks(int numberOfBlocks) {
    _numberOfBlocks = numberOfBlocks;
}
*/

BuyerTorrentPlugin::Piece::State BuyerTorrentPlugin::Piece::state() const {
    return _state;
}

void BuyerTorrentPlugin::Piece::setState(State state) {
    _state = state;
}

BuyerPeerPlugin * BuyerTorrentPlugin::Piece::peerPlugin() const {
    return _peerPlugin;
}

void BuyerTorrentPlugin::Piece::setPeerPlugin(BuyerPeerPlugin *peerPlugin) {
    _peerPlugin = peerPlugin;
}

/**
 * BuyerTorrentPlugin::Status
 */

BuyerTorrentPlugin::Status::Status() {
}

BuyerTorrentPlugin::Status::Status(State state,
                                   const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> & peers,
                                   const Payor::Status & payor)
    : _state(state)
    , _peerPluginStatuses(peers)
    , _payor(payor) {
}

BuyerTorrentPlugin::State BuyerTorrentPlugin::Status::state() const {
    return _state;
}

void BuyerTorrentPlugin::Status::setState(State state) {
    _state = state;
}

QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> BuyerTorrentPlugin::Status::peerPluginStatuses() const {
    return _peerPluginStatuses;
}

void BuyerTorrentPlugin::Status::setPeerPluginStatuses(const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> & peerPluginStatuses) {
    _peerPluginStatuses = peerPluginStatuses;
}

Payor::Status BuyerTorrentPlugin::Status::payor() const {
    return _payor;
}

void BuyerTorrentPlugin::Status::setPayor(const Payor::Status & payor) {
    _payor = payor;
}

/**
 * BuyerTorrentPlugin::Configuration
 */

#include <QLoggingCategory>

#include <libtorrent/entry.hpp>

BuyerTorrentPlugin::Configuration::Configuration() {

}

BuyerTorrentPlugin::Configuration::Configuration(bool enableBanningSets,
                                                 quint64 maxPrice,
                                                 quint32 maxLock,
                                                 quint64 maxFeePerByte,
                                                 quint32 numberOfSellers)
    : TorrentPlugin::Configuration(enableBanningSets)
    , _maxPrice(maxPrice)
    , _maxLock(maxLock)
    , _maxFeePerKb(maxFeePerByte)
    , _numberOfSellers(numberOfSellers) {
}

BuyerTorrentPlugin::Configuration::Configuration(const libtorrent::entry::dictionary_type & dictionaryEntry)
    : TorrentPlugin::Configuration(dictionaryEntry) {
    // IMPLEMENT LATER
}

void BuyerTorrentPlugin::Configuration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

    // Call super version
    //TorrentPlugin::Configuration::toDictionaryEntry(dictionaryEntry);

    // IMPLEMENT LATER
}

PluginMode BuyerTorrentPlugin::Configuration::pluginMode() const {
    return PluginMode::Buyer;
}

quint64 BuyerTorrentPlugin::Configuration::maxPrice() const {
    return _maxPrice;
}

void BuyerTorrentPlugin::Configuration::setMaxPrice(quint64 maxPrice) {
    _maxPrice = maxPrice;
}

quint32 BuyerTorrentPlugin::Configuration::maxLock() const{
    return _maxLock;
}

void BuyerTorrentPlugin::Configuration::setMaxLock(quint32 maxLock) {
    _maxLock = maxLock;
}

quint64 BuyerTorrentPlugin::Configuration::maxFeePerKb() const {
    return _maxFeePerKb;
}

void BuyerTorrentPlugin::Configuration::setMaxFeePerKb(quint64 maxFeePerByte) {
    _maxFeePerKb = maxFeePerByte;
}

quint32 BuyerTorrentPlugin::Configuration::numberOfSellers() const {
    return _numberOfSellers;
}

void BuyerTorrentPlugin::Configuration::setNumberOfSellers(quint32 numberOfSellers) {
    _numberOfSellers = numberOfSellers;
}

/**
 * BuyerTorrentPlugin
 */

#include "BuyerPeerPlugin.hpp"

#include "Message/Buy.hpp"
#include "Message/Sell.hpp"
#include "Message/Observe.hpp"
#include "Message/JoinContract.hpp"
#include "Message/SignRefund.hpp"
#include "Message/RequestFullPiece.hpp"
#include "Message/Ready.hpp"
#include "Message/Payment.hpp"

#include "Alert/BuyerTorrentPluginStatusAlert.hpp"
#include "Alert/BuyerPeerAddedAlert.hpp"
#include "Alert/BuyerPeerPluginRemovedAlert.hpp"

#include "BitCoin/BitSwaprjs.hpp"
#include "BitCoin/Wallet.hpp"
#include "BitCoin/UnspentP2PKHOutput.hpp"

//#include "extension/BitCoin/BitCoin.hpp"

#include "Plugin.hpp"

#include "extension/PeerPlugin.hpp" // PeerModeAnnounced

#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/socket_io.hpp> // print_endpoint

#include <QtMath> // QFloor
#include <math.h> // ceil

// Maximum allowable a peer may have in responding to given message (ms)
#define SIGN_REFUND_MAX_DELAY 5*1000

BuyerTorrentPlugin::BuyerTorrentPlugin(Plugin * plugin,
                                       const boost::shared_ptr<libtorrent::torrent> & torrent,
                                       Wallet * wallet,
                                       const BuyerTorrentPlugin::Configuration & configuration,
                                       const UnspentP2PKHOutput & utxo,
                                       QLoggingCategory & category)
    : TorrentPlugin(plugin, torrent, configuration, category)
    , _state(State::waiting_for_payor_to_be_ready)
    , _wallet(wallet)
    , _maxPrice(configuration.maxPrice())
    , _maxLock(configuration.maxLock())
    , _maxFeePerKb(configuration.maxFeePerKb())
    , _numberOfSellers(configuration.numberOfSellers())
    , _slotToPluginMapping(configuration.numberOfSellers(), NULL)
    , _numberOfUnassignedPieces(0)
    , _assignmentLowerBound(0) {

    // Get torrent info
    const libtorrent::torrent_info torrentInfo = _torrent->torrent_file();

    // Start clock for when picking sellers can begin
    _timeSincePluginStarted.start();

    /**
     * Setup payor configurations
     *
     * MOVE THIS TO SOME WHERE ELSE LATER, E.G. CTR OF PAYORCONFIGURATION
     */

    // Check that we found valid utxo
    Q_ASSERT(utxo.value() != 0);

    // Contract transaction fee is computed AS IF channel is full, change this later!!!
    quint64 txFee = Payor::contractFee(configuration.numberOfSellers(), _maxFeePerKb);

    // Allocate enough funds to buy full file from each seller at WORST price
    quint64 fundingPerSeller = torrentInfo.num_pieces()*_maxPrice;

    // Compute change
    quint64 changeValue = utxo.value() - txFee - configuration.numberOfSellers()*fundingPerSeller;

    // Generate keys in wallet
    QList<Wallet::Entry> buyerInContractKeys = _wallet->generateNewKeys(configuration.numberOfSellers(), Wallet::Purpose::BuyerInContractOutput).values();
    QList<Wallet::Entry> buyerFinalKeys = _wallet->generateNewKeys(configuration.numberOfSellers(), Wallet::Purpose::ContractFinal).values();
    QList<Wallet::Entry> changeKey = _wallet->generateNewKeys(1, Wallet::Purpose::ContractChange).values();

    Q_ASSERT(buyerInContractKeys.count() == configuration.numberOfSellers());
    Q_ASSERT(buyerFinalKeys.count() == configuration.numberOfSellers());
    Q_ASSERT(changeKey.count() == 1);

    // Create channel configurations
    QVector<Payor::Channel::Configuration> channelConfigurations;
    for(int i = 0;i < configuration.numberOfSellers();i++)
        channelConfigurations.append(Payor::Channel::Configuration(i,
                                                      Payor::Channel::State::unassigned,
                                                      0,
                                                      0,
                                                      fundingPerSeller,
                                                      buyerInContractKeys[i].keyPair(),
                                                      buyerFinalKeys[i].keyPair(),
                                                      PublicKey(),
                                                      PublicKey(),
                                                      Signature(),
                                                      Signature(),
                                                      0,
                                                      0,
                                                      0));
    // Payor configuration
    Payor::Configuration payorConfiguration(Payor::State::waiting_for_full_set_of_sellers,
                                            channelConfigurations,
                                            utxo,
                                            changeKey[0].keyPair(),
                                            changeValue,
                                            TxId(),
                                            0);
    // Set payor
    _payor = Payor(payorConfiguration);

    /**
     *  Setup pieces management data
     */

    // Get number of pieces
    int numberOfPieces = torrentInfo.num_pieces();

    for(int i = 0;i < numberOfPieces;i++) {

        // Do we already have the valid piece
        if(!_torrent->have_piece(i)) {

            // Add piece vector of pieces
            _pieces.push_back(Piece(i, Piece::State::unassigned, NULL));

            // Count piece as unassigned
            _numberOfUnassignedPieces++;

        } else // Add to piece vector of pieces, and indicate that we have it
            _pieces.push_back(Piece(i, Piece::State::fully_downloaded_and_valid, NULL));

    }

    /**
    // Get block size
    libtorrent::torrent_status torrentStatus;
    boost::uint32_t flags = libtorrent::torrent_handle::query_name +
                            libtorrent::torrent_handle::query_save_path +
                            libtorrent::torrent_handle::query_name +
                            libtorrent::torrent_handle::query_torrent_file;
                            //libtorrent::torrent_handle::query_accurate_download_counters +
                            //libtorrent::torrent_handle::query_verified_pieces +
                            //libtorrent::torrent_handle::query_pieces +
                            //libtorrent::torrent_handle::query_distributed_copies;

    _torrent->status(&torrentStatus, flags);
    _blockSize = torrentStatus.block_size;

    Old code using blockSize and length piece
    for(int i = 0;i < numberOfPieces;i++) {

        // Get byte size of given piece: all pieces, except possibly last one, are of same length
        int pieceSize = torrentInfo.piece_size(i);

        // Get number of blocks in given piece
        int numberOfBlocksInPiece = (int) ceil(((double)pieceSize) / _blockSize);

        // Do we already have the valid piece
        if(!_torrent->have_piece(i)) {

            // Add piece vector of pieces
            _pieces.push_back(Piece(i, pieceSize, numberOfBlocksInPiece, Piece::State::unassigned, NULL));

            // Count piece as unassigned
            _numberOfUnassignedPieces++;

        } else // Add to piece vector of pieces, and indicate that we have it
            _pieces.push_back(Piece(i, pieceSize, numberOfBlocksInPiece,Piece::State::fully_downloaded_and_valid, NULL));

    }
    */
}

boost::shared_ptr<libtorrent::peer_plugin> BuyerTorrentPlugin::new_connection(libtorrent::peer_connection * peerConnection) {

    /**
     * Libtorrent docs (http://libtorrent.org/reference-Plugins.html#peer_plugin):
     * The peer_connection will be valid as long as the shared_ptr is being held by the
     * torrent object. So, it is generally a good idea to not keep a shared_ptr to
     * your own peer_plugin. If you want to keep references to it, use weak_ptr.
     */

    /**
     * Note: You cannot disconnect this peer here, e.g. by using peer_connection::disconnect().
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
    libtorrent::bt_peer_connection * btConnection = static_cast<libtorrent::bt_peer_connection*>(peerConnection);

    // Create seller buyer peer plugin
    BuyerPeerPlugin * peerPlugin;

    // Check if this peer should be accepted, if not
    // a null is returned, hence plugin is not installed
    if(!TorrentPlugin::isPeerWellBehaved(peerConnection)) {

        qCDebug(_category) << "Peer is on ban list, scheduling peer for disconnection and plugin for deletion.";
        peerPlugin = new BuyerPeerPlugin(this, btConnection, true, _category);

    } else if(_peers.contains(endPoint)) {

        qCDebug(_category) << "Already added peer, scheduling peer for disconnection and plugin for deletion.";
        peerPlugin = new BuyerPeerPlugin(this, btConnection, true, _category);

    } else {

        qCDebug(_category) << "Installed buyer plugin #" << _peers.count() << endPointString.c_str();
        peerPlugin = new BuyerPeerPlugin(this, btConnection, false, _category);

        // Alert that peer was added
        sendTorrentPluginAlert(BuyerPeerAddedAlert(_torrent->info_hash(),
                                                   endPoint,
                                                   peerPlugin->status()));
    }

    // Create shared pointer
    boost::shared_ptr<BuyerPeerPlugin> sharedPeerPluginPtr(peerPlugin);

    // Add to collection
    if(peerPlugin->scheduledForDeletingInNextTorrentPluginTick())
        _peersScheduledForDeletion.append(sharedPeerPluginPtr);
    else
        _peers[endPoint] = boost::weak_ptr<BuyerPeerPlugin>(sharedPeerPluginPtr);

    // Return pointer to plugin as required
    return sharedPeerPluginPtr;
}

void BuyerTorrentPlugin::on_piece_pass(int index) {

    qCDebug(_category) << "on_piece_pass:" << index;

    // on_piece_pass() =>
    //Q_ASSERT(_state == State::downloading_pieces);
    if(_state != State::downloading_pieces) {
        qCDebug(_category) << "Ingoring piece from unkown source" << index;
        return;
    }

    // Get reference to piece
    Piece & piece = _pieces[index];

    Q_ASSERT(piece.state() == Piece::State::assigned);

    // Get peer
    BuyerPeerPlugin * peer = piece.peerPlugin();

    Q_ASSERT(peer->indexOfAssignedPiece() == index);
    Q_ASSERT(peer->clientState() == BuyerPeerPlugin::ClientState::waiting_for_libtorrent_to_validate_piece);
    Q_ASSERT(!_peerPluginsWithoutPieceAssignment.contains(peer));

    // Make a payment
    Signature paymentSignature = makePaymentAndGetPaymentSignature(peer);
    peer->sendExtendedMessage(Payment(paymentSignature));

    // Note payment
    _plugin->registerSentFunds(_payor.channels()[peer->payorSlot()].price());

    // Update peer
    peer->addDownloadedPiece(index); // Remember that piece was downloaded
    peer->setClientState(BuyerPeerPlugin::ClientState::needs_to_be_assigned_piece);

    _peerPluginsWithoutPieceAssignment.insert(peer); // Place back in idle peer pool

    // Get a new piece if one is available.
    // In the future, we should perhaps delay this decision, and let
    // tick() in torretn plugin decide, since it has information about down speeds.
    assignPieceToPeerPlugin(peer);

    // Update piece
    piece.setState(Piece::State::fully_downloaded_and_valid);
    piece.setPeerPlugin(NULL); // for safety
}

void BuyerTorrentPlugin::on_piece_failed(int index) {
    //qCDebug(_category) << "on_piece_failed" << index;
    throw std::exception("BuyerTorrentPlugin::on_piece_failed");
}

void BuyerTorrentPlugin::tick() {

    //qCDebug(_category) << "BuyerTorrentPlugin.tick()";

    // If we are downloading, then try to assign them to peers without pieces assigned
    if(_state == State::downloading_pieces) {

        // While there are unassigned pieces and peers without a piece, try to assign
        while(_numberOfUnassignedPieces > 0 &&
              !_peerPluginsWithoutPieceAssignment.empty()) {

            qCDebug(_category) << "Assigning piece to peer.";

            // Get the "first" peer plugin in set, but doe not remove
            BuyerPeerPlugin * peerPlugin = *(_peerPluginsWithoutPieceAssignment.begin());

            Q_ASSERT(peerPlugin->clientState() == BuyerPeerPlugin::ClientState::needs_to_be_assigned_piece);

            // Assign to peer plugin
            bool assigned = assignPieceToPeerPlugin(peerPlugin);

            // loop invariant =>
            Q_ASSERT(assigned);
        }
    }

    // Delete peer plugins
    int toBeDeleted = _peersScheduledForDeletion.size();
    int actuallyDeleted = deleteAndDisconnectPeers();

    Q_ASSERT(toBeDeleted == actuallyDeleted);
    Q_ASSERT(_peersScheduledForDeletion.empty());

    if(actuallyDeleted > 0)
        qCDebug(_category) << "Disconnected and deleted" << actuallyDeleted << "peer plugins.";


    // Send status update to controller
    sendTorrentPluginAlert(BuyerTorrentPluginStatusAlert(_torrent->info_hash(), status()));
}

bool BuyerTorrentPlugin::on_resume() {
    return false;
}

bool BuyerTorrentPlugin::on_pause() {
    return false;
}

void BuyerTorrentPlugin::on_files_checked() {

}

void BuyerTorrentPlugin::on_state(int s) {

}

void BuyerTorrentPlugin::on_add_peer(const libtorrent::tcp::endpoint & endPoint, int src, int flags) {

}

/**
bool BuyerTorrentPlugin::inviteSeller(quint32 minPrice, quint32 minLock) const {

    // Check that we are
    // 1) Still building payment channel
    // 2) Price is low enough
    // 3) nLockTime is short enough
    return _state == State::waiting_for_payor_to_be_ready &&
            minPrice <= _maxPrice &&
            minLock <= _maxLock;
}
*/

bool BuyerTorrentPlugin::sellerWantsToJoinContract(BuyerPeerPlugin * peer, quint64 price, quint32 refundLockTime, const PublicKey & contractPk, const PublicKey & finalPk) {

    // Check payor is trying to find sellers
    if(_payor.state() != Payor::State::waiting_for_full_set_of_sellers) {

        // Update peer plugin client state
        peer->setClientState(BuyerPeerPlugin::ClientState::ignored_join_contract_from_peer);

        qWarning(_category) << "Ignoring joining_contract message from this peer.";

        return false;
    }

    // _payor.state() == Payor::State::waiting_for_full_set_of_sellers =>
    Q_ASSERT(_state == State::waiting_for_payor_to_be_ready);

    // Try to assign slot in payment channel
    quint32 slot = _payor.assignUnassignedSlot(price, contractPk, finalPk, refundLockTime);

    // Unassigned slots should always have null pointer
    Q_ASSERT(_slotToPluginMapping[slot] == NULL);

    // Save reference to peer for given slot
    _slotToPluginMapping[slot] = peer;

    // Save slot membership in peer plugin.
    peer->setPayorSlot(slot);

    // If this was the last slot, then we have a full set,
    // start asking for signatures
    if(_payor.isFull()) {

        qDebug(_category) << "Contract was filled, sending sign_refund messages to sellers.";

        /**
         * For each contract channel,
         * grab value and payor final pk,
         * and send sign_refund message
         */

        // Get channels
        const QVector<Payor::Channel> & channels = _payor.channels();

        // Iterate
        quint32 index = 0;
        const TxId & contractHash = _payor.contractHash();
        for(QVector<Payor::Channel>::const_iterator i = channels.constBegin(),
            end(channels.constEnd()); i != end;i++, index++) {

            // Get channel
            const Payor::Channel & channel = *i;

            // isContractFull() =>
            Q_ASSERT(channel.state() == Payor::Channel::State::assigned);

            // Create sign_refund message
            SignRefund m(contractHash, index, channel.funds(), channel.payorContractKeyPair().pk(), channel.payorFinalKeyPair().pk());

            // Get corresponding buyer peer plugin
            BuyerPeerPlugin * channelPeer = _slotToPluginMapping[index];

            // channel.state() == Payor::Channel::State::assigned
            // and
            // sign_refund message not yet sent
            // =>
            Q_ASSERT(channelPeer->clientState() == BuyerPeerPlugin::ClientState::invited_to_contract);
            //Q_ASSERT(channelPeer->peerState().lastAction() == BuyerPeerPlugin::PeerState::LastValidAction::joined_contract);
            Q_ASSERT(channelPeer->peerState().failureMode() == BuyerPeerPlugin::PeerState::FailureMode::not_failed);

            // Have plugin send message
            channelPeer->sendExtendedMessage(m);

            // Update peer plugin client state
            channelPeer->setClientState(BuyerPeerPlugin::ClientState::asked_for_refund_signature);
        }

        // Update payor state
        _payor.setState(Payor::State::waiting_for_full_set_of_refund_signatures);
    }

    return true;
}

bool BuyerTorrentPlugin::sellerProvidedRefundSignature(BuyerPeerPlugin * peer, const Signature & refundSignature) {

    // call conditions =>
    Q_ASSERT(_state == State::waiting_for_payor_to_be_ready);
    Q_ASSERT(peer->clientState() == BuyerPeerPlugin::ClientState::asked_for_refund_signature);
    Q_ASSERT(_peerPluginsWithoutPieceAssignment.empty());

    // Check that signature is valid
    bool wasValid = _payor.processRefundSignature(peer->payorSlot(), refundSignature);

    // Return that signature was invalid
    if(!wasValid) {

        Q_ASSERT(false); // should not happen now

        return false;
    } else
        peer->setClientState(BuyerPeerPlugin::ClientState::received_valid_refund_signature_and_waiting_for_others);

    // Check if we have all signatures
    if(_payor.allRefundsSigned()) {

        // Construct and broadcast contract
        //_payor.broadcast_contract();

        // Register tx fee we are spending
        _plugin->registerSentFunds(_payor.contractFee(_numberOfSellers, _maxFeePerKb));

        qCDebug(_category) << "Broadcasting contract, txId:" << _payor.contractHash().toString();

        // Tell all peers with ready message
        for(QVector<BuyerPeerPlugin *>::iterator i = _slotToPluginMapping.begin(),
            end(_slotToPluginMapping.end()); i != end;i++) {

            // Get pointer to plugin
            BuyerPeerPlugin * p = *i;

            // _payor.allRefundSigned =>
            Q_ASSERT(p->clientState() == BuyerPeerPlugin::ClientState::received_valid_refund_signature_and_waiting_for_others);
            //Q_ASSERT(p->peerState().lastAction() == BuyerPeerPlugin::PeerState::LastValidAction::signed_refund);

            // Send ready message
            p->sendExtendedMessage(Ready());

            // Update client state
            //p->setClientState(BuyerPeerPlugin::ClientState::announced_ready);
            p->setClientState(BuyerPeerPlugin::ClientState::needs_to_be_assigned_piece);

            // Note that peer has not been assigned a piece
            _peerPluginsWithoutPieceAssignment.insert(p);

            // Register funds locked in this channel
            _plugin->registerLockedInChannelsFunds(_payor.channels()[peer->payorSlot()].funds());
        }

        // Update state
        _state = State::downloading_pieces;

    } else // note that we ignored responding for now since we have to wait for others
        peer->setClientState(BuyerPeerPlugin::ClientState::received_valid_refund_signature_and_waiting_for_others);

    // Return tht signature was valid
    return true;
}

bool BuyerTorrentPlugin::assignPieceToPeerPlugin(BuyerPeerPlugin * peerPlugin) {

    // This routine should only be called if following holds
    Q_ASSERT(_state == State::downloading_pieces);
    Q_ASSERT(peerPlugin->clientState() == BuyerPeerPlugin::ClientState::needs_to_be_assigned_piece);
    Q_ASSERT(_peerPluginsWithoutPieceAssignment.contains(peerPlugin));
    //Q_ASSERT(peerPlugin->unservicedRequests().empty());

    // Try to get next piece
    int pieceIndex;

    try {
        pieceIndex = getNextUnassignedPiece(_assignmentLowerBound);
    } catch (std::exception & e) {
        // and signal failure
        return false;
    }

     // Update piece object
     Piece & piece = _pieces[pieceIndex];

     // getNextUnassignedPiece() correctness requires that
     // piece returned must have proper state
     Q_ASSERT(piece.index() == pieceIndex);
     Q_ASSERT(piece.state() == Piece::State::unassigned);

     piece.setState(Piece::State::assigned);
     piece.setPeerPlugin(peerPlugin);

     // Send request
     peerPlugin->sendExtendedMessage(RequestFullPiece(pieceIndex));

     // Update peer plugin
     peerPlugin->setClientState(BuyerPeerPlugin::ClientState::waiting_for_full_piece);
     peerPlugin->setIndexOfAssignedPiece(pieceIndex);

     // Remove from set of unassigned peer plugins, if its there
     _peerPluginsWithoutPieceAssignment.remove(peerPlugin);

     // Reduced the number of unassigned pieces
     _numberOfUnassignedPieces--;

     // Should never go negative
     Q_ASSERT(_numberOfUnassignedPieces >= 0);

     // Was able to assign piece
     return true;
 }

int BuyerTorrentPlugin::getNextUnassignedPiece(int startIndex) const {

     // Start looking for piece at given starting point index
     int index = startIndex;

     // Find first unasigned piece with index no less than startIndex
     for(;index < _pieces.length();index++) {

         // Get piece
         const Piece & piece = _pieces[index];

         // Return index if piece is unassigned
         if(piece.state() == Piece::State::unassigned)
             return index;

     }

     // No match found, lets start from beginning then
     for(index = 0;index < startIndex;index++) {

         // Get piece
         const Piece & piece = _pieces[index];

         // Return index if piece is unassigned
         if(piece.state() == Piece::State::unassigned)
             return index;

     }

     // We did not find anything
     throw std::exception("Unable to find any unassigned pieces.");
 }

Signature BuyerTorrentPlugin::makePaymentAndGetPaymentSignature(BuyerPeerPlugin * peerPlugin) {

    // This routine should only be called if following holds
    Q_ASSERT(_state == State::downloading_pieces);

    // Find payor slot for this plugin
    quint32 payorSlot = peerPlugin->payorSlot();

    // Note payment
    _payor.incrementPaymentCounter(payorSlot);

    // Get payment signature for given slot
    return _payor.getPresentPaymentSignature(payorSlot);
}

void BuyerTorrentPlugin::fullPieceArrived(BuyerPeerPlugin * peer, const boost::shared_array<char> & piece, int length) {

    // This routine should only be called if following holds
    Q_ASSERT(_state == State::downloading_pieces);

    // Get what piece has been assigned to this peer
    int pieceIndex = peer->indexOfAssignedPiece();

    // Check that piece has correct length
    if(length != _torrent->torrent_file().piece_size(pieceIndex))
        throw std::exception("Full piece message had invalid length.");

    // Tell libtorrent to validate piece
    // last argument is a flag which presently seems to only test
    // flags & torrent::overwrite_existing, which seems to be whether
    // the piece should be overwritten if it is already present
    //
    // libtorrent::torrent_plugin::on_piece_pass()
    // libtorrent::torrent_plugin::on_piece_failed()
    // processes result of checking
    _torrent->add_piece(pieceIndex, piece.get(), 0);

    // Update client state
    peer->setClientState(BuyerPeerPlugin::ClientState::waiting_for_libtorrent_to_validate_piece);
}

quint64 BuyerTorrentPlugin::totalSentSinceStart() const {

    // Iterate channels in payor and count total amount sent
    int numberOfChannels = _payor.numberOfChannels();

    // Add tx fee of contract itself
    quint64 total = _payor.contractFee(numberOfChannels, _maxFeePerKb);

    for(int i = 0; i < numberOfChannels;i++) {

        // Get channel
        const Payor::Channel & channel = _payor.channel(i);

        // Count how much was paid to channel
        total += channel.numberOfPaymentsMade() * channel.price();
    }

    return total;
}

quint64 BuyerTorrentPlugin::channelBalance(int i) const {

    Q_ASSERT(i < _payor.numberOfChannels());

    const Payor::Channel & c = _payor.channel(i);

    return c.price() * c.numberOfPaymentsMade();
}

/**
quint64 BuyerTorrentPlugin::totalCurrentlyLockedInChannels() const {

    // Iterate channels in payor and count total amount sent
    int numberOfChannels = _payor.numberOfChannels();
    quint64 total = 0;

    for(int i = 0; i < numberOfChannels;i++) {

        // Get channel
        const Payor::Channel & channel = _payor.channel(i);

        // Count how much is locked up
        total += channel.funds();
    }

    return total;
}
*/

/**
bool BuyerTorrentPlugin::removePluginIfInPeersMap(const libtorrent::tcp::endpoint & endPoint) {
    return _peers.remove(endPoint) == 1;
}
*/
void BuyerTorrentPlugin::on_peer_plugin_disconnect(BuyerPeerPlugin * peerPlugin, libtorrent::error_code const & ec) {

    // Get endpoint
    libtorrent::tcp::endpoint endPoint = peerPlugin->endPoint();

    qCDebug(_category) << "on_disconnect ["<< (peerPlugin->connection()->is_outgoing() ? "outgoing" : "incoming") << "]:" << ec.message().c_str();

    if(!peerPlugin->scheduledForDeletingInNextTorrentPluginTick()) {

        // Scheduled for deletion <=> must NOT be in peers map
        Q_ASSERT(_peers.contains(endPoint));

        // Remove peer plugin from map
        boost::weak_ptr<BuyerPeerPlugin> weakPtr = _peers.take(endPoint);

        // ASSERT: is same plugin
        Q_ASSERT(!weakPtr.expired());
        boost::shared_ptr<BuyerPeerPlugin> sharedPtr = weakPtr.lock();
        Q_ASSERT(sharedPtr.get() == peerPlugin);

        // Schedule for prompt deletion
        peerPlugin->setScheduledForDeletingInNextTorrentPluginTick(true);

        // Save error_code which
        peerPlugin->setDeletionErrorCode(ec);

        // Place in deletion list
        _peersScheduledForDeletion.append(weakPtr);

        // Send alert for peer plugin removal
        sendTorrentPluginAlert(BuyerPeerPluginRemovedAlert(_torrent->info_hash(), sharedPtr->endPoint()));

    } else {

        /**
         * SLOW ASSERT:
         * _peers does not contain peer pluing
         */
        if(_peers.contains(endPoint)) {

            boost::weak_ptr<BuyerPeerPlugin> weakPtr = _peers[endPoint];

            // ASSERT: is NOT same plugin
            Q_ASSERT(!weakPtr.expired());
            boost::shared_ptr<BuyerPeerPlugin> sharedPtr = weakPtr.lock();
            Q_ASSERT(sharedPtr.get() != peerPlugin);

        }

        /**
         * SLOW ASSERT:
         * _peersScheduledForDeletion does contain peer plugin
         */
        bool present = false;
        for(QList<boost::weak_ptr<BuyerPeerPlugin> >::const_iterator
            i = _peersScheduledForDeletion.constBegin(),
            end = _peersScheduledForDeletion.constEnd();
            i != end;i++) {

            // Get weak ptr
            const boost::weak_ptr<BuyerPeerPlugin> weakPtr = *i;

            if(!weakPtr.expired()) {

                const boost::shared_ptr<BuyerPeerPlugin> sharedPtr = weakPtr.lock();

                if(sharedPtr.get() == peerPlugin) {
                    present = true;
                    break;
                }
            }
        }

        Q_ASSERT(present);
    }

}

BuyerTorrentPlugin::Status BuyerTorrentPlugin::status() const {

    // Create map for peer plugin statuses
    QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> peerPluginStatuses;

    for(QMap<libtorrent::tcp::endpoint, boost::weak_ptr<BuyerPeerPlugin> >::const_iterator
        i = _peers.constBegin(),
        end = _peers.constEnd();
        i != end;i++) {

        // Get peer
        boost::weak_ptr<BuyerPeerPlugin> weakPtr = i.value();

        // Make sure peer plugin is still valid
        if(const boost::shared_ptr<BuyerPeerPlugin> sharedPtr = weakPtr.lock()) {

            // Compute status and save in map
            peerPluginStatuses[i.key()] = sharedPtr->status();

        } else {

            // Peer entry will instead be deleted in delete routine,
            // doing it here undermines the const correctness of routine.
        }
    }

    // Compute base level status
    //TorrentPlugin::Status s = TorrentPlugin::status();

    // Return final status
    return Status(_state,
                  peerPluginStatuses,
                  _payor.status());
 }

 /**
 boost::shared_ptr<libtorrent::peer_plugin> BuyerTorrentPlugin::peerPlugin(const libtorrent::tcp::endpoint & endPoint) const {

     if(_peers.contains(endPoint)) {
         return _peers[endPoint];
     else
         return boost::shared_ptr<libtorrent::peer_plugin>(NULL);

 }

 void BuyerTorrentPlugin::removePeerPlugin(PeerPlugin * plugin) {

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

 BuyerTorrentPlugin::State BuyerTorrentPlugin::state() const {
     return _state;
 }

 void BuyerTorrentPlugin::setState(const State & state) {
     _state = state;
 }

 quint64 BuyerTorrentPlugin::maxPrice() const {
     return _maxPrice;
 }

 void BuyerTorrentPlugin::setMaxPrice(quint64 maxPrice) {
     _maxPrice = maxPrice;
 }

 quint32 BuyerTorrentPlugin::maxLock() const {
     return _maxLock;
 }

 void BuyerTorrentPlugin::setMaxLock(quint32 maxLock) {
     _maxLock = maxLock;
 }

 quint64 BuyerTorrentPlugin::maxFeePerKb() const {
     return _maxFeePerKb;
 }

 void BuyerTorrentPlugin::setMaxFeePerKb(quint64 maxFeePerByte) {
     _maxFeePerKb = maxFeePerByte;
 }

 quint32 BuyerTorrentPlugin::numberOfSellers() const {
     return _numberOfSellers;
 }

 void BuyerTorrentPlugin::setnumberOfSellers(quint32 numberOfSellers) {
    _numberOfSellers = numberOfSellers;
 }

 int BuyerTorrentPlugin::assignmentLowerBound() const {
     return _assignmentLowerBound;
 }

 void BuyerTorrentPlugin::setAssignmentLowerBound(int assignmentLowerBound) {
     _assignmentLowerBound = assignmentLowerBound;
 }


 /**
 int BuyerTorrentPlugin::blockSize() const {
     return _blockSize;
 }

 void BuyerTorrentPlugin::setBlockSize(int blockSize) {
     _blockSize = blockSize;
 }

 const Payor & BuyerTorrentPlugin::payor() const {
     return _payor;
 }
 */

 PluginMode BuyerTorrentPlugin::pluginMode() const {
     return PluginMode::Buyer;
 }

 QList<libtorrent::tcp::endpoint> BuyerTorrentPlugin::endPoints() const {
     return _peers.keys();
 }

 /**
 const PeerPlugin * BuyerTorrentPlugin::peerPlugin(const libtorrent::tcp::endpoint & endPoint) const {

     if(_peers.contains(endPoint)) {

         boost::shared_ptr<BuyerPeerPlugin> p = _peers[endPoint];
         return p.get();

     }
         return NULL;
 }
*/

 /**
 // ** NEEDS TO BE ABSTRACTED TO PARENT CLASS **
 int BuyerTorrentPlugin::deleteAndDisconnectPeers() {

     // Deletion counter
     int count = 0;

     // Iterate peers
     for(QMap<libtorrent::tcp::endpoint, boost::weak_ptr<BuyerPeerPlugin> >::iterator
         i = _peers.begin(),
         end = _peers.end();
         i != end;) { // We do not uconditionally increment iterator (i++), since we may erase in loop

         // Get pointer
         boost::weak_ptr<BuyerPeerPlugin> weakPtr = i.value();

         // Check if plugin object still exists
         if(boost::shared_ptr<BuyerPeerPlugin> sharedPtr = weakPtr.lock()) {

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


 int BuyerTorrentPlugin::deleteAndDisconnectPeers() {

     // Iterate peers and try to disconnect all which are still valid
     for(QList<boost::weak_ptr<BuyerPeerPlugin> >::iterator
         i = _peersScheduledForDeletion.begin(),
         end = _peersScheduledForDeletion.end();
         i != end;i++) {

         // Get weak pointer
         boost::weak_ptr<BuyerPeerPlugin> weakPtr = *i;

         // Check if plugin object still exists
         if(boost::shared_ptr<BuyerPeerPlugin> sharedPtr = weakPtr.lock()) {

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
