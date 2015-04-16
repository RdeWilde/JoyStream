#include "SellerPeerPlugin.hpp"

/**
 * SellerPeerPlugin::PeerState
 */

SellerPeerPlugin::PeerState::PeerState()
    : _lastAction(LastValidAction::no_bitswapr_message_sent)
    , _failureMode(FailureMode::not_failed) {
}

SellerPeerPlugin::PeerState::PeerState(LastValidAction lastAction,
                                       FailureMode failureMode,
                                       const Buy & lastBuyReceived,
                                       const SignRefund & lastSignRefundReceived,
                                       const Payment & lastPaymentReceived)
    : _lastAction(lastAction)
    , _failureMode(failureMode)
    , _lastBuyReceived(lastBuyReceived)
    , _lastSignRefundReceived(lastSignRefundReceived)
    , _lastPaymentReceived(lastPaymentReceived) {
}

SellerPeerPlugin::PeerState::LastValidAction SellerPeerPlugin::PeerState::lastAction() const {
    return _lastAction;
}

void SellerPeerPlugin::PeerState::setLastAction(LastValidAction lastAction) {
    _lastAction = lastAction;
}

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

JoinContract SellerPeerPlugin::PeerState::lastJoinContractReceived() const {
    return _lastJoinContractReceived;
}

void SellerPeerPlugin::PeerState::setLastJoinContractReceived(const JoinContract & lastJoinContractReceived) {
    _lastJoinContractReceived = lastJoinContractReceived;
}

Buy SellerPeerPlugin::PeerState::lastBuyReceived() const {
    return _lastBuyReceived;
}

void SellerPeerPlugin::PeerState::setLastBuyReceived(const Buy & lastBuyReceived){
    _lastBuyReceived = lastBuyReceived;
}

/**
 * SellerPeerPlugin::Status
 */

SellerPeerPlugin::Status::Status()
    : _clientState(ClientState::no_bitswapr_message_sent) {
}

SellerPeerPlugin::Status::Status(const PeerState &peerState, ClientState clientState)
    : _peerState(peerState)
    , _clientState(clientState) {
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

#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/storage.hpp> // libtorrent::disk_io_job

#include <QLoggingCategory>

SellerPeerPlugin::SellerPeerPlugin(SellerTorrentPlugin * torrentPlugin,
                                   libtorrent::bt_peer_connection * connection,
                                   const Payee::Configuration & payeeConfiguration,
                                   QLoggingCategory & category)
    : PeerPlugin(torrentPlugin, connection, category)
    , _plugin(torrentPlugin)
    , _clientState(ClientState::no_bitswapr_message_sent)
    , _payee(payeeConfiguration) {
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

    qCDebug(_category) << "on_disconnect";

    //_connectionAlive = false;
}

void SellerPeerPlugin::on_connected() {

}

bool SellerPeerPlugin::on_extension_handshake(libtorrent::lazy_entry const & handshake) {

    Q_ASSERT(_clientState == ClientState::no_bitswapr_message_sent);

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
 * If the plugin returns false, the peer will not be disconnected.
 */
bool SellerPeerPlugin::can_disconnect(libtorrent::error_code const & ec)  {

    qCDebug(_category) << "can_disconnect";

    // CRITICAL
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
    qCDebug(_category) << "SellerPeerPlugin.tick()";
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

void SellerPeerPlugin::processObserve(const Observe * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // Update peer state with new valid action
    _peerState.setLastAction(PeerState::LastValidAction::mode_announced);

    // Note that peer is in observe mode
    _peerModeAnnounced = PeerModeAnnounced::observer;
}

void SellerPeerPlugin::processBuy(const Buy * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // Update peer state with new valid action
    _peerState.setLastAction(PeerState::LastValidAction::mode_announced);
    _peerState.setLastBuyReceived(*m);

    // Note that peer is buyer
    _peerModeAnnounced = PeerModeAnnounced::buyer;
}

void SellerPeerPlugin::processSell(const Sell * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // Update peer state with new valid action
    _peerState.setLastAction(PeerState::LastValidAction::mode_announced);

    // Note that peer is in observe mode
    _peerModeAnnounced = PeerModeAnnounced::seller;
}

void SellerPeerPlugin::processJoinContract(const JoinContract * m) {

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

    if(_clientState != ClientState::joined_contract)
        throw std::exception("SignRefund message should only be sent in response to joining the contract.");

    // _clientState == ClientState::joined_contract =>
    Q_ASSERT(_payee.state() == Payee::State::waiting_for_payor_information || // if this is first signing
             _payee.state() == Payee::State::has_all_information_required); // signed before and kept data

    // Save sign refund message
    _peerState.setLastSignRefundSent(*m);

    // Update payee with most recent
    bool spent;
    const OutPoint fundingOutPoint(m->hash(), m->index());
    quint64 funds = BitSwaprjs::get_tx_outpoint(fundingOutPoint, spent);

    // Check if outpoint is unspent
    if(!spent) {

        // Check that it has enough funding?

        // Register informaiton for payor
        _payee.registerPayorInformation(fundingOutPoint, m->contractPk(), m->finalPk(), funds);

    } else {

        // Some sort of error

        throw std::exception("fundingOutPoint was spent.");
    }

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

    qCDebug(_category) << "Ready.";

    if(_clientState != ClientState::signed_refund)
        throw std::exception("Ready message should only be sent in response to signing a refund.");

    // Start timer or something, to start looking for contract

    // Update client state
    _clientState = ClientState::awaiting_fullpiece_request_after_ready_announced;
}

void SellerPeerPlugin::processRequestFullPiece(const RequestFullPiece * m) {

    if(_clientState != ClientState::awaiting_fullpiece_request_after_ready_announced &&
       _clientState != ClientState::awaiting_piece_request_after_payment)
        throw std::exception("RequestFullPiece message should only be sent in response to signing a refund or a full piece.");

    // _clientState values =>
    Q_ASSERT(_payee.state() == Payee::State::has_all_information_required);

    // false => ()
    Q_ASSERT(_clientState != ClientState::awaiting_piece_request_after_payment ||
            !_sendFullPieces.isEmpty());

    // Check piece validity
    if(m->pieceIndex() < 0 || m->pieceIndex() >= num_pieces)
        throw std::exception("Invalid full piece requested.");

    // Remember piece
    _lastRequestedFullPiece = m->pieceIndex();

    // Update client state
    _clientState = ClientState::reading_piece_from_disk;

    // Start call to read piece
    _plugin->disk_async_read(this);

    //_torrent.filesystem().async_read(r, boost::bind(&smart_ban_plugin::on_read_ok_block, shared_from_this(), *i, _1, _2));
}

void SellerPeerPlugin::disk_async_read_handler(int, const libtorrent::disk_io_job & job) {

    Q_ASSERT(_clientState == ClientState::reading_piece_from_disk);
    Q_ASSERT(_payee.state() == Payee::State::has_all_information_required);

    // Save data

    //job.

    // Was this last one

    // if so, call to final handler and update state

    // if not, make a new async call

}

void SellerPeerPlugin::processFullPiece(const FullPiece * m) {
    throw std::exception("FullPiece message should never be sent to seller mode peer.");
}

void SellerPeerPlugin::processPayment(const Payment * m) {

    if(_clientState != ClientState::awaiting_payment)
        throw std::exception("Payment message should only be sent in response to a full piece message.");

    // _clientState values =>
    Q_ASSERT(_payee.state() = Payee::State::has_all_information_required);

    // Validate payment
    bool valid = _payee.registerPayment(m->sig());

    if(!valid) {

        // Update error state in some way

        throw std::exception("Invalid payment received.");
    }

    // Update state
    _clientState = ClientState::awaiting_piece_request_after_payment;
}

void SellerPeerPlugin::peerModeReset() {
    //Properly reset payee (state)?
}
