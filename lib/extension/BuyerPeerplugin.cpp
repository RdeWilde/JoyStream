#include "BuyerPeerPlugin.hpp"

#include "BitCoin/PublicKey.hpp"
#include "BitCoin/Hash.hpp"
#include "BitCoin/Signature.hpp"


#include <QLoggingCategory> // qCDebug

BuyerPeerPlugin::BuyerPeerPlugin(TorrentPlugin * torrentPlugin,
                                 libtorrent::bt_peer_connection * bittorrentPeerConnection,
                                 QLoggingCategory & category)
    : PeerPlugin(torrentPlugin, bittorrentPeerConnection,category)
    , _lastPeerAction(PeerAction::not_acted)
    , _peerPluginModeObserved(false)
    , _invitedToJoinContract(false)
    , _invitedToSignRefund(false)
    , _failedToSignRefund(false) {
}

/*
 * Called when an extended message is received. If returning true,
 * the message is not processed by any other plugin and if false
 * is returned the next plugin in the chain will receive it to
 * be able to handle it this is not called for web seeds.
 */
bool BuyerPeerPlugin::on_extended(int length, int msg, libtorrent::buffer::const_interval body) {

    /**
    // No processing is done before a successful extended handshake
    if(_peerBitSwaprBEPSupportedStatus != BEPSupportStatus::supported)
        return;
    */

    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "buyer:on_extended(" << length << "," << msg << ")";

    // Update peerPluginState_

    // Create QByte array where you put data, then wrap witha stream, after mesage has b een parsed, check that pointer received is not null.

    // CRITICAL
    return false;
}

/*
 * Called aproximately once every second
 */
void BuyerPeerPlugin::tick() {

    // call parent tick() also?

    qCDebug(category_) << "BuyerPeerPlugin.tick()";
}


bool BuyerPeerPlugin::on_have(int index) {

    qCDebug(_category) << "on_have";
    return false;
}

/*
 * m_pc.disconnect(errors::pex_message_too_large, 2);
 * m_pc.disconnect(errors::too_frequent_pex);
 * m_pc.remote().address()
 */

void BuyerPeerPlugin::on_disconnect(libtorrent::error_code const & ec) {

    qCDebug(_category) << "on_disconnect";

    _isConnected = false;
}

void BuyerPeerPlugin::on_connected() {

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

bool PeerPlugin::processBuy(const Buy * buyMessage) {

    // Check that message is compatible with last peer action
    if(_lastPeerAction != PeerAction::extended_handshake_completed &&
       _lastPeerAction != PeerAction::observe_mode_announced &&
       _lastPeerAction != PeerAction::buy_mode_announced &&
       _lastPeerAction != PeerAction::sell_mode_announced &&
       _lastPeerAction != PeerAction::ended)
        return false;

    // Update last peer action
    _lastPeerAction = PeerAction::buy_mode_announced;

    // Note that peer is buyer
    _peerPluginMode = PluginMode::Buy;


    /**
     * Do later
     */


    // Message was valid
    return true;
}

bool PeerPlugin::processSell(const Sell * m) {

    // Only ok if last action by peer was handshake/mode message or end
    if(_lastPeerAction != PeerAction::extended_handshake_completed &&
       _lastPeerAction != PeerAction::observe_mode_announced &&
       _lastPeerAction != PeerAction::buy_mode_announced &&
       _lastPeerAction != PeerAction::sell_mode_announced &&
       _lastPeerAction != PeerAction::ended)
        return false;

    // Update last peer action
    _lastPeerAction = PeerAction::sell_mode_announced;

    // Note that peer is seller
    _peerPluginMode = PluginMode::Sell;

    /**
     * The only spesific processing is if we are in buy mode
     */
    if(_clientPluginMode == PluginMode::Buy) {

        // Save last terms
        _bsellerMinPrice = m->minPrice();
        _bsellerMinLock = m->minLock();

        // If we are building payment channel,
        // and peer has not been invited,
        // and peer has sufficiently good terms,
        // then invite to join contract
        if(_torrentPlugin->buyerTorrentPluginState() == BuyerTorrentPluginState::populating_payment_channel &&
            !_invitedToJoinContract &&
            m->minPrice() < _torrentPlugin->config()->_maxPrice &&
            m->minLock() < _torrentPlugin->config()->_maxLock) {

            // Invite to join contract
            plugin->sendExtendedMessage(JoinContract());

            // and remember invite
            _invitedToJoinContract = true;
        }
    }

    // Message was valid
    return true;
}

bool PeerPlugin::processJoinContract(const JoinContract * m) {

    // Check mode of peer is compatible with message

}

bool PeerPlugin::processJoiningContract(const JoiningContract * m) {

    // Only ok if last action by peer was to set sell mode, and peer has been invited to join contract
    if(_lastPeerAction != PeerAction::sell_mode_announced ||
       !_invitedToJoinContract) // <== change?
        return false;

    // Update last peer action
    _lastPeerAction = PeerAction::sell_mode_announced;



}

bool PeerPlugin::processSignRefund(const SignRefund * m) {

    // Check mode of peer is compatible with message

}

bool PeerPlugin::processRefundSigned(const RefundSigned * m) {

    // Check mode of peer is compatible with message

}

bool PeerPlugin::processReady(const Ready * m) {

    // Check mode of peer is compatible with message

}

bool PeerPlugin::processPayment(const Payment * m) {

    // Check mode of peer is compatible with message

}

bool PeerPlugin::processEnd(const End * m) {

    // Check mode of peer is compatible with message

}


void BuyerPeerPlugin::sendStatusToController() {

    // Send status to controller
    //PeerPluginStatus status(_peerPluginConfiguration.getPeerPluginId(), _peerPluginConfiguration.getPeerPluginState(), 0);
    PeerPluginStatus status(_peerPluginId, _lastPeerAction, 0);

    //emit peerPluginStatusUpdated(status);
}
