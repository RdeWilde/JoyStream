#include "BuyerTorrentPlugin.hpp"

/**
 * BuyerTorrentPlugin::Piece
 */

BuyerTorrentPlugin::Piece::Piece()
    : _index(-1) // safe value
    , _state(State::unassigned)
    , _peerPlugin(NULL) {
}

BuyerTorrentPlugin::Piece::Piece(int index, int length, int numberOfBlocks, State state, BuyerPeerPlugin * peerPlugin)
    : _index(index)
    , _length(length)
    , _numberOfBlocks(numberOfBlocks)
    , _state(state)
    , _peerPlugin(peerPlugin) {
}

int BuyerTorrentPlugin::Piece::index() const {
    return _index;
}

void BuyerTorrentPlugin::Piece::setIndex(int index) {
    _index = index;
}

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

BuyerTorrentPlugin::Piece::State BuyerTorrentPlugin::Piece::state() const {
    return _state;
}

void BuyerTorrentPlugin::Piece::setState(const State & state) {
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

BuyerTorrentPlugin::Status::Status(State state,
                                   const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> & peers,
                                   const Payor::Status & payor)
    : _state(state)
    , _peers(peers)
    , _payor(payor) {
}

BuyerTorrentPlugin::State BuyerTorrentPlugin::Status::state() const {
    return _state;
}

void BuyerTorrentPlugin::Status::setState(State state) {
    _state = state;
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
    , _maxFeePerByte(maxFeePerByte)
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

quint64 BuyerTorrentPlugin::Configuration::maxFeePerByte() const {
    return _maxFeePerByte;
}

void BuyerTorrentPlugin::Configuration::setMaxFeePerByte(quint64 maxFeePerByte) {
    _maxFeePerByte = maxFeePerByte;
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
#include "Message/Ready.hpp"

#include "Alert/BuyerTorrentPluginStatuAlert.hpp"

#include "BitCoin/BitSwaprjs.hpp"
#include "BitCoin/Wallet.hpp"
#include "BitCoin/UnspentP2PKHOutput.hpp"

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
    , _maxFeePerByte(configuration.maxFeePerByte())
    , _numberOfSellers(configuration.numberOfSellers())
    , _slotToPluginMapping(configuration.numberOfSellers(), NULL)
    , _numberOfUnassignedPieces(0)
    , _assignmentLowerBound(0) {

    // Get torrent info
    const libtorrent::torrent_info & torrentInfo = _torrent->torrent_file();

    // Start clock for when picking sellers can begin
    _timeSincePluginStarted.start();

    /**
     * Setup payor configurations
     *
     * MOVE THIS TO SOME WHERE ELSE LATER, E.G. CTR OF PAYORCONFIGURATION
     */

    // Check that we found valid utxo
    if(utxo.fundingValue() == 0)
        throw std::exception("No utxo found.");

    // Figure out how much to lock up with each seller
    quint64 fundingPerSeller = qFloor(utxo.fundingValue()/configuration.numberOfSellers());

    // Contract transaction fee: Get from where?
    quint64 tx_fee = 0;

    // Compute change
    quint64 changeValue = utxo.fundingValue() - configuration.numberOfSellers()*fundingPerSeller - tx_fee;

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
                                            utxo.fundingOutput(),
                                            utxo.fundingOutputKeyPair(),
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
}

boost::shared_ptr<libtorrent::peer_plugin> BuyerTorrentPlugin::new_connection(libtorrent::peer_connection * connection) {

    /**
     * Libtorrent docs (http://libtorrent.org/reference-Plugins.html#peer_plugin):
     * The peer_connection will be valid as long as the shared_ptr is being held by the
     * torrent object. So, it is generally a good idea to not keep a shared_ptr to
     * your own peer_plugin. If you want to keep references to it, use weak_ptr.
     */

    // Get end point to look up sets
    const libtorrent::tcp::endpoint & endPoint = connection->remote();
    std::string endPointString = libtorrent::print_endpoint(endPoint);

    qCDebug(_category) << "New connection from" << endPointString.c_str();

    // Check if this peer should be accepted, if not
    // a null is returned, hence plugin is not installed
    if(!TorrentPlugin::isPeerWellBehaved(connection)) {
        qCDebug(_category) << "Rejected connection from peer, peer plugin not installed.";
        return boost::shared_ptr<libtorrent::peer_plugin>();
    }

    // Create bittorrent peer connection
    libtorrent::bt_peer_connection * btConnection = static_cast<libtorrent::bt_peer_connection*>(connection);

    // Create seller buyer peer plugin
    boost::shared_ptr<BuyerPeerPlugin> sharedPeerPluginPtr(new BuyerPeerPlugin(this,
                                                                           btConnection,
                                                                           _category));

    // Add to collection
    _peers[endPoint] = sharedPeerPluginPtr;

    qCDebug(_category) << "Buyer #" << _peers.count() << endPointString.c_str() << "added.";

    // Return pointer to plugin as required
    return sharedPeerPluginPtr;
}

void BuyerTorrentPlugin::on_piece_pass(int index) {

}

void BuyerTorrentPlugin::on_piece_failed(int index) {

}

void BuyerTorrentPlugin::tick() {

    qCDebug(_category) << "BuyerTorrentPlugin.tick()";

    // If we are downloading, then try to assign them to peers without pieces assigned
    if(_state == State::downloading_pieces) {

        qCDebug(_category) << "Trying to assign pieces to peers.";

        // Iterate peers while there are unassigned pieces left
        for(QSet<BuyerPeerPlugin *>::const_iterator i = _peerPluginsWithoutPieceAssignment.constBegin(),
            end(_peerPluginsWithoutPieceAssignment.constEnd());
            i != end && _numberOfUnassignedPieces > 0;i++) {

            // Get peer plugin
            BuyerPeerPlugin * peerPlugin = *i;

            // Check if peer is giving us a decent down speed?

            // Assign to peer plugin
            assignPieceToPeerPlugin(peerPlugin);
        }
    }

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

bool BuyerTorrentPlugin::inviteSeller(quint32 minPrice, quint32 minLock) const {

    // Check that we are
    // 1) Still building payment channel
    // 2) Price is low enough
    // 3) nLockTime is short enough
    return _state == State::waiting_for_payor_to_be_ready &&
            minPrice <= _maxPrice &&
            minLock <= _maxLock;
}

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

    // Check that signature is valid
    bool wasValid = _payor.processRefundSignature(peer->payorSlot(), refundSignature);

    // Return that signature was invalid
    if(!wasValid)
        return false;
    else
        peer->setClientState(BuyerPeerPlugin::ClientState::received_valid_refund_signature_and_waiting_for_others);

    // Check if we have all signatures
    if(_payor.allRefundsSigned()) {

        // Construct and broadcast contract
        _payor.broadcast_contract();

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

        // No unassigned pieces presently available, so add to set of unassigne peer plugins
        _peerPluginsWithoutPieceAssignment.insert(peerPlugin);

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

     // Update peer plugin
     //peerPlugin->setAssignedPiece(true);
     peerPlugin->setIndexOfAssignedPiece(pieceIndex);
     //peerPlugin->setPieceSize(piece.length());
     //peerPlugin->setBlockSize(_blockSize); // Does not change from piece to piece, put in peer configuration or something later
     //peerPlugin->setNumberOfBlocksInPiece(piece.numberOfBlocks());
     //peerPlugin->setNumberOfBlocksRequested(0);
     //peerPlugin->setNumberOfBlocksReceived(0);
     //instead: peerPlugin->assignPiece(piece);

     // Remove from set of unassigned peer plugins, if its there
     _peerPluginsWithoutPieceAssignment.remove(peerPlugin);

     // Reduced the number of unassigned pieces
     _numberOfUnassignedPieces--;

     // Should never go negative
     Q_ASSERT(_numberOfUnassignedPieces >= 0);
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

void BuyerTorrentPlugin::pieceDownloaded(int index) {

    // Check that index is valid
    Q_ASSERT(index < _pieces.length());

    // Get piece
    Piece & piece = _pieces[index];

    // Note that peer plugin has no piece at present
    _peerPluginsWithoutPieceAssignment.insert(piece.peerPlugin());

    // Update state
    piece.setState(Piece::State::fully_downloaded_and_valid);
    piece.setPeerPlugin(NULL); // for safety
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

bool BuyerTorrentPlugin::checkLengthAndValidatePiece(int pieceIndex, const QVector<char> & pieceData) {

    // This routine should only be called if following holds
    Q_ASSERT(_state == State::downloading_pieces);

    // Check that piece has correct length
    if(pieceData.size() != _torrent->torrent_file().piece_size(pieceIndex))
        return false;

    // Tell libtorrent to validate piece
    // last argument is a flag which presently seems to only test
    // flags & torrent::overwrite_existing, which seems to be whether
    // the piece should be overwritten if it is already present
    _torrent->add_piece(pieceIndex, pieceData.data(), 0);

    return true;
}

BuyerTorrentPlugin::Status BuyerTorrentPlugin::status() const {

     // Build list of buyer peer statuses
     QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> peers;

     for(QMap<libtorrent::tcp::endpoint, boost::shared_ptr<BuyerPeerPlugin> >::const_iterator i = _peers.constBegin(),
             end(_peers.constEnd());i != end;i++)
         peers[i.key()] = (i.value())->status();

     // Return final status
     return Status(_state, peers, _payor.status());
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

 quint64 BuyerTorrentPlugin::maxFeePerByte() const {
     return _maxFeePerByte;
 }

 void BuyerTorrentPlugin::setMaxFeePerByte(quint64 maxFeePerByte) {
     _maxFeePerByte = maxFeePerByte;
 }

 quint32 BuyerTorrentPlugin::numberOfSellers() const {
     return _numberOfSellers;
 }

 void BuyerTorrentPlugin::setnumberOfSellers(quint32 numberOfSellers) {
    _numberOfSellers = numberOfSellers;
 }

 int BuyerTorrentPlugin::blockSize() const {
     return _blockSize;
 }

 void BuyerTorrentPlugin::setBlockSize(int blockSize) {
     _blockSize = blockSize;
 }


 /*
 const Payor & BuyerTorrentPlugin::payor() const {
     return _payor;
 }
 */

 PluginMode BuyerTorrentPlugin::pluginMode() const {
     return PluginMode::Buyer;
 }

