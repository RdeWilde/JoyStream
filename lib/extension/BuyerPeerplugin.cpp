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
    , _pK(pK){
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

PublicKey BuyerPeerPlugin::PeerState::pK() const {
    return _pK;
}

void BuyerPeerPlugin::PeerState::setPK(const PublicKey & pK) {
    _pK = pK;
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

BuyerPeerPlugin::ClientState BuyerPeerPlugin::Status::clientState() const {
    return _clientState;
}

void BuyerPeerPlugin::Status::setClientState(ClientState clientState) {
    _clientState = clientState;
}

BuyerPeerPlugin::PeerState BuyerPeerPlugin::Status::peerState() const {
    return _peerState;
}

void BuyerPeerPlugin::Status::setPeerState(const PeerState & peerState) {
    _peerState = peerState;
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
                                              ClientState clientState)
    : PeerPlugin::Configuration(clientMapping, peerMapping, peerBEP10SupportStatus, peerBitSwaprBEPSupportStatus)
    , _peerState(peerState)
    , _clientState(clientState) {
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
#include "Message/Buy.hpp"
#include "Message/Sell.hpp"
#include "Message/JoinContract.hpp"

#include <QLoggingCategory>

BuyerPeerPlugin::BuyerPeerPlugin(BuyerTorrentPlugin * plugin,
                                 libtorrent::bt_peer_connection * connection,
                                 const Configuration & configuration,
                                 QLoggingCategory & category)
    : PeerPlugin(plugin, connection, configuration, category)
    , _plugin(plugin)
    , _peerState(configuration.peerState())
    , _clientState(configuration.clientState()) {
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
        sendExtendedMessage(Buy(_plugin->payor().maxPrice(), _plugin->payor().maxLock()));

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

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_piece";
        */

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

}

/*
 * Called when a piece that this peer participated in fails the hash_check
 */
void BuyerPeerPlugin::on_piece_failed(int index) {

}

/*
 * Called aproximately once every second
 */
void BuyerPeerPlugin::tick() {

    qCDebug(_category) << "BuyerPeerPlugin.tick()";
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

BuyerPeerPlugin::Status BuyerPeerPlugin::status() const {
    return Status(_peerState, _clientState);
}

void BuyerPeerPlugin::processObserve(const Observe * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // Update peer state with new valid action
    _peerState.setLastAction(PeerState::LastValidAction::mode_announced);

    // Note that peer is in observe mode
    _peerModeAnnounced = PeerModeAnnounced::observer;

    // We dont do anything else, since we cant buy from peer in observer mode
}

void BuyerPeerPlugin::processBuy(const Buy * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // Update peer state with new valid action
    _peerState.setLastAction(PeerState::LastValidAction::mode_announced);

    // Note that peer is in buyer mode
    _peerModeAnnounced = PeerModeAnnounced::buyer;

    // We dont do anything else, since we cant buy from peer in buyer mode
}

void BuyerPeerPlugin::processSell(const Sell * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // Update peer state with new valid action
    _peerState.setLastAction(PeerState::LastValidAction::mode_announced);
    _peerState.setMinPrice(m->minPrice());
    _peerState.setMinLock(m->minLock());

    // Note that peer is seller
    _peerModeAnnounced = PeerModeAnnounced::seller;

    // If we are building payment channel,
    // and peer has not been invited,
    // and peer has sufficiently good terms,
    // then
    if(_plugin->state() == BuyerTorrentPlugin::State::waiting_for_payor_to_be_ready &&
            _clientState == ClientState::no_bitswapr_message_sent &&
            m->minPrice() < _plugin->payor().maxPrice() &&
            m->minLock() < _plugin->payor().maxLock()) {

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

    // Update peer state with new valid action
    _peerState.setLastAction(PeerState::LastValidAction::joined_contract);
}

void BuyerPeerPlugin::processSignRefund(const SignRefund * m) {
    throw std::exception("SignRefund message should never be sent to buyer mode peer.");
}

void BuyerPeerPlugin::processRefundSigned(const RefundSigned * m) {

    // Check mode of peer is compatible with message

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
BuyerPeerPlugin::Configuration BuyerPeerPlugin::configuration() const {
    return _configuration;
}
*/

PluginMode BuyerPeerPlugin::mode() const {
    return PluginMode::Buyer;
}

/*
void BuyerPeerPlugin::sendStatusToController() {

    // Send status to controller
    //PeerPluginStatus status(_peerPluginConfiguration.getPeerPluginId(), _peerPluginConfiguration.getPeerPluginState(), 0);
    PeerPluginStatus status(_peerPluginId, _lastPeerAction, 0);

    //emit peerPluginStatusUpdated(status);
}
*/
