#include "SellerPeerPlugin.hpp"

/**
 * SellerPeerPlugin::PeerState
 */

SellerPeerPlugin::PeerState::PeerState()
    : //_lastAction(LastValidAction::no_bitswapr_message_sent)
    _failureMode(FailureMode::not_failed) {
}

SellerPeerPlugin::PeerState::PeerState(//LastValidAction lastAction,
                                       FailureMode failureMode,
                                       const Buy & lastBuyReceived,
                                       const SignRefund & lastSignRefundReceived,
                                       const Payment & lastPaymentReceived,
                                       const RequestFullPiece & lastRequestFullPieceReceived)
    : //_lastAction(lastAction)
      _failureMode(failureMode)
    , _lastBuyReceived(lastBuyReceived)
    , _lastSignRefundReceived(lastSignRefundReceived)
    , _lastPaymentReceived(lastPaymentReceived)
    , _lastRequestFullPieceReceived(lastRequestFullPieceReceived){
}

/**
SellerPeerPlugin::PeerState::LastValidAction SellerPeerPlugin::PeerState::lastAction() const {
    return _lastAction;
}

void SellerPeerPlugin::PeerState::setLastAction(LastValidAction lastAction) {
    _lastAction = lastAction;
}
*/

SellerPeerPlugin::PeerState::FailureMode SellerPeerPlugin::PeerState::failureMode() const {
    return _failureMode;
}

void SellerPeerPlugin::PeerState::setFailureMode(FailureMode failureMode) {
    _failureMode = failureMode;
}

Payment SellerPeerPlugin::PeerState::lastPaymentReceived() const {
    return _lastPaymentReceived;
}

void SellerPeerPlugin::PeerState::setLastPaymentReceived(const Payment & lastPaymentReceived) {
    _lastPaymentReceived = lastPaymentReceived;
}

SignRefund SellerPeerPlugin::PeerState::lastSignRefundReceived() const {
    return _lastSignRefundReceived;
}

void SellerPeerPlugin::PeerState::setLastSignRefundReceived(const SignRefund & lastSignRefundReceived) {
    _lastSignRefundReceived = lastSignRefundReceived;
}

Buy SellerPeerPlugin::PeerState::lastBuyReceived() const {
    return _lastBuyReceived;
}

void SellerPeerPlugin::PeerState::setLastBuyReceived(const Buy & lastBuyReceived){
    _lastBuyReceived = lastBuyReceived;
}

RequestFullPiece SellerPeerPlugin::PeerState::lastRequestFullPieceReceived() const {
    return _lastRequestFullPieceReceived;
}

void SellerPeerPlugin::PeerState::setLastRequestFullPieceReceived(const RequestFullPiece & lastRequestFullPieceReceived) {
    _lastRequestFullPieceReceived = lastRequestFullPieceReceived;
}

/**
 * SellerPeerPlugin::Status
 */

#include "PluginMode.hpp"

SellerPeerPlugin::Status::Status()
    : _clientState(ClientState::no_bitswapr_message_sent) {
}

SellerPeerPlugin::Status::Status(PeerModeAnnounced peerModeAnnounced,
                                 BEPSupportStatus peerBEP10SupportStatus,
                                 BEPSupportStatus peerBitSwaprBEPSupportStatus,
                                 const PeerState & peerState,
                                 ClientState clientState,
                                 const Payee::Status & payeeStatus)
    : PeerPlugin::Status(peerModeAnnounced,
                         peerBEP10SupportStatus,
                         peerBitSwaprBEPSupportStatus)
    , _peerState(peerState)
    , _clientState(clientState)
    , _payeeStatus(payeeStatus) {
}

PluginMode SellerPeerPlugin::Status::pluginMode() const {
    return PluginMode::Seller;
}

SellerPeerPlugin::PeerState SellerPeerPlugin::Status::peerState() const {
    return _peerState;
}

void SellerPeerPlugin::Status::setPeerState(const PeerState &peerState) {
    _peerState = peerState;
}

SellerPeerPlugin::ClientState SellerPeerPlugin::Status::clientState() const {
    return _clientState;
}

void SellerPeerPlugin::Status::setClientState(ClientState clientState) {
    _clientState = clientState;
}

Payee::Status SellerPeerPlugin::Status::payeeStatus() const {
    return _payeeStatus;
}

void SellerPeerPlugin::Status::setPayeeStatus(const Payee::Status & payeeStatus) {
    _payeeStatus = payeeStatus;
}


/**
 * SellerPeerPlugin
 */

#include "SellerTorrentPlugin.hpp" // Needed to do conversion to TorrentPlugin *
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

#include "BitCoin/BitSwaprjs.hpp" // get_tx_outpoint

#include "Plugin.hpp"

#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/storage.hpp> // libtorrent::disk_io_job

#include <QLoggingCategory>
#include <QtMath> // qCeil

#include <boost/bind.hpp>


SellerPeerPlugin::SellerPeerPlugin(SellerTorrentPlugin * torrentPlugin,
                                   libtorrent::bt_peer_connection * connection,
                                   const Payee::Configuration & payeeConfiguration,
                                   //const libtorrent::torrent_info & torrentFile,
                                   int numberOfPieces,
                                   QLoggingCategory & category)
    : PeerPlugin(torrentPlugin, connection, category)
    , _plugin(torrentPlugin)
    , _peerState(//PeerState::LastValidAction::no_bitswapr_message_sent,
                 PeerState::FailureMode::not_failed,
                 Buy(),
                 SignRefund(),
                 Payment(),
                 RequestFullPiece())
    , _clientState(ClientState::no_bitswapr_message_sent)
    , _payee(payeeConfiguration)
    //, _torrentFile(torrentFile) {
    , _numberOfPieces(numberOfPieces) {

    //, _minPrice(minPrice)
    //, _minLock(minLock)
    //, _maxSellers(maxSellers) {
}

SellerPeerPlugin::~SellerPeerPlugin() {
    qCDebug(_category) << "~SellerPeerPlugin";
}

char const * SellerPeerPlugin::type() const {
    return "BitSwapr payment seller peer plugin.";
}

/*
 * m_pc.disconnect(errors::pex_message_too_large, 2);
 * m_pc.disconnect(errors::too_frequent_pex);
 * m_pc.remote().address()
 */
void SellerPeerPlugin::on_disconnect(libtorrent::error_code const & ec) {

    qCDebug(_category) << "on_disconnect:" << ec.message().c_str();

    //_connectionAlive = false;
}

/**
 * Called when the peer is successfully connected. Note that incoming
 * connections will have been connected by the time the peer plugin is
 * attached to it, and won't have this hook called.
 */
void SellerPeerPlugin::on_connected() {

}

bool SellerPeerPlugin::on_extension_handshake(libtorrent::lazy_entry const & handshake) {

    if(_clientState != ClientState::no_bitswapr_message_sent) {
        throw std::exception("Extended handshake initiated at incorrect state.");
    }

    qCDebug(_category) << "Extended handshake arrived.";

    // Use base class extension handhsake processor
    bool keepPlugin = PeerPlugin::on_extension_handshake(handshake);

    // If handshake was successful
    if(keepPlugin) {

        // send mode message
        sendExtendedMessage(Sell(_payee.price(), _payee.lockTime(), _payee.maximumNumberOfSellers())); // _plugin->minPrice(), _plugin->minLock()

        // and update new client state correspondingly
        _clientState = ClientState::seller_mode_announced;
    }

    // Return status to libtorrent
    return keepPlugin;
}

bool SellerPeerPlugin::on_have(int index) {

    qCDebug(_category) << "Ignoring have message.";

    // Signal that we handled message
    return true;
}

bool SellerPeerPlugin::on_bitfield(libtorrent::bitfield const & bitfield) {

    qCDebug(_category) << "Ignoring bitfield";

    // Signal that we handled message
    return true;
}

bool SellerPeerPlugin::on_have_all()  {

    qCDebug(_category) << "Ignoring have all message.";

    // Signal that we handled message
    return true;
}

bool SellerPeerPlugin::on_reject(libtorrent::peer_request const & peerRequest) {

    qCDebug(_category) << "Ignoring reject message.";

    // Signal that we handled message
    return true;
}

bool SellerPeerPlugin::on_request(libtorrent::peer_request const & peerRequest)  {

    qCDebug(_category) << "Ignoring reject message.";

    // Signal that we handled message
    return true;

    /**
    if(_clientState == ClientState::awaiting_request_after_ready_announced) {

        // are we being asked for piece we have not sent,
        // and which is part of first piece? if so, then ok.

    } else if(_clientState == ClientState::awaiting_request_after_piece_sent) {


        //
        // Should we get paid now.

    } else {
        throw std::exception("Peer sent request and incorrect stage.");
    }

    Q_ASSERT(_peerBEP10SupportStatus == BEPSupportStatus::supported);
    Q_ASSERT(_peerBitSwaprBEPSupportStatus == BEPSupportStatus::supported);




    // Signal that we handled message
    return true;
    */
}

bool SellerPeerPlugin::on_unchoke()  {

    qCDebug(_category) << "Ignoring unchoke message.";

    // Signal that we handled message
    return true;
}

bool SellerPeerPlugin::on_interested()  {

    qCDebug(_category) << "Ignoring interested message.";

    // Signal that we handled message
    return true;
}

bool SellerPeerPlugin::on_allowed_fast(int index)  {

    qCDebug(_category) << "Ignoring allowed fast message.";

    // Signal that we handled message
    return true;
}

bool SellerPeerPlugin::on_have_none()  {

    qCDebug(_category) << "Ignoring have none message.";

    // Signal that we handled message
    return true;
}

bool SellerPeerPlugin::on_choke()  {

    qCDebug(_category) << "Ignoring choke message.";

    // Signal that we handled message
    return true;
}

bool SellerPeerPlugin::on_not_interested()  {

    qCDebug(_category) << "Ignoring not intereste message.";

    // Signal that we handled message
    return true;
}

bool SellerPeerPlugin::on_piece(libtorrent::peer_request const & piece, libtorrent::disk_buffer_holder & data)  {

    qCDebug(_category) << "Ignoring piece message.";

    // Signal that we handled message
    return true;
}

bool SellerPeerPlugin::on_suggest(int index)  {

    qCDebug(_category) << "Ignoring suggest message.";

    // Signal that we handled message
    return true;
}

bool SellerPeerPlugin::on_cancel(libtorrent::peer_request const & peerRequest)  {

    qCDebug(_category) << "Ignoring cancel message.";

    // Signal that we handled message
    return true;
}

bool SellerPeerPlugin::on_dont_have(int index)  {

    qCDebug(_category) << "Ignoring dont have message.";

    // Signal that we handled message
    return true;
}

/*
 * Called after a choke message has been sent to the peer
 */
void SellerPeerPlugin::sent_unchoke()  {
    qCDebug(_category) << "sent_unchoke: Should not happen!";
}

/*
 * Called when libtorrent think this peer should be disconnected.
 * If the plugin returns false, the peer will not be disconnected by libtorrent.
 */
bool SellerPeerPlugin::can_disconnect(libtorrent::error_code const & ec)  {

    //qCDebug(_category) << "can_disconnect:" << ec.message().c_str();

    // CRITICAL
    //    return false;
    return true;
}

/*
 * This is not called for web seeds
 */
bool SellerPeerPlugin::on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body)  {

    qCDebug(_category) << "on_unknown_message";

    // CRITICAL
    return false;
}

/*
 * Called when a piece that this peer participated in passes the hash_check
 */
void SellerPeerPlugin::on_piece_pass(int index)  {

    qCDebug(_category) << "Ignoring on_piece_pass.";

    // Use default handler?
    return;
}

/*
 * Called when a piece that this peer participated in fails the hash_check
 */
void SellerPeerPlugin::on_piece_failed(int index)  {

    qCDebug(_category) << "Ignoring on_piece_failed.";

    // Use default handler?
    return;
}

/*
 * Called aproximately once every second
 */
void SellerPeerPlugin::tick()  {
    //qCDebug(_category) << "SellerPeerPlugin.tick()";
}

/*
 * Called each time a request message is to be sent. If true is returned,
 * the original request message won't be sent and no other plugin will have this function called.
 */
bool SellerPeerPlugin::write_request(libtorrent::peer_request const & peerRequest)  {

    qCDebug(_category) << "Blocked writing request.";

    return true;
}

PluginMode SellerPeerPlugin::mode() const {
    return PluginMode::Seller;
}

SellerPeerPlugin::ClientState SellerPeerPlugin::clientState() const {
    return _clientState;
}

void SellerPeerPlugin::setClientState(const ClientState & clientState) {
    _clientState = clientState;
}

/**
quint32 SellerPeerPlugin::totalNumberOfPieces() const {
    return _totalNumberOfPieces;
}

void SellerPeerPlugin::setTotalNumberOfPieces(quint32 totalNumberOfPieces) {
    _totalNumberOfPieces = totalNumberOfPieces;
}

int SellerPeerPlugin::lastRequestedFullPiece() const {
    return _lastRequestedFullPiece;
}

void SellerPeerPlugin::setLastRequestedFullPiece(int lastRequestedFullPiece) {
    _lastRequestedFullPiece = lastRequestedFullPiece;
}

int SellerPeerPlugin::lastRequestedFullPieceSize() const {
    return _lastRequestedFullPieceSize;
}

void SellerPeerPlugin::setLastRequestedFullPieceSize(int lastRequestedFullPieceSize) {
    _lastRequestedFullPieceSize = lastRequestedFullPieceSize;
}

quint32 SellerPeerPlugin::numberOfAsyncReadsCompleted() const {
    return _numberOfAsyncReadsCompleted;
}

void SellerPeerPlugin::setNumberOfAsyncReadsCompleted(quint32 asyncReadsCompleted) {
    _numberOfAsyncReadsCompleted = asyncReadsCompleted;
}

QVector<libtorrent::disk_io_job> SellerPeerPlugin::completedAsyncReads() const {
    return _completedAsyncReads;
}

void SellerPeerPlugin::setCompletedAsyncReads(const QVector<libtorrent::disk_io_job> &completedAsyncReads) {
    _completedAsyncReads = completedAsyncReads;
}
*/

void SellerPeerPlugin::processObserve(const Observe * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // Update peer state with new valid action
    //_peerState.setLastAction(PeerState::LastValidAction::mode_announced);

    // Note that peer is in observe mode
    _peerModeAnnounced = PeerModeAnnounced::observer;
}

void SellerPeerPlugin::processBuy(const Buy * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // Update peer state with new valid action
    //_peerState.setLastAction(PeerState::LastValidAction::mode_announced);
    _peerState.setLastBuyReceived(*m);

    // Note that peer is buyer
    _peerModeAnnounced = PeerModeAnnounced::buyer;
}

void SellerPeerPlugin::processSell(const Sell * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // Update peer state with new valid action
    //_peerState.setLastAction(PeerState::LastValidAction::mode_announced);

    // Note that peer is in observe mode
    _peerModeAnnounced = PeerModeAnnounced::seller;
}

void SellerPeerPlugin::processJoinContract(const JoinContract * m) {

    qCDebug(_category) << "processJoinContract";

    if(_clientState != ClientState::seller_mode_announced &&
            _clientState != ClientState::ignored_contract_invitation)
        throw std::exception("JoinContract message should only be sent in response to an announced seller mode.");

    // _clientState != seller_mode_announced,ignored_contract_invitation =>
    Q_ASSERT(_payee.state() == Payee::State::waiting_for_payor_information);

    // Check that buy terms are compatible
    const Buy & lastBuyReceived = _peerState.lastBuyReceived();

    if(_payee.price() <=  lastBuyReceived.maxPrice() &&
       _payee.lockTime() <= lastBuyReceived.maxLock() &&
       _payee.maximumNumberOfSellers() >= lastBuyReceived.minSellers()) {

        // invite to join contract
        sendExtendedMessage(JoiningContract(_payee.payeeContractKeys().pk(), _payee.payeePaymentKeys().pk()));

        // and remember invitation
        _clientState = ClientState::joined_contract;

        qCDebug(_category) << "Joined contract.";

    } else {

        // Remember that invite was ignored
        _clientState = ClientState::ignored_contract_invitation;

        qCDebug(_category) << "Ignored contract invitation.";
    }
}

void SellerPeerPlugin::processJoiningContract(const JoiningContract * m) {
    throw std::exception("JoiningContract message should never be sent to seller mode peer.");
}

void SellerPeerPlugin::processSignRefund(const SignRefund * m) {

    qCDebug(_category) << "processSignRefund";

    if(_clientState != ClientState::joined_contract)
        throw std::exception("SignRefund message should only be sent in response to joining the contract.");

    // _clientState == ClientState::joined_contract =>
    Q_ASSERT(_payee.state() == Payee::State::waiting_for_payor_information || // if this is first signing
             _payee.state() == Payee::State::has_all_information_required); // signed before and kept data

    // Save sign refund message
    _peerState.setLastSignRefundReceived(*m);

    // Update payee with most recent
    _payee.registerPayorInformation(OutPoint(m->hash(), m->index()), m->contractPk(), m->finalPk(), m->value());

    // Create refund signature
    Signature refundSignature = _payee.generateRefundSignature();

    // Send signed_refund message
    sendExtendedMessage(RefundSigned(refundSignature));

    // Update state
    _clientState = ClientState::signed_refund;

    qCDebug(_category) << "Signed refund.";
}

void SellerPeerPlugin::processRefundSigned(const RefundSigned * m) {
    throw std::exception("RefundSigned message should never be sent to seller mode peer.");
}

void SellerPeerPlugin::processReady(const Ready * m) {

    qCDebug(_category) << "processReady";

    if(_clientState != ClientState::signed_refund)
        throw std::exception("Ready message should only be sent in response to signing a refund.");

    // Start timer or something, to start looking for contract
    //quint64 funds = BitSwaprjs::get_tx_outpoint(fundingOutPoint, spent);

    // Update client state
    _clientState = ClientState::awaiting_fullpiece_request_after_ready_announced;
}

void SellerPeerPlugin::processRequestFullPiece(const RequestFullPiece * m) {

    qCDebug(_category) << "processRequestFullPiece";

    if(_clientState != ClientState::awaiting_fullpiece_request_after_ready_announced &&
       _clientState != ClientState::awaiting_piece_request_after_payment)
        throw std::exception("RequestFullPiece message should only be sent in response to signing a refund or a full piece.");

    // _clientState values =>
    Q_ASSERT(_payee.state() == Payee::State::has_all_information_required);
    // true => ()
    Q_ASSERT(_clientState != ClientState::awaiting_piece_request_after_payment ||
            !_fullPiecesSent.isEmpty());
    //Q_ASSERT(_completedAsyncReads.size() == 0);
    //Q_ASSERT(_numberOfAsyncReadsCompleted == 0);

    // Check piece validity
    if(m->pieceIndex() < 0 || m->pieceIndex() >= _numberOfPieces) //  _torrentFile.num_pieces()
        throw std::exception("Invalid full piece requested.");

    // Update peer state
    _peerState.setLastRequestFullPieceReceived(*m);

    // Update client state
    _clientState = ClientState::reading_piece_from_disk;

    // Do async read of piece
    _plugin->readPiece(this, m->pieceIndex());

    qCDebug(_category) << "reading piece " << m->pieceIndex();

    /**
    // Get size of given piece
    int pieceSize = _torrentFile.piece_size(m->pieceIndex());

    // Calculate number of blocks in the piece
    int numberOfBlocksInPiece = qCeil(((double)pieceSize) / _blockSize);

    // Create space in peer for the future callbacks
    _completedAsyncReads = QVector<libtorrent::disk_io_job>(numberOfBlocksInPiece);

    // Generate one request per block
    for(int block = 0;block < numberOfBlocksInPiece;block++) {

        // Build request
        libtorrent::peer_request request;
        request.piece = m->pieceIndex();
        request.start = block*_blockSize;

        // take into account that last block may be smaller
        request.length = (block < numberOfBlocksInPiece - 1) ? _blockSize : pieceSize - request.start;

        // Make request of piece manager
        _plugin->async_read(request, boost::bind(&SellerPeerPlugin::disk_async_read_handler, this, block, _1, _2));

        qCDebug(_category) << "async_read block " << block << " in piece " << m->pieceIndex();
    }
    */
}

/**
void SellerPeerPlugin::disk_async_read_handler(int block, int a, const libtorrent::disk_io_job & job) {

    // Check peer plugin
    Q_ASSERT(_numberOfAsyncReadsCompleted > 0 && _numberOfAsyncReadsCompleted <= _completedAsyncReads.size());
    Q_ASSERT(_clientState == SellerPeerPlugin::ClientState::reading_piece_from_disk);
    Q_ASSERT(_payee.state() == Payee::State::has_all_information_required);

    // Check request
    if(a != job.buffer_size)
        throw std::exception("Some sort of read error occured.");

    // Save result
    _completedAsyncReads[block] = job;

    // Count write
    Q_ASSERT(_numberOfAsyncReadsCompleted < _completedAsyncReads.size());
    _numberOfAsyncReadsCompleted++;

    // Was this last block?
    if(_numberOfAsyncReadsCompleted == _completedAsyncReads.size()) {

        // Container for piece data
        QVector<char> pieceData(_peerState.lastRequestFullPieceReceived().pieceIndex());

        char * pieceDataPtr = pieceData.data();

        // Create full piece message
        for(QVector<libtorrent::disk_io_job>::const_iterator i = _completedAsyncReads.constBegin(),
            end(_completedAsyncReads.constEnd());i != end;i++) {

            // Get disk job
            const libtorrent::disk_io_job & j = *i;

            // Copy data into standard vector
            memcpy(pieceDataPtr, j.buffer, j.buffer_size);

            // Increment pointer
            pieceDataPtr += j.buffer_size;
        }

        Q_ASSERT(pieceDataPtr - pieceData.data() == pieceData.size());

        // Send message
        sendExtendedMessage(FullPiece(pieceData));

        // Update client state
        _fullPiecesSent.append(job.piece);
        _numberOfAsyncReadsCompleted = 0;
        _completedAsyncReads.clear();
        _clientState = ClientState::awaiting_payment;
    }
}
*/

void SellerPeerPlugin::pieceRead(int piece, const boost::shared_array<char> & pieceData, int size) {

    qCDebug(_category) << "pieceRead" << piece;

    Q_ASSERT(_clientState == ClientState::reading_piece_from_disk);
    Q_ASSERT(piece == _peerState.lastRequestFullPieceReceived().pieceIndex());
    Q_ASSERT(_payee.state() == Payee::State::has_all_information_required);

    // Send piece
    sendExtendedMessage(FullPiece(pieceData, size));

    // Note that piece was sent
    _fullPiecesSent.append(piece);

    // Update client state
    _clientState = ClientState::awaiting_payment;
}

void SellerPeerPlugin::pieceReadFailed(int piece) {

    qCDebug(_category) << "pieceReadFailed" << piece;

    Q_ASSERT(_clientState == ClientState::reading_piece_from_disk);
    Q_ASSERT(piece == _peerState.lastRequestFullPieceReceived().pieceIndex());
    Q_ASSERT(_payee.state() == Payee::State::has_all_information_required);

    qCDebug(_category) << "reading pieceReadFailed " << piece;

    throw std::exception("Reading piece failed.");
}

SellerPeerPlugin::Status SellerPeerPlugin::status() const {

    return Status(_peerModeAnnounced,
                  _peerBEP10SupportStatus,
                  _peerBitSwaprBEPSupportStatus,
                  _peerState,
                  _clientState,
                  _payee.status());
}

void SellerPeerPlugin::processFullPiece(const FullPiece * m) {
    throw std::exception("FullPiece message should never be sent to seller mode peer.");
}

void SellerPeerPlugin::processPayment(const Payment * m) {

    qCDebug(_category) << "processPayment";

    if(_clientState != ClientState::awaiting_payment)
        throw std::exception("Payment message should only be sent in response to a full piece message.");

    // _clientState values =>
    Q_ASSERT(_payee.state() == Payee::State::has_all_information_required);

    // Validate payment
    bool valid = _payee.registerPayment(m->sig());

    if(!valid) {

        // Update error state in some way
        throw std::exception("Invalid payment received.");
    } else
        qCDebug(_category) << "Valid payment received";

    // Update state
    _clientState = ClientState::awaiting_piece_request_after_payment;

    // Note payment
    _plugin->plugin()->registerReceivedFunds(_payee.price());
}

void SellerPeerPlugin::peerModeReset() {
    //Properly reset payee (state)?
}
