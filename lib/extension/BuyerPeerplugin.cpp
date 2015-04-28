#include "BuyerPeerPlugin.hpp"

/**
 * BuyerPeerPlugin::PeerState
 */

BuyerPeerPlugin::PeerState::PeerState()
    : //_lastAction(LastValidAction::no_bitswapr_message_sent)
     _failureMode(FailureMode::not_failed) {
}

BuyerPeerPlugin::PeerState::PeerState(//LastValidAction lastAction,
                                        FailureMode failureMode,
                                        const Sell & lastSellReceived,
                                        const JoiningContract & lastJoiningContractReceived)
    : //_lastAction(lastAction)
      _failureMode(failureMode)
    , _lastSellReceived(lastSellReceived)
    , _lastJoiningContractReceived(lastJoiningContractReceived) {
}

/*
BuyerPeerPlugin::PeerState::LastValidAction BuyerPeerPlugin::PeerState::lastAction() const {
    return _lastAction;
}

void BuyerPeerPlugin::PeerState::setLastAction(LastValidAction lastAction) {
    _lastAction = lastAction;
}
*/

BuyerPeerPlugin::PeerState::FailureMode BuyerPeerPlugin::PeerState::failureMode() const {
    return _failureMode;
}

void BuyerPeerPlugin::PeerState::setFailureMode(FailureMode failureMode) {
    _failureMode = failureMode;
}

JoiningContract BuyerPeerPlugin::PeerState::lastJoiningContractReceived() const {
    return _lastJoiningContractReceived;
}

void BuyerPeerPlugin::PeerState::setLastJoiningContractReceived(const JoiningContract &lastJoiningContractReceived) {
    _lastJoiningContractReceived = lastJoiningContractReceived;
}
/**
quint64 BuyerPeerPlugin::PeerState::minPrice() const {
    return _minPrice;
}

void BuyerPeerPlugin::PeerState::setMinPrice(quint64 minPrice) {
    _minPrice = minPrice;
}

PublicKey BuyerPeerPlugin::PeerState::contractPk() const {
    return _contractPk;
}

void BuyerPeerPlugin::PeerState::setContractPk(const PublicKey & contractPk) {
    _contractPk = contractPk;
}

PublicKey BuyerPeerPlugin::PeerState::finalPk() const {
    return _finalPk;
}

void BuyerPeerPlugin::PeerState::setFinalPk(const PublicKey & finalPk) {
    _finalPk = finalPk;
}

quint32 BuyerPeerPlugin::PeerState::minLock() const {
    return _minLock;
}

void BuyerPeerPlugin::PeerState::setMinLock(quint32 minLock) {
    _minLock = minLock;
}
*/

Sell BuyerPeerPlugin::PeerState::lastSellReceived() const {
    return _lastSellReceived;
}

void BuyerPeerPlugin::PeerState::setLastSellReceived(const Sell &lastSellReceived) {
    _lastSellReceived = lastSellReceived;
}

BuyerPeerPlugin::PeerState BuyerPeerPlugin::peerState() const {
    return _peerState;
}

void BuyerPeerPlugin::setPeerState(const PeerState & peerState) {
    _peerState = peerState;
}

BuyerPeerPlugin::ClientState BuyerPeerPlugin::clientState() const {
    return _clientState;
}

void BuyerPeerPlugin::setClientState(ClientState clientState) {
    _clientState = clientState;
}

/**
 * BuyerPeerPlugin::Status
 */

BuyerPeerPlugin::Status::Status() {
}

BuyerPeerPlugin::Status::Status(const PeerState & peerState, ClientState clientState)
    : _peerState(peerState)
    , _clientState(clientState) {
}

BuyerPeerPlugin::PeerState BuyerPeerPlugin::Status::peerState() const {
    return _peerState;
}

void BuyerPeerPlugin::Status::setPeerState(const PeerState & peerState) {
    _peerState = peerState;
}

BuyerPeerPlugin::ClientState BuyerPeerPlugin::Status::clientState() const {
    return _clientState;
}

void BuyerPeerPlugin::Status::setClientState(ClientState clientState) {
    _clientState = clientState;
}

quint32 BuyerPeerPlugin::payorSlot() const {
    return _payorSlot;
}

void BuyerPeerPlugin::setPayorSlot(quint32 payorSlot) {
    _payorSlot = payorSlot;
}

/**
 * BuyerPeerPlugin
 */

#include "BuyerTorrentPlugin.hpp"
#include "PluginMode.hpp"
#include "Message/Observe.hpp"
#include "Message/Buy.hpp"
#include "Message/Sell.hpp"
#include "Message/JoinContract.hpp"
#include "Message/JoiningContract.hpp"
#include "Message/SignRefund.hpp"
#include "Message/RefundSigned.hpp"
#include "Message/Ready.hpp"
#include "Message/RequestFullPiece.hpp"
#include "Message/FullPiece.hpp"
#include "Message/Payment.hpp"

#include <libtorrent/bt_peer_connection.hpp>

#include <QLoggingCategory>

BuyerPeerPlugin::BuyerPeerPlugin(BuyerTorrentPlugin * plugin,
                                 libtorrent::bt_peer_connection * connection,
                                 QLoggingCategory & category)
    : PeerPlugin(plugin, connection, category)
    , _plugin(plugin)
    , _peerState(//PeerState::LastValidAction::no_bitswapr_message_sent,
                 PeerState::FailureMode::not_failed,
                 Sell(),
                 JoiningContract())
    , _clientState(ClientState::no_bitswapr_message_sent)
    , _payorSlot(-1) // deterministic sentinel value
    , _indexOfAssignedPiece(-1) { // deterministic sentinel value
    //, _pieceSize(-1) { // deterministic sentinel value
    //, _blockSize(0)
    //, _numberOfBlocksInPiece(0)
    //, _numberOfBlocksRequested(0)
    //, _numberOfBlocksReceived(0) {
}

BuyerPeerPlugin::~BuyerPeerPlugin() {
    qCDebug(_category) << "~BuyerPeerPlugin";
}

char const * BuyerPeerPlugin::type() const {
    return "BitSwapr payment buyer peer plugin.";
}

/*
 * m_pc.disconnect(errors::pex_message_too_large, 2);
 * m_pc.disconnect(errors::too_frequent_pex);
 * m_pc.remote().address()
 */

void BuyerPeerPlugin::on_disconnect(libtorrent::error_code const & ec) {

    /**
     *
     * m_torrent->num_peers() >= m_torrent->max_connections()
            && ses.num_connections() >= ses.max_connections()
            && c.remote().address() != m_torrent->current_tracker().address()

     */

    qCDebug(_category) << "on_disconnect:" << ec.message().c_str();

    //_connectionAlive = false;
}


/**
 * Called when the peer is successfully connected. Note that incoming
 * connections will have been connected by the time the peer plugin is
 * attached to it, and won't have this hook called.
 */
void BuyerPeerPlugin::on_connected() {

}

bool BuyerPeerPlugin::on_extension_handshake(libtorrent::lazy_entry const & handshake) {

    if(_clientState != ClientState::no_bitswapr_message_sent) {
        throw std::exception("Extended handshake initiated at incorrect state.");
    }

    qCDebug(_category) << "Extended handshake arrived.";

    // Use base class extension handhsake processor
    bool keepPlugin = PeerPlugin::on_extension_handshake(handshake);

    // If handshake was successful
    if(keepPlugin) {

        // send mode message
        sendExtendedMessage(Buy(_plugin->maxPrice(), _plugin->maxLock(), _plugin->numberOfSellers()));
        // _plugin->maxFeePerByte()

        // and update new client state correspondingly
        _clientState = ClientState::buyer_mode_announced;
    }

    // Return status to libtorrent
    return keepPlugin;
}

bool BuyerPeerPlugin::on_have(int index) {

    qCDebug(_category) << "Ignoring have message.";

    // Signal that we handled message
    return true;
}

bool BuyerPeerPlugin::on_bitfield(libtorrent::bitfield const & bitfield) {

    qCDebug(_category) << "Ignoring bitfield";

    // Signal that we handled message
    return true;
}

bool BuyerPeerPlugin::on_have_all() {

    qCDebug(_category) << "Ignoring have all message.";

    // Signal that we handled message
    return true;
}

bool BuyerPeerPlugin::on_reject(libtorrent::peer_request const & peerRequest) {

    qCDebug(_category) << "Ignoring reject message.";

    // Signal that we handled message
    return true;
}

bool BuyerPeerPlugin::on_request(libtorrent::peer_request const & peerRequest) {

    qCDebug(_category) << "Ignoring request message.";

    // Signal that we handled message
    return true;
}

bool BuyerPeerPlugin::on_unchoke() {

    qCDebug(_category) << "Ignoring unchoke message.";

    // Signal that we handled message
    return true;
}

bool BuyerPeerPlugin::on_interested() {

    qCDebug(_category) << "Ignoring interested message.";

    // Signal that we handled message
    return true;
}

bool BuyerPeerPlugin::on_allowed_fast(int index) {

    qCDebug(_category) << "Ignoring allowed fast message.";

    // Signal that we handled message
    return true;
}

bool BuyerPeerPlugin::on_have_none() {

    qCDebug(_category) << "Ignoring have none message.";

    // Signal that we handled message
    return true;
}

bool BuyerPeerPlugin::on_choke() {

    qCDebug(_category) << "Ignoring choke message.";

    // Signal that we handled message
    return true;
}

bool BuyerPeerPlugin::on_not_interested() {

    qCDebug(_category) << "Ignoring not intereste message.";

    // Signal that we handled message
    return true;
}

bool BuyerPeerPlugin::on_piece(libtorrent::peer_request const& piece, libtorrent::disk_buffer_holder & data) {

    qCDebug(_category) << "Ignoring piece message.";

    // Signal that we handled message
    return true;
}

bool BuyerPeerPlugin::on_suggest(int index) {

    qCDebug(_category) << "Ignoring suggest message.";

    // Signal that we handled message
    return true;
}

bool BuyerPeerPlugin::on_cancel(libtorrent::peer_request const & peerRequest) {

    qCDebug(_category) << "Ignoring cancel message.";

    // Signal that we handled message
    return true;
}

bool BuyerPeerPlugin::on_dont_have(int index) {

    qCDebug(_category) << "Ignoring dont have message.";

    // Signal that we handled message
    return true;
}

/*
 * Called after a choke message has been sent to the peer
 */
void BuyerPeerPlugin::sent_unchoke() {
    qCDebug(_category) << "sent_unchoke: Should not happen!";
}

/*
 * Called when libtorrent think this peer should be disconnected.
 * If the plugin returns false, the peer will not be disconnected.
 */
bool BuyerPeerPlugin::can_disconnect(libtorrent::error_code const & ec) {

   // qCDebug(_category) << "can_disconnect:" << ec.message().c_str();

    // CRITICAL
    //return true;
    return false;
}

/*
 * This is not called for web seeds
 */
bool BuyerPeerPlugin::on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body) {

    qCDebug(_category) << "on_unknown_message";

    // CRITICAL
    return false;
}

/*
 * Called when a piece that this peer participated in passes the hash_check
 */
void BuyerPeerPlugin::on_piece_pass(int index) {
/**
    qCDebug(_category) << "on_piece_pass:" << index;

    // on_piece_pass() =>
    Q_ASSERT(_indexOfAssignedPiece == index); // peer should only be relaying blocks I am requesting

    Q_ASSERT(_clientState == ClientState::waiting_for_libtorrent_to_validate_piece);


    //Q_ASSERT(_peerState.lastAction() == PeerState::LastValidAction::signed_refund ||
    //        _peerState.lastAction() == PeerState::LastValidAction::sent_valid_piece); // presumes that we would have stopped peer if it started sending pieces without being asked

    //Q_ASSERT(_numberOfBlocksInPiece == _numberOfBlocksReceived);
    //Q_ASSERT(_numberOfBlocksRequested == _numberOfBlocksReceived);
    //Q_ASSERT(_unservicedRequests.empty());

    // Make a payment
    const Signature & paymentSignature = _plugin->makePaymentAndGetPaymentSignature(this);
    sendExtendedMessage(Payment(paymentSignature));

    // Remember that piece was downloaded
    _downloadedValidPieces.append(_indexOfAssignedPiece);

    // Notify torrent plugin that piece has been downloaded
    _plugin->pieceDownloaded(_indexOfAssignedPiece);

    // Update state
    _clientState = ClientState::needs_to_be_assigned_piece;

    // Get a new piece if one is available.
    // In the future, we should perhaps delay this decision, and let
    // tick() in torretn plugin decide, since it has information about down speeds.
    _plugin->assignPieceToPeerPlugin(this);
    */
}

/*
 * Called when a piece that this peer participated in fails the hash_check
 */
void BuyerPeerPlugin::on_piece_failed(int index) {
    qCDebug(_category) << "BuyerPeerPlugin::on_piece_failed() NOT IMPLEMENTED.";
}

/*
 * Called aproximately once every second
 */
void BuyerPeerPlugin::tick() {
    //qCDebug(_category) << "BuyerPeerPlugin.tick()";
}

/*
 * Called each time a request message is to be sent. If true is returned,
 * the original request message won't be sent and no other plugin will have this function called.
 */
bool BuyerPeerPlugin::write_request(libtorrent::peer_request const & peerRequest) {

    qCDebug(_category) << "Blocking write_request:" << peerRequest.piece << ":" << peerRequest.start;

    // Signal that it has been handled
    return true;
}

/**
quint32 BuyerPeerPlugin::refillPipeline() {

    // Pipeline params must be well founded <=>
    Q_ASSERT(_requestPipelineLength > _requestPipelineRefillBound);

    // pipline is maintained <=>
    Q_ASSERT(_unservicedRequests.size() <= _requestPipelineLength);

    // Check that the pipeline actually needs to be refilled
    if(_unservicedRequests.size() >= _requestPipelineRefillBound)
        return 0;

    // How many pieces to add to pipeline at most
    int numberOfMessagesToAddToPipeline = _requestPipelineLength - _unservicedRequests.size();

    qCDebug(_category) << "refillPipeline with " << numberOfMessagesToAddToPipeline << " requests.";

    // Issue requests
    for(int i = 0;i < numberOfMessagesToAddToPipeline;i++) {

        // Create request
        libtorrent::peer_request r;
        r.piece = _indexOfAssignedPiece;
        r.start = _numberOfBlocksRequested * _blockSize;

        // Length of last block may be different from other blocks
        if(_numberOfBlocksRequested == _numberOfBlocksInPiece - 1)
            r.length = _pieceSize - r.start; // last block is shorter
        else
            r.length = _blockSize;

        // Send to peer
        _connection->write_request(r);

        // Keep track of number of blocks requested
        _numberOfBlocksRequested++;

        // This request has not yet been serviced
        _unservicedRequests.insert(r);
    }
}
*/

BuyerPeerPlugin::Status BuyerPeerPlugin::status() const {
    return Status(_peerState, _clientState);
}

PluginMode BuyerPeerPlugin::mode() const {
    return PluginMode::Buyer;
}

/**
bool BuyerPeerPlugin::assignedPiece() const {
    return _assignedPiece;
}

void BuyerPeerPlugin::setAssignedPiece(bool assignedPiece) {
    _assignedPiece = assignedPiece;
}
*/

int BuyerPeerPlugin::indexOfAssignedPiece() const {
    return _indexOfAssignedPiece;
}

void BuyerPeerPlugin::setIndexOfAssignedPiece(int indexOfAssignedPiece) {
    _indexOfAssignedPiece = indexOfAssignedPiece;
}

/**
int BuyerPeerPlugin::pieceSize() const {
    return _pieceSize;
}

void BuyerPeerPlugin::setPieceSize(int pieceSize) {
    _pieceSize = pieceSize;
}

int BuyerPeerPlugin::blockSize() const {
    return _blockSize;
}

void BuyerPeerPlugin::setBlockSize(int blockSize) {
    _blockSize = blockSize;
}

int BuyerPeerPlugin::numberOfBlocksInPiece() const {
    return _numberOfBlocksInPiece;
}

void BuyerPeerPlugin::setNumberOfBlocksInPiece(int numberOfBlocksInPiece) {
    _numberOfBlocksInPiece = numberOfBlocksInPiece;
}


int BuyerPeerPlugin::numberOfBlocksRequested() const {
    return _numberOfBlocksRequested;
}

void BuyerPeerPlugin::setNumberOfBlocksRequested(int numberOfBlocksRequested) {
    _numberOfBlocksRequested = numberOfBlocksRequested;
}

int BuyerPeerPlugin::numberOfBlocksReceived() const {
    return _numberOfBlocksReceived;
}

void BuyerPeerPlugin::setNumberOfBlocksReceived(int numberOfBlocksReceived) {
    _numberOfBlocksReceived = numberOfBlocksReceived;
}

QSet<libtorrent::peer_request> BuyerPeerPlugin::unservicedRequests() const {
    return _unservicedRequests;
}

void BuyerPeerPlugin::setUnservicedRequests(const QSet<libtorrent::peer_request> &unservicedRequests) {
    _unservicedRequests = unservicedRequests;
}

QDateTime BuyerPeerPlugin::whenLastRequestServiced() const {
    return _whenLastSentFullPiece;
}

void BuyerPeerPlugin::setWhenLastRequestServiced(const QDateTime &whenLastRequestServiced) {
    _whenLastSentFullPiece = whenLastRequestServiced;
}
*/

QList<int> BuyerPeerPlugin::downloadedPieces() const {
    return _downloadedValidPieces;
}

void BuyerPeerPlugin::setDownloadedPieces(const QList<int> & downloadedPieces) {
    _downloadedValidPieces = downloadedPieces;
}

void BuyerPeerPlugin::addDownloadedPiece(int index) {
    _downloadedValidPieces.append(index);
}

void BuyerPeerPlugin::processObserve(const Observe * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // May arrive at any time, hence no _peerState.lastAction() invariant to check

    // Update peer state with new valid action
    //_peerState.setLastAction(PeerState::LastValidAction::mode_announced);

    // Note that peer is in observe mode
    _peerModeAnnounced = PeerModeAnnounced::observer;

    // We dont do anything else, since we cant buy from peer in observer mode
}

void BuyerPeerPlugin::processBuy(const Buy * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // May arrive at any time, hence no _peerState.lastAction() invariant to check

    // Update peer state with new valid action
    //_peerState.setLastAction(PeerState::LastValidAction::mode_announced);

    // Note that peer is in buyer mode
    _peerModeAnnounced = PeerModeAnnounced::buyer;

    // We dont do anything else, since we cant buy from peer in buyer mode
}

void BuyerPeerPlugin::processSell(const Sell * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // May arrive at any time, hence no _peerState.lastAction() invariant to check

    // Update peer state with new valid action
    //_peerState.setLastAction(PeerState::LastValidAction::mode_announced);
    _peerState.setLastSellReceived(*m);

    // Note that peer is seller
    _peerModeAnnounced = PeerModeAnnounced::seller;

    // If peer has not been invited, and plugin says ok,
    // then we send invite
    if(_clientState == ClientState::buyer_mode_announced &&
            _plugin->inviteSeller(m->minPrice(), m->minLock())) {

        // invite to join contract
        sendExtendedMessage(JoinContract());

        // and remember invitation
        _clientState = ClientState::invited_to_contract;

        qCDebug(_category) << "Invited seller";
    } else
        qCDebug(_category) << "Did not invite seller";
}

void BuyerPeerPlugin::processJoinContract(const JoinContract * m) {
    throw std::exception("JoinContract message should never be sent to buyer mode peer.");
}

void BuyerPeerPlugin::processJoiningContract(const JoiningContract * m) {

    // Check that we are in correct stage
    if(_clientState != ClientState::invited_to_contract)
        throw std::exception("JoiningContract message should only be sent in response to a contract invitation.");

    // _clientState == ClientState::invited_to_contract =>
    //Q_ASSERT(_peerState.lastAction() == PeerState::LastValidAction::mode_announced);

    // Update peer state
    //_peerState.setLastAction(PeerState::LastValidAction::joined_contract);
    _peerState.setLastJoiningContractReceived(*m);

    // Tell torrent plugin about contract joining attempt
    const Sell & sell = _peerState.lastSellReceived();
    const JoiningContract & joinContract = _peerState.lastJoiningContractReceived();

    _plugin->sellerWantsToJoinContract(this,
                                       sell.minPrice(),
                                       sell.minLock(),
                                       joinContract.contractPk(),
                                       joinContract.finalPk());
}

void BuyerPeerPlugin::processSignRefund(const SignRefund * m) {
    throw std::exception("SignRefund message should never be sent to buyer mode peer.");
}

void BuyerPeerPlugin::processRefundSigned(const RefundSigned * m) {

    // Check that we are in correct stage
    if(_clientState != ClientState::asked_for_refund_signature)
        throw std::exception("RefundSigned message should only be sent in response to a refund signature invitation.");

    // _clientState != ClientState::asked_for_refund_signature =>
    //Q_ASSERT(_peerState.lastAction() == PeerState::LastValidAction::joined_contract);

    // Tell torrent plugin about refund signing attempt
    // Also updates _clientState
    bool wasValid = _plugin->sellerProvidedRefundSignature(this, m->sig());

    // Update peer state
    if(wasValid) {

        //_peerState.setLastAction(PeerState::LastValidAction::signed_refund);

    } else {

        // We could come here just because peer was late to respond with an old
        // signature, or we had to reset a contract

        // Deal with sequence numbers here later!!!!

        //_peerState.failureMode(Peer);

        Q_ASSERT(false); // Should never happen, we do not support this properly as of yet
    }
}

void BuyerPeerPlugin::processReady(const Ready * m) {
    throw std::exception("Ready message should never be sent to buyer mode peer.");
}

void BuyerPeerPlugin::processRequestFullPiece(const RequestFullPiece * m) {
    throw std::exception("RequestFullPiece message should never be sent to buyer mode peer.");
}

void BuyerPeerPlugin::processFullPiece(const FullPiece * m) {

    // Check that peer is sending a state compatible message
    if(_clientState != ClientState::waiting_for_full_piece)
        throw std::exception("Peer Full Piece at incorrect stage."); // Handle properly later

    // _clientState == ClientState::waiting_for_requests_to_be_serviced =>
    Q_ASSERT(_peerBEP10SupportStatus == BEPSupportStatus::supported);
    Q_ASSERT(_peerBitSwaprBEPSupportStatus == BEPSupportStatus::supported);
    //Q_ASSERT(_peerState.lastAction() == PeerState::LastValidAction::signed_refund ||
    //         _peerState.lastAction() == PeerState::LastValidAction::sent_valid_piece);

    // Check that this is indeed the piece for which we have issued requests
    //if(piece.piece != _indexOfAssignedPiece)
    //    throw std::exception("Peer sent piece message for incorrect piece."); // Handle properly later

    // Check that we actually requested this block
    //if(!_unservicedRequests.contains(piece))
    //    throw std::exception("Peer sent piece which was not requested"); // Handle properly later

    // Remove request from set of unserviced requests
    //_unservicedRequests.remove(piece);

    // Note that last request was serviced now
    //_whenLastSentFullPiece = QDateTime::currentDateTime();

    // Update block download counter
    //_numberOfBlocksReceived++;

    // Update state if we have no outstanding requests
    //if(_numberOfBlocksInPiece == _numberOfBlocksReceived)
    //    _clientState = ClientState::waiting_for_libtorrent_to_validate_piece;

    // Make new requests if pipeline is running low
    //if(_unservicedRequests.size() < _requestPipelineRefillBound)
    //    refillPipeline();

    // Ask libtorrent to validate piece
    _plugin->fullPieceArrived(this, m->piece(), m->length());
}

void BuyerPeerPlugin::processPayment(const Payment * m) {
    throw std::exception("Payment message should never be sent to buyer mode peer.");
}

void BuyerPeerPlugin::peerModeReset() {

}

/*
void BuyerPeerPlugin::sendStatusToController() {

    // Send status to controller
    //PeerPluginStatus status(_peerPluginConfiguration.getPeerPluginId(), _peerPluginConfiguration.getPeerPluginState(), 0);
    PeerPluginStatus status(_peerPluginId, _lastPeerAction, 0);

    //emit peerPluginStatusUpdated(status);
}
*/
