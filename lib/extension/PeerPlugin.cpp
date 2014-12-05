
#include "PeerPlugin.hpp"
#include "TorrentPlugin.hpp"
#include "Plugin.hpp"
#include "controller/Controller.hpp" // needed to connect
#include "Config.hpp"
#include "PeerPluginStatus.hpp"

#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/socket_io.hpp>
#include <libtorrent/peer_info.hpp>

PeerPlugin::PeerPlugin(TorrentPlugin * torrentPlugin, libtorrent::bt_peer_connection * bittorrentPeerConnection, QLoggingCategory & category)
    : torrentPlugin_(torrentPlugin)
    , bittorrentPeerConnection_(bittorrentPeerConnection)
    , peerBEP10SupportedStatus(unknown)
    , peerBEP43SupportedStatus(unknown)
    , peerPluginState_(PeerPluginState::started)
    , category_(category) {

    // Setup signals
    Controller * controller = torrentPlugin_->getPlugin()->getController();

    // peer added
    qRegisterMetaType<PeerPlugin *>();
    QObject::connect(this,
                     SIGNAL(peerAdded(PeerPlugin *)),
                     controller,
                     SLOT(extensionPeerAdded(PeerPlugin *)));

    // update status
    qRegisterMetaType<PeerPluginStatus>();
    QObject::connect(this,
                     SIGNAL(updatePeerPluginStatus(PeerPluginStatus)),
                     controller,
                     SLOT(updatePeerPluginStatus(PeerPluginStatus)));

    // Set endPointString_
    const boost::asio::ip::tcp::endpoint & endPoint = bittorrentPeerConnection_->remote();
    endPointString_ = libtorrent::print_endpoint(endPoint);
}

char const * PeerPlugin::type() const {
    return "";
}

/*
 * Can add entries to the extension handshake this is not called for web seeds
 */
void PeerPlugin::add_handshake(libtorrent::entry & handshake) {

    /**
      * We can safely assume hanshake has proper structure, that is
      * 1) is dictionary entry
      * 2) has key m which maps to a dictionary entry
      */

    // Add top level key for extesion version information
    handshake[EXTENSION_NAME] = 1; // write version, which is 1 for now

    // Add top level key for client identification
    QString clientIdentifier = QString(" ")
                                + QString::number(BITSWAPR_VERSION_MAJOR)
                                + QString(".")
                                + QString::number(BITSWAPR_VERSION_MINOR);

    handshake["v"] = clientIdentifier.toStdString();

    // Add m keys for extended message ids
    libtorrent::entry::dictionary_type & m = handshake["m"].dict();

    /**
      * Starting point from where to map:
      * =================================
      * So this is a bit of a mess. First I attempted to just look at all prexisting registrations,
      * and make sure that I started after the greatest one. However, we cannot be sure that add_handshake
      * is actually called last on our extension, and since the other extensions dont give a FUCK
      * about not overwriting other peoples extensions values. The only solution is then to just start
      * on some huge value which has no other extensions above it, so this value was found by trial and error.
      *
      * Old approach which did not work due to libtorrent
        int maxExistingID = 0;
        // Iterate m key dictionary and find the greatest ID
        for(std::map<std::string, libtorrent::entry>::iterator i = m.begin(),end(m.end());i != end;i++)
            maxExistingID = std::max((int)(((*i).second).integer()), maxExistingID);
      */

    // Set all ids from 60
    clientMapping.setAllStartingAt(60);

    // Write mapping to key
    clientMapping.writeToDictionary(m);
}

/*
 * This is called when the initial BASIC BT handshake is received.
 * Returning false means that the other end doesn't support this
 * extension and will remove it from the list of plugins.
 * This is not called for web seeds.
 *
 * The BEP10 docs say:
 * The bit selected for the extension protocol is bit 20 from
 * the right (counting starts at 0).
 * So (reserved_byte[5] & 0x10) is the expression to use for
 * checking if the client supports extended messaging.
 */
bool PeerPlugin::on_handshake(char const * reserved_bits) {

    //qCDebug(category_) << "on_handshake";

    // Check if BEP10 is enabled
    if(reserved_bits[5] & 0x10) {
        // Almost always here:
        //qCDebug(category_) << "BEP10 supported in handshake.";
        peerBEP10SupportedStatus = supported;
        return true;
    } else {
        qCDebug(category_) << "BEP10 not supported in handshake.";
        peerBEP10SupportedStatus = not_supported;
        return false;
    }
}

/*
 * Called when the extension handshake from the other end is received
 * if this returns false, it means that this extension isn't supported
 * by this peer. It will result in this peer_plugin being removed from
 * the peer_connection and destructed. this is not called for web seeds
 */
bool PeerPlugin::on_extension_handshake(libtorrent::lazy_entry const & handshake) {

    // Note that we have received extended handshake
    peerPluginState_ = PeerPluginState::handshake_received;

    // Write what client is trying to handshake us, should now be possible given initial hand shake
    libtorrent::peer_info peerInfo;
    bittorrentPeerConnection_->get_peer_info(peerInfo);
    qCDebug(category_) << "on_extension_handshake[" << peerInfo.client.c_str() << "]";

    // Check that BEP10 was actually supported, if
    // it wasnt, then the peer is misbehaving
    if(peerBEP10SupportedStatus != supported) {

        peerBEP43SupportedStatus = not_supported;
        qCWarning(category_) << "Peer didn't support BEP10, but it sent extended handshake.";
        return false;
    }

    /**
     * We cannot trust structure of entry, since it is from peer,
     * hence we must check it properly.
     */

    // If its not a dictionary, we are done
    if(handshake.type() != libtorrent::lazy_entry::dict_t) {

        peerBEP43SupportedStatus = not_supported;
        qCWarning(category_) << "Malformed handshake received: not dictionary.";
        return false;
    }

    // Check if plugin key is there
    int version = handshake.dict_find_int_value(EXTENSION_NAME,-1);

    if(version == -1) {
        qCDebug(category_) << "Extension not supported.";
        return false;
    } else
        qCDebug(category_) << "Extension version" << version << "supported.";

    // Try to extract m key, if its not present, then we are done
    const libtorrent::lazy_entry * m = handshake.dict_find_dict("m");
    if(!m) {

        peerBEP43SupportedStatus = not_supported;
        qCWarning(category_) << "Malformed handshake received: m key not present.";
        return false;
    }

    // Get peer mapping
    peerMapping = ExtendedMessageIdMapping(m);

    // Check that peer mapping is valid: all messages are present, and duplicate ids
    if(!peerMapping.isValid()) {

        peerBEP43SupportedStatus = not_supported;
        qCDebug(category_) << "m key does not contain mapping for all messages.";

        // DISCONNECT OR TELL TORRENT PLUGIN OR SOMETHING

        return false;
    }

    // Notify
    qCDebug(category_) << "Found extension handshake for peer " << endPointString_.c_str();

    // All messages were present, hence the protocol is supported
    peerBEP43SupportedStatus = supported;

    // Send signal about new peer
    emit peerAdded(this);

    // Tell libtorrent that our extension should be kept in the loop for this peer
    return true;
}

/*
 * m_pc.disconnect(errors::pex_message_too_large, 2);
 * m_pc.disconnect(errors::too_frequent_pex);
 * m_pc.remote().address()
 */
void PeerPlugin::on_disconnect(libtorrent::error_code const & ec) {

    //if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_disconnect";

}

void PeerPlugin::on_connected() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_connected";
*/

}

/*
 * Returning true from any of the message handlers indicates that the
 * plugin has handeled the message. it will break the plugin chain
 * traversing and not let anyone else handle the message, including the default handler.
 */

bool PeerPlugin::on_have(int index) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_have(" << index << ")";
        */

    return false;
}

bool PeerPlugin::on_bitfield(libtorrent::bitfield const & bitfield) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_bitfield";
*/
    return false;
}

bool PeerPlugin::on_have_all() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_have_all";
        */

    return false;
}

bool PeerPlugin::on_reject(libtorrent::peer_request const & peerRequest) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_reject";
        */

    return false;
}

bool PeerPlugin::on_request(libtorrent::peer_request const & peerRequest) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_request";
        */

    return false;
}

bool PeerPlugin::on_unchoke() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_unchoke";
        */

    return false;
}

bool PeerPlugin::on_interested() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_interested";
        */

    return false;
}

bool PeerPlugin::on_allowed_fast(int index) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_allowed_fast(" << index << ")";
        */

    return false;
}

bool PeerPlugin::on_have_none() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_have_none";
        */

    return false;
}

bool PeerPlugin::on_choke() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_choke";
        */

    return false;
}

bool PeerPlugin::on_not_interested() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_not_interested";
        */

    return false;
}

bool PeerPlugin::on_piece(libtorrent::peer_request const& piece, libtorrent::disk_buffer_holder & data) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_piece";
        */

    return false;
}

bool PeerPlugin::on_suggest(int index) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_suggest(" << index << ")";
        */

    return false;
}

bool PeerPlugin::on_cancel(libtorrent::peer_request const & peerRequest) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_cancel";
        */

    return false;
}

bool PeerPlugin::on_dont_have(int index) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_dont_have(" << index << ")";
        */

    return false;
}

/*
 * Called after a choke message has been sent to the peer
 */
void PeerPlugin::sent_unchoke() {
}

/*
 * Called when libtorrent think this peer should be disconnected.
 * If the plugin returns false, the peer will not be disconnected.
 */
bool PeerPlugin::can_disconnect(libtorrent::error_code const & ec) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "can_disconnect";
        */

    // CRITICAL
    return true;
}



/*
 * This is not called for web seeds
 */
bool PeerPlugin::on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body) {


    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "on_unknown_message(" << length << "," << msg << ")";


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
void PeerPlugin::tick() {

    qCDebug(category_) << "PeerPlugin.tick()";

    // Send signal
    PeerPluginStatus status;

    status.peerPlugin_ = this;
    status.peerPluginState_ = peerPluginState_;
    status.balance_ = 100;

    emit updatePeerPluginStatus(status);
}

/*
 * Called each time a request message is to be sent. If true is returned,
 * the original request message won't be sent and no other plugin will have this function called.
 */
bool PeerPlugin::write_request(libtorrent::peer_request const & peerRequest) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(category_) << "write_request";
        */

    return false;
}

const libtorrent::sha1_hash & PeerPlugin::getInfoHash() const {
    return torrentPlugin_->getInfoHash();
}

PeerPlugin::PEER_BEP_SUPPORTED_STATUS PeerPlugin::getPeerBEP10SupportedStatus() const {
    return peerBEP10SupportedStatus;
}

PeerPlugin::PEER_BEP_SUPPORTED_STATUS PeerPlugin::getPeerBEP43SupportedStatus() const {
    return peerBEP43SupportedStatus;
}

const boost::asio::ip::tcp::endpoint & PeerPlugin::getEndPoint() const {
    return bittorrentPeerConnection_->remote();
}

/*
const PeerPluginId & PeerPlugin::getPeerPluginId() const {
    return peerPluginId_;
}

bool PeerPlugin::operator<(PeerPlugin other) const {
    this->peerPluginId_ < other.getPeerPluginId();
}
*/
