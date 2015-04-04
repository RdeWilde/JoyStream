#include "BuyerPeerPlugin.hpp"

/**
 * BuyerPeerPlugin::PeerState
 */

BuyerPeerPlugin::PeerState::PeerState()
    : _lastAction(LastValidAction::no_bitswapr_message_sent)
    , _failureMode(FailureMode::not_failed)
    , _minPrice(0)
    , _minLock(0) {
}

BuyerPeerPlugin::PeerState::PeerState(LastValidAction lastAction,
                                        FailureMode failureMode,
                                        quint64 minPrice,
                                        quint32 minLock,
                                        const PublicKey & pK)
    : _lastAction(lastAction)
    , _failureMode(failureMode)
    , _minPrice(minPrice)
    , _minLock(minLock)
    , _contractPk(pK){
}

BuyerPeerPlugin::PeerState::LastValidAction BuyerPeerPlugin::PeerState::lastAction() const {
    return _lastAction;
}

void BuyerPeerPlugin::PeerState::setLastAction(LastValidAction lastAction) {
    _lastAction = lastAction;
}

BuyerPeerPlugin::PeerState::FailureMode BuyerPeerPlugin::PeerState::failureMode() const {
    return _failureMode;
}

void BuyerPeerPlugin::PeerState::setFailureMode(FailureMode failureMode) {
    _failureMode = failureMode;
}

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
 * BuyerPeerPlugin::Configuration
 */

BuyerPeerPlugin::Configuration::Configuration() {

}

BuyerPeerPlugin::Configuration::Configuration(const ExtendedMessageIdMapping & clientMapping,
                                              const ExtendedMessageIdMapping & peerMapping,
                                              BEPSupportStatus peerBEP10SupportStatus,
                                              BEPSupportStatus peerBitSwaprBEPSupportStatus,
                                              const PeerState & peerState,
                                              ClientState clientState,
                                              quint32 payorSlot)
    : PeerPlugin::Configuration(clientMapping, peerMapping, peerBEP10SupportStatus, peerBitSwaprBEPSupportStatus)
    , _peerState(peerState)
    , _clientState(clientState)
    , _payorSlot(payorSlot) {
}

BuyerPeerPlugin::PeerState BuyerPeerPlugin::Configuration::peerState() const {
    return _peerState;
}

void BuyerPeerPlugin::Configuration::setPeerState(const PeerState & peerState) {
    _peerState = peerState;
}

BuyerPeerPlugin::ClientState BuyerPeerPlugin::Configuration::clientState() const {
    return _clientState;
}

void BuyerPeerPlugin::Configuration::setClientState(ClientState clientState) {
    _clientState = clientState;
}

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
#include "Message/Payment.hpp"

#include <QLoggingCategory>

BuyerPeerPlugin::BuyerPeerPlugin(BuyerTorrentPlugin * plugin,
                                 libtorrent::bt_peer_connection * connection,
                                 const Configuration & configuration,
                                 QLoggingCategory & category)
    : PeerPlugin(plugin, connection, configuration, category)
    , _plugin(plugin)
    , _peerState(configuration.peerState())
    , _clientState(configuration.clientState())
    , _payorSlot(-1) // deterministic value sentinel value
    , _assignedPiece(false)
    , _indexOfAssignedPiece(-1) // deterministic value sentinel value
    , _lastBlockDownloaded(-1) { // deterministic value sentinel value


    //, _lastMessageStateCompatibility(MessageStateCompatibility::compatible){
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

    qCDebug(_category) << "on_disconnect";

    _connectionAlive = false;
}

void BuyerPeerPlugin::on_connected() {

}

bool BuyerPeerPlugin::on_extension_handshake(libtorrent::lazy_entry const & handshake) {

    // Use base class extension handhsake processor
    bool keepPlugin = PeerPlugin::on_extension_handshake(handshake);

    // If handshake was successful
    if(keepPlugin) {

        // send mode message
        sendExtendedMessage(Buy(_plugin->maxPrice(), _plugin->maxLock()));

        // and update new client state correspondingly
        _clientState = ClientState::buyer_mode_announced;
    }

    // Return status to libtorrent
    return keepPlugin;
}

bool BuyerPeerPlugin::on_have(int index) {

    qCDebug(_category) << "on_have";
    return false;
}

bool BuyerPeerPlugin::on_bitfield(libtorrent::bitfield const & bitfield) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_bitfield";
*/
    return false;
}

bool BuyerPeerPlugin::on_have_all() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_have_all";
        */

    return false;
}

bool BuyerPeerPlugin::on_reject(libtorrent::peer_request const & peerRequest) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_reject";
        */

    return false;
}

bool BuyerPeerPlugin::on_request(libtorrent::peer_request const & peerRequest) {

    //bittorrentPeerConnection_->incoming_request();

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_request";
        */

    return false;
}

bool BuyerPeerPlugin::on_unchoke() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_unchoke";
        */

    return false;
}

bool BuyerPeerPlugin::on_interested() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_interested";
        */

    return false;
}

bool BuyerPeerPlugin::on_allowed_fast(int index) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_allowed_fast(" << index << ")";
        */

    return false;
}

bool BuyerPeerPlugin::on_have_none() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_have_none";
        */

    return false;
}

bool BuyerPeerPlugin::on_choke() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_choke";
        */

    return false;
}

bool BuyerPeerPlugin::on_not_interested() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_not_interested";
        */

    return false;
}

bool BuyerPeerPlugin::on_piece(libtorrent::peer_request const& piece, libtorrent::disk_buffer_holder & data) {

    qCDebug(_category) << "on_piece";

    // Check that peer is sending a state compatible message
    if(_clientState != ClientState::waiting_for_requests_to_be_serviced)
        throw std::exception("Peer sent on_piece at incorrect stage."); // Handle properly later

    // _clientState == ClientState::waiting_for_requests_to_be_serviced =>
    Q_ASSERT(_peerBEP10SupportStatus == BEPSupportStatus::supported);
    Q_ASSERT(_peerBitSwaprBEPSupportStatus == BEPSupportStatus::supported);
    Q_ASSERT(_peerState.lastAction() == PeerState::LastValidAction::signed_refund ||
             _peerState.lastAction() == PeerState::LastValidAction::sent_valid_piece);

    // Check that this is indeed the piece for which we have issued requests
    if(piece.piece != _indexOfAssignedPiece)
        throw std::exception("Peer sent piece message for incorrect piece."); // Handle properly later

    // Check that we actually requested this block
    if(!_unservicedRequests.contains(piece))
        throw std::exception("Peer sent piece which was not requested"); // Handle properly later

    // Remove request from set of unserviced requests
    _unservicedRequests.remove(piece);

    // Note that last request was serviced now
    _whenLastRequestServiced = QDateTime::currentDateTime();

    // Update block download counter
    _numberOfBlocksReceived++;

    // Update state if we have no outstanding requests
    if(_numberOfBlocksInPiece == _numberOfBlocksReceived)
        _clientState = ClientState::waiting_for_libtorrent_to_validate_piece;

    // Make new requests if pipeline is running low
    if(_unservicedRequests.size() < _requestPipelineRefillBound)
        refillPipeline();

    // Use normal piece message handler
    return false;
}

bool BuyerPeerPlugin::on_suggest(int index) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_suggest(" << index << ")";
        */

    return false;
}

bool BuyerPeerPlugin::on_cancel(libtorrent::peer_request const & peerRequest) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_cancel";
        */

    return false;
}

bool BuyerPeerPlugin::on_dont_have(int index) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_dont_have(" << index << ")";
        */

    return false;
}

/*
 * Called after a choke message has been sent to the peer
 */
void BuyerPeerPlugin::sent_unchoke() {
}

/*
 * Called when libtorrent think this peer should be disconnected.
 * If the plugin returns false, the peer will not be disconnected.
 */
bool BuyerPeerPlugin::can_disconnect(libtorrent::error_code const & ec) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "can_disconnect";
        */

    // CRITICAL
    return true;
}

/*
 * This is not called for web seeds
 */
bool BuyerPeerPlugin::on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body) {

    // CRITICAL
    return false;
}

/*
 * Called when a piece that this peer participated in passes the hash_check
 */
void BuyerPeerPlugin::on_piece_pass(int index) {

    qCDebug(_category) << "on_piece_pass:" << index;

    // on_piece_pass() =>
    Q_ASSERT(_indexOfAssignedPiece == index); // peer should only be relaying blocks I am requesting
    Q_ASSERT(_clientState == ClientState::waiting_for_libtorrent_to_validate_piece);
    Q_ASSERT(_peerState == PeerState::LastValidAction::signed_refund ||
            _peerState == PeerState::LastValidAction::sent_valid_piece); // presumes that we would have stopped peer if it started sending pieces without being asked
    Q_ASSERT(_numberOfBlocksInPiece == _numberOfBlocksReceived);
    Q_ASSERT(_numberOfBlocksRequested == _numberOfBlocksReceived);
    Q_ASSERT(_unservicedRequests.empty());

    // Make a payment
    const Signature & paymentSignature = _plugin->makePaymentAndGetPaymentSignature(this);
    sendExtendedMessage(Payment(paymentSignature));

    // Remember that piece was downloaded
    _downloadedPieces.insert(_indexOfAssignedPiece);

    // Notify torrent plugin that piece has been downloaded
    _plugin->pieceDownloaded(_indexOfAssignedPiece);

    // Update state
    _clientState = ClientState::needs_to_be_assigned_piece;

    // Get a new piece if one is available.
    // In the future, we should perhaps delay this decision, and let
    // tick() in torretn plugin decide, since it has information about down speeds.
    _plugin->assignPieceToPeerPlugin(this);
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

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "write_request";
    */

    return false;
}

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
    return _whenLastRequestServiced;
}

void BuyerPeerPlugin::setWhenLastRequestServiced(const QDateTime &whenLastRequestServiced) {
    _whenLastRequestServiced = whenLastRequestServiced;
}

QSet<int> BuyerPeerPlugin::downloadedPieces() const {
    return _downloadedPieces;
}

void BuyerPeerPlugin::setDownloadedPieces(const QSet<int> & downloadedPieces) {
    _downloadedPieces = downloadedPieces;
}

void BuyerPeerPlugin::processObserve(const Observe * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // May arrive at any time, hence no _peerState.lastAction() invariant to check

    // Update peer state with new valid action
    _peerState.setLastAction(PeerState::LastValidAction::mode_announced);

    // Note that peer is in observe mode
    _peerModeAnnounced = PeerModeAnnounced::observer;

    // We dont do anything else, since we cant buy from peer in observer mode
}

void BuyerPeerPlugin::processBuy(const Buy * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // May arrive at any time, hence no _peerState.lastAction() invariant to check

    // Update peer state with new valid action
    _peerState.setLastAction(PeerState::LastValidAction::mode_announced);

    // Note that peer is in buyer mode
    _peerModeAnnounced = PeerModeAnnounced::buyer;

    // We dont do anything else, since we cant buy from peer in buyer mode
}

void BuyerPeerPlugin::processSell(const Sell * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // May arrive at any time, hence no _peerState.lastAction() invariant to check

    // Update peer state with new valid action
    _peerState.setLastAction(PeerState::LastValidAction::mode_announced);
    _peerState.setMinPrice(m->minPrice());
    _peerState.setMinLock(m->minLock());

    // Note that peer is seller
    _peerModeAnnounced = PeerModeAnnounced::seller;

    // If peer has not been invited, and plugin says ok,
    // then we send invite
    if(_clientState == ClientState::no_bitswapr_message_sent &&
            _plugin->inviteSeller(m->minPrice(), m->minLock())) {

        // invite to join contract
        sendExtendedMessage(JoinContract());

        // and remember invitation
        _clientState = ClientState::invited_to_contract;
    }
}

void BuyerPeerPlugin::processJoinContract(const JoinContract * m) {
    throw std::exception("JoinContract message should never be sent to buyer mode peer.");
}

void BuyerPeerPlugin::processJoiningContract(const JoiningContract * m) {

    // Check that we are in correct stage
    if(_clientState != ClientState::invited_to_contract)
        throw std::exception("JoiningContract message should only be sent in response to a contract invitation.");

    // _clientState == ClientState::invited_to_contract =>
    Q_ASSERT(_peerState.lastAction() == PeerState::LastValidAction::mode_announced);

    // Update peer state
    _peerState.setLastAction(PeerState::LastValidAction::joined_contract);
    _peerState.setContractPk(m->contractPk());
    _peerState.setFinalPk(m->finalPk());

    // Tell torrent plugin about contract joining attempt
    _plugin->sellerWantsToJoinContract(this,
                                        _peerState.minPrice(),
                                        _peerState.contractPk(),
                                        _peerState.finalPk(),
                                        _peerState.minLock());
}

void BuyerPeerPlugin::processSignRefund(const SignRefund * m) {
    throw std::exception("SignRefund message should never be sent to buyer mode peer.");
}

void BuyerPeerPlugin::processRefundSigned(const RefundSigned * m) {

    // Check that we are in correct stage
    if(_clientState != ClientState::asked_for_refund_signature)
        throw std::exception("RefundSigned message should only be sent in response to a refund signature invitation.");

    // _clientState != ClientState::asked_for_refund_signature =>
    Q_ASSERT(_peerState.lastAction() == PeerState::LastValidAction::joined_contract);

    // Tell torrent plugin about refund signing attempt
    bool wasValid = _plugin->sellerProvidedRefundSignature(this, m->sig());

    // Update peer state
    if(wasValid)
        _peerState.setLastAction(PeerState::LastValidAction::signed_refund);
    else {

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
