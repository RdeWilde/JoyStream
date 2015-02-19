// ddddd Configuration

#include "SellerPeerPlugin.hpp"
#include "PluginMode.hpp"

#include <QLoggingCategory>

SellerPeerPlugin::SellerPeerPlugin(TorrentPlugin * torrentPlugin,
                                   libtorrent::bt_peer_connection * bittorrentPeerConnection,
                                   QLoggingCategory & category)
      : PeerPlugin(torrentPlugin, bittorrentPeerConnection, category) {
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

    _connectionAlive = false;
}

void SellerPeerPlugin::on_connected() {

}

bool SellerPeerPlugin::on_have(int index) {

    qCDebug(_category) << "on_have";
    return false;
}

bool SellerPeerPlugin::on_bitfield(libtorrent::bitfield const & bitfield) {
    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_bitfield";
*/
    return false;
}

bool SellerPeerPlugin::on_have_all()  {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_have_all";
        */

    return false;
}

bool SellerPeerPlugin::on_reject(libtorrent::peer_request const & peerRequest) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_reject";
        */

    return false;
}

bool SellerPeerPlugin::on_request(libtorrent::peer_request const & peerRequest)  {

    //bittorrentPeerConnection_->incoming_request();

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_request";
        */

    return false;
}

bool SellerPeerPlugin::on_unchoke()  {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_unchoke";
        */

    return false;
}

bool SellerPeerPlugin::on_interested()  {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_interested";
        */

    return false;
}

bool SellerPeerPlugin::on_allowed_fast(int index)  {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_allowed_fast(" << index << ")";
        */

    return false;
}

bool SellerPeerPlugin::on_have_none()  {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_have_none";
        */

    return false;
}

bool SellerPeerPlugin::on_choke()  {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_choke";
        */

    return false;
}

bool SellerPeerPlugin::on_not_interested()  {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_not_interested";
        */

    return false;
}

bool SellerPeerPlugin::on_piece(libtorrent::peer_request const & piece, libtorrent::disk_buffer_holder & data)  {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_piece";
        */

    return false;
}

bool SellerPeerPlugin::on_suggest(int index)  {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_suggest(" << index << ")";
        */

    return false;
}

bool SellerPeerPlugin::on_cancel(libtorrent::peer_request const & peerRequest)  {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_cancel";
        */

    return false;
}

bool SellerPeerPlugin::on_dont_have(int index)  {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_dont_have(" << index << ")";
        */

    return false;
}

/*
 * Called after a choke message has been sent to the peer
 */
void SellerPeerPlugin::sent_unchoke()  {

}

/*
 * Called when libtorrent think this peer should be disconnected.
 * If the plugin returns false, the peer will not be disconnected.
 */
bool SellerPeerPlugin::can_disconnect(libtorrent::error_code const & ec)  {

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
bool SellerPeerPlugin::on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body)  {

    // CRITICAL
    return false;
}

/*
 * Called when a piece that this peer participated in passes the hash_check
 */
void SellerPeerPlugin::on_piece_pass(int index)  {

}

/*
 * Called when a piece that this peer participated in fails the hash_check
 */
void SellerPeerPlugin::on_piece_failed(int index)  {

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

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "write_request";
    */

    return false;
}

PluginMode SellerPeerPlugin::mode() const {
    return PluginMode::Seller;
}
