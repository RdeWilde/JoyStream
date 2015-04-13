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
                                       const Buy & lastBuySent,
                                       const SignRefund & lastSignRefundSent,
                                       const Payment & lastPaymentSent)
    : _lastAction(lastAction)
    , _failureMode(failureMode)
    , _lastBuySent(lastBuySent)
    , _lastSignRefundSent(lastSignRefundSent)
    , _lastPaymentSent(lastPaymentSent) {
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

Buy SellerPeerPlugin::PeerState::lastBuySent() const {
    return _lastBuySent;
}

void SellerPeerPlugin::PeerState::setLastBuySent(const Buy & lastBuySent) {
    _lastBuySent = lastBuySent;
}

SignRefund SellerPeerPlugin::PeerState::lastSignRefundSent() const {
    return _lastSignRefundSent;
}

void SellerPeerPlugin::PeerState::setLastSignRefundSent(const SignRefund & lastSignRefundSent) {
    _lastSignRefundSent = lastSignRefundSent;
}

Payment SellerPeerPlugin::PeerState::lastPaymentSent() const {
    return _lastPaymentSent;
}

void SellerPeerPlugin::PeerState::setLastPaymentSent(const Payment & lastPaymentSent) {
    _lastPaymentSent = lastPaymentSent;
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

#include <libtorrent/bt_peer_connection.hpp>

#include <QLoggingCategory>

SellerPeerPlugin::SellerPeerPlugin(SellerTorrentPlugin * torrentPlugin,
                                   libtorrent::bt_peer_connection * connection,
                                   quint32 minPrice,
                                   quint32 minLock,
                                   quint32 maxSellers,
                                   const PublicKey & contractPk,
                                   const PublicKey & finalPk,
                                   QLoggingCategory & category)
    : PeerPlugin(torrentPlugin, connection, category)
    , _plugin(torrentPlugin)
    , _clientState(ClientState::no_bitswapr_message_sent)
    , _minPrice(minPrice)
    , _minLock(minLock)
    , _maxSellers(maxSellers)
    , _contractPk(contractPk)
    , _finalPk(finalPk) {
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
        sendExtendedMessage(Sell(_plugin->minPrice(), _plugin->minLock()));

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

    // May arrive at any time, hence no _peerState.lastAction() invariant to check

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
    _peerState.setLastBuySent(*m);

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

    if(_clientState != ClientState::seller_mode_announced ||
       _clientState != ClientState::ignored_contract_invitation)
        throw std::exception("JoinContract message should only be sent in response to an announced seller mode.");

    // If peer has not been invited, and plugin says ok,
    // then we send invite
    if(_minPrice <= m->maxPrice() &&
       _minLock <= m->maxLock() &&
       _maxSellers >= m->minSellers()) {

        // invite to join contract
        sendExtendedMessage(JoiningContract(_payeeContractKeys.pk(), _payeePaymentKeys.pk()));

        // and remember invitation
        _clientState = ClientState::joined_contract;

    } else {

        _clientState = ClientState::ignored_contract_invitation;

        qCDebug(_category) << "Did not invite seller, terms were incompatible.";
    }
}

void SellerPeerPlugin::processJoiningContract(const JoiningContract * m) {
    throw std::exception("JoiningContract message should never be sent to seller mode peer.");
}

void SellerPeerPlugin::processSignRefund(const SignRefund * m) {

    if(_clientState != ClientState::joined_contract)
        throw std::exception("SignRefund message should only be sent in response to joining the contract.");

    // Save sign refund message
    _peerState.setLastSignRefundSent(*m);

    //
    _payee.registerPayeeInformation(quint32 lockTime, quint32 price, quint32 maximumNumberOfSellers, const KeyPair & payeeContractKeys, const KeyPair & payeePaymentKeys);

    // Save payor information in payee
    _payee.registerPayorInformation(OutPoint(m->hash(), m->index()), contract pk ?, m->pk(), m->);

    // Create refund signature
    Signature refundSignature = _payee.generateRefundSignature();

    // Send signed_refund message
    sendExtendedMessage(RefundSigned(refundSignature));

    // Update state
    _clientState = ClientState::signed_refund;
}

void SellerPeerPlugin::processRefundSigned(const RefundSigned * m) {
    throw std::exception("RefundSigned message should never be sent to seller mode peer.");
}

void SellerPeerPlugin::processReady(const Ready * m) {
    qCDebug(_category) << "Ready.";
}

void SellerPeerPlugin::processRequestFullPiece(const RequestFullPiece * m) {


    // service request?

    // get piece

    //_torrent.filesystem().async_read(r, boost::bind(&smart_ban_plugin::on_read_ok_block, shared_from_this(), *i, _1, _2));

    // send piece message

    // mark if we have sent a full piece??

}

void SellerPeerPlugin::processFullPiece(const FullPiece * m) {
    throw std::exception("FullPiece message should never be sent to seller mode peer.");
}

void SellerPeerPlugin::processPayment(const Payment * m) {

    // Check state

    // check that payment is valid

    // Update state

}
