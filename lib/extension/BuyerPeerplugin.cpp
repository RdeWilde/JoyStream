#include "BuyerPeerPlugin.hpp"

BuyerPeerPlugin::Configuration::Peer::Peer()
    : _lastAction(LastValidAction::no_bitswapr_message_sent)
    , _failureMode(FailureMode::not_failed) {
}

BuyerPeerPlugin::Configuration::Peer::LastValidAction BuyerPeerPlugin::PeerState::lastAction() const {
    return _lastAction;
}

void BuyerPeerPlugin::Configuration::Peer::setLastAction(const LastValidAction &lastAction) {
    _lastAction = lastAction;
}

BuyerPeerPlugin::Configuration::Peer::FailureMode BuyerPeerPlugin::PeerState::failureMode() const {
    return _failureMode;
}

void BuyerPeerPlugin::Configuration::Peer::setFailureMode(const FailureMode & failureMode) {
    _failureMode = failureMode;
}

quint64 BuyerPeerPlugin::Configuration::Peer::minPrice() const {
    return _minPrice;
}

void BuyerPeerPlugin::Configuration::Peer::setMinPrice(const quint64 &minPrice) {
    _minPrice = minPrice;
}

PublicKey BuyerPeerPlugin::Configuration::Peer::pK() const {
    return _pK;
}

void BuyerPeerPlugin::Configuration::Peer::setPK(const PublicKey &pK) {
    _pK = pK;
}

quint32 BuyerPeerPlugin::Configuration::Peer::minLock() const {
    return _minLock;
}

void BuyerPeerPlugin::Configuration::Peer::setMinLock(const quint32 &minLock) {
    _minLock = minLock;
}

BuyerPeerPlugin::Configuration::Peer BuyerPeerPlugin::Configuration::peerState() const {
    return _peer;
}

void BuyerPeerPlugin::Configuration::setPeer(const Peer &peer) {
    _peer = peer;
}

BuyerPeerPlugin::Configuration::Client BuyerPeerPlugin::Configuration::clientState() const {
    return _client;
}

void BuyerPeerPlugin::Configuration::setClient(const Client &client) {
    _client = client;
}

#include "BuyerTorrentPlugin.hpp"
#include "PluginMode.hpp"

#include <QLoggingCategory>

BuyerPeerPlugin::BuyerPeerPlugin(BuyerTorrentPlugin * plugin, libtorrent::bt_peer_connection * connection, QLoggingCategory & category, , const Configuration & configuration)
    : PeerPlugin(plugin, connection, category)
    , _plugin(plugin)
    , _configuration(configuration)
    , _lastMessageStateCompatibility(MessageStateCompatibility::compatible){
}

BuyerPeerPlugin::~BuyerPeerPlugin() {

}

char const * PeerPlugin::type() const {
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

        // get buyer torrent plugin configurations
        BuyerTorrentPluginConfiguration configuration = _plugin->configuration();

        // send mode message
        sendExtendedMessage(Buy(configuration.maxPrice(), configuration.maxLock());

        // and update new client state correspondingly
        _configuration.setClient(Configuration::Client::buyer_mode_announced);
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
bool PeerPlugin::on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body) {

    // CRITICAL
    return false;
}

/*
 * Called when a piece that this peer participated in passes the hash_check
 */
void PeerPlugin::on_piece_pass(int index) {

}

/*
 * Called when a piece that this peer participated in fails the hash_check
 */
void PeerPlugin::on_piece_failed(int index) {

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
bool PeerPlugin::write_request(libtorrent::peer_request const & peerRequest) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "write_request";
    */

    return false;
}

void BuyerPeerPlugin::processObserve(const Observe * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // Update peer state with new valid action
    _configuration.peerState().setLastAction(Configuration::Peer::LastValidAction::mode_announced);

    // Note that peer is in observe mode
    _peerModeAnnounced = PeerModeAnnounced::observer;

    // We dont do anything else, since we cant buy from peer in observer mode
}

void BuyerPeerPlugin::processBuy(const Buy * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // Update peer state with new valid action
    _configuration.peerState().setLastAction(Configuration::Peer::LastValidAction::mode_announced);

    // Note that peer is in observe mode
    _peerModeAnnounced = PeerModeAnnounced::buyer;

    // We dont do anything else, since we cant buy from peer in buyer mode
}

void BuyerPeerPlugin::processSell(const Sell * m) {

    // Do processing in response to mode reset
    peerModeReset();

    // do something clever about state processing.

    // Update peer state with new valid action
    Configuration::Peer peerState = _configuration.peerState();
    peerState.setLastAction(Configuration::Peer::LastValidAction::mode_announced);
    peerState.setMinPrice(m->minPrice());
    peerState.setMinLock(m->minLock());

    // Note that peer is seller
    _peerModeAnnounced = PeerModeAnnounced::seller;

    // Get buyer torrent plugin configurations
    BuyerTorrentPlugin::Configuration configuration = _plugin->configuration();

    // If we are building payment channel,
    // and peer has not been invited,
    // and peer has sufficiently good terms,
    // then
    if(configuration.Stage == BuyerTorrentPlugin::Configuration::Stage::building_contract &&
            _configuration.client() == Configuration::Client::no_bitswapr_message_sent &&
            m->minPrice() < configuration.maxPrice() &&
            m->minLock() < configuration.maxLock()) {

        // invite to join contract
        sendExtendedMessage(JoinContract());

        // and remember invitation
        _configuration.setClient(Configuration::Client::invited_to_contract);
    }
}

void BuyerPeerPlugin::processJoinContract(const JoinContract * m) {
    qCDebug(_category) << "JoinContract is state incompatible.";
    _lastMessageWasStateIncompatible = true;
}

void BuyerPeerPlugin::processJoiningContract(const JoiningContract * m) {

    // Only ok if last action by peer was to set sell mode, and peer has been invited to join contract
    if(_lastPeerAction != PeerAction::sell_mode_announced ||
       !_invitedToJoinContract) // <== change?
        return false;

    // Update last peer action
    _peerState = PeerState::joined_contract;
}

void BuyerPeerPlugin::processSignRefund(const SignRefund * m) {
    qCDebug(_category) << "SignRefund is state incompatible.";
    _lastMessageWasStateIncompatible = true;
}

void BuyerPeerPlugin::processRefundSigned(const RefundSigned * m) {

    // Check mode of peer is compatible with message

}

void BuyerPeerPlugin::processReady(const Ready * m) {
    qCDebug(_category) << "Ready is state incompatible.";
    _lastMessageWasStateIncompatible = true;
}

void BuyerPeerPlugin::processPayment(const Payment * m) {

    qCDebug(_category) << "Payment is state incompatible.";
    _lastMessageWasStateIncompatible = true;
}

void BuyerPeerPlugin::peerModeReset() {

}

BuyerPeerPlugin::Configuration BuyerPeerPlugin::configuration() const {
    return _configuration;
}

PluginMode BuyerPeerPlugin::mode() const {
    return PluginMode::Buy;
}

/*
void BuyerPeerPlugin::sendStatusToController() {

    // Send status to controller
    //PeerPluginStatus status(_peerPluginConfiguration.getPeerPluginId(), _peerPluginConfiguration.getPeerPluginState(), 0);
    PeerPluginStatus status(_peerPluginId, _lastPeerAction, 0);

    //emit peerPluginStatusUpdated(status);
}
*/
