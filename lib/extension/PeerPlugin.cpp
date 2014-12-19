
#include "PeerPlugin.hpp"
#include "TorrentPlugin.hpp"
#include "Plugin.hpp"
#include "controller/Controller.hpp" // needed to connect
#include "Config.hpp"
#include "PeerPluginStatus.hpp"
#include "PeerPluginRequest/PeerPluginRequest.hpp"

#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/socket_io.hpp>
#include <libtorrent/peer_info.hpp>

PeerPlugin::PeerPlugin(TorrentPlugin * torrentPlugin, libtorrent::bt_peer_connection * bittorrentPeerConnection, QLoggingCategory & category) //, PeerPluginConfiguration & peerPluginConfiguration)
    : _torrentPlugin(torrentPlugin)
    , _bittorrentPeerConnection(bittorrentPeerConnection)
    , _category(category) {
    //, _peerPluginConfiguration(peerPluginConfiguration) {
}

PeerPlugin::~PeerPlugin() {

    // Lets log, so we understand when libtorrent disposes of shared pointer
    qCDebug(_category) << "~PeerPlugin() called.";
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

    // Get
    //ExtendedMessageIdMapping & clientMapping = _peerPluginConfiguration.getClientMapping();

    // Set all ids from 60
    _clientMapping.setAllStartingAt(60);

    // Write mapping to key
    _clientMapping.writeToDictionary(m);
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

    //qCDebug(_category) << "on_handshake";

    // Get reference to BEP10 support status
    //BEPSupportStatus & peerBEP10SupportedStatus = _peerPluginConfiguration.getPeerBEP10SupportedStatus();

    // Check if BEP10 is enabled
    if(reserved_bits[5] & 0x10) {

        //qCDebug(_category) << "BEP10 supported in handshake.";
        _peerBEP10SupportedStatus = BEPSupportStatus::supported;
        return true;

    } else {

        qCDebug(_category) << "BEP10 not supported in handshake.";
        _peerBEP10SupportedStatus = BEPSupportStatus::not_supported;
        return false;

    }
}

/**
 * Called when the extension handshake from the other end is received
 * if this returns false, it means that this extension isn't supported
 * by this peer. It will result in this peer_plugin being removed from
 * the peer_connection and destructed. this is not called for web seeds
 */
bool PeerPlugin::on_extension_handshake(libtorrent::lazy_entry const & handshake) {

    // Note that we have received extended handshake
    //_peerPluginConfiguration.getPeerPluginState() = PeerPluginState::handshake_received;
    _peerPluginState = PeerPluginState::handshake_received;

    // Write what client is trying to handshake us, should now be possible given initial hand shake
    libtorrent::peer_info peerInfo;
    _bittorrentPeerConnection->get_peer_info(peerInfo);
    qCDebug(_category) << "on_extension_handshake[" << peerInfo.client.c_str() << "]";

    // Get reference to BEP43
    //BEPSupportStatus & peerBEP43SupportedStatus = _peerPluginConfiguration.getPeerBEP43SupportedStatus();

    // Check that BEP10 was actually supported, if it wasnt, then the peer is misbehaving
    //_peerPluginConfiguration.getPeerBEP10SupportedStatus() != BEPSupportStatus::supported
    if(_peerBEP10SupportedStatus != BEPSupportStatus::supported) {

        // Mark peer as not supporting BEP43
        _peerBEP43SupportedStatus = BEPSupportStatus::not_supported;
        qCWarning(_category) << "Peer didn't support BEP10, but it sent extended handshake.";

        // Remember that this peer does not have extension
        _torrentPlugin->addToPeersWithoutExtensionSet(peerInfo.ip);

        // Remove this plugin from torrent plugin
        _torrentPlugin->removePeerPlugin(this);

        // Do no keep extension around
        return false;
    }

    /**
     * We cannot trust structure of entry, since it is from peer,
     * hence we must check it properly.
     */

    // If its not a dictionary, we are done
    if(handshake.type() != libtorrent::lazy_entry::dict_t) {

        // Mark peer as not supporting BEP43
        _peerBEP43SupportedStatus = BEPSupportStatus::not_supported;
        qCWarning(_category) << "Malformed handshake received: not dictionary.";

        // Remember that this peer does not have extension
        _torrentPlugin->addToIrregularPeersSet(peerInfo.ip);

        // Remove this plugin from torrent plugin
        _torrentPlugin->removePeerPlugin(this);

        // Do no keep extension around
        return false;
    }

    // Check if plugin key is there
    int version = handshake.dict_find_int_value(EXTENSION_NAME,-1);

    if(version == -1) {

        qCDebug(_category) << "Extension not supported.";

        // Remember that this peer does not have extension
        _torrentPlugin->addToPeersWithoutExtensionSet(peerInfo.ip);

        // Remove this plugin from torrent plugin
        _torrentPlugin->removePeerPlugin(this);

        // Do no keep extension around
        return false;

    } else
        qCDebug(_category) << "Extension version" << version << "supported.";

    // Try to extract m key, if its not present, then we are done
    const libtorrent::lazy_entry * m = handshake.dict_find_dict("m");
    if(!m) {

        // Mark peer as not supporting BEP43
        _peerBEP43SupportedStatus = BEPSupportStatus::not_supported;
        qCWarning(_category) << "Malformed handshake received: m key not present.";

        // Remember that this peer does not have extension
        _torrentPlugin->addToIrregularPeersSet(peerInfo.ip);

        // Remove this plugin from torrent plugin
        _torrentPlugin->removePeerPlugin(this);

        // Do no keep extension around
        return false;
    }

    // Get peer mapping
    ExtendedMessageIdMapping peerMapping(m);

    // Set peer mapping in configuration
    //_peerPluginConfiguration.setPeerMapping(peerMapping);
    _peerMapping = peerMapping;

    // Check that peer mapping is valid: all messages are present, and duplicate ids
    if(!_peerMapping.isValid()) {

        // Mark peer as not supporting BEP43
        _peerBEP43SupportedStatus = BEPSupportStatus::not_supported;
        qCDebug(_category) << "m key does not contain mapping for all messages.";

        // Remember that this peer does not have extension
        _torrentPlugin->addToIrregularPeersSet(peerInfo.ip);

        // Remove this plugin from torrent plugin
        _torrentPlugin->removePeerPlugin(this);

        // Do no keep extension around
        return false;
    }

    // Notify
    std::string endPointString = libtorrent::print_endpoint(peerInfo.ip);
    qCDebug(_category) << "Found extension handshake for peer " << endPointString.c_str();

    // All messages were present, hence the protocol is supported
    _peerBEP43SupportedStatus = BEPSupportStatus::supported;

    // Tell libtorrent that our extension should be kept in the loop for this peer
    return true;
}

bool PeerPlugin::on_have(int index) {

    qCDebug(_category) << "on_have";
    return false;
}

/*
 * m_pc.disconnect(errors::pex_message_too_large, 2);
 * m_pc.disconnect(errors::too_frequent_pex);
 * m_pc.remote().address()
 */

void PeerPlugin::on_disconnect(libtorrent::error_code const & ec) {

    qCDebug(_category) << "on_disconnect";

    // Remove from torrent plugin
    _torrentPlugin->removePeerPlugin(this);
}

void PeerPlugin::on_connected() {

}

bool PeerPlugin::on_bitfield(libtorrent::bitfield const & bitfield) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_bitfield";
*/
    return false;
}

bool PeerPlugin::on_have_all() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_have_all";
        */

    return false;
}

bool PeerPlugin::on_reject(libtorrent::peer_request const & peerRequest) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_reject";
        */

    return false;
}

bool PeerPlugin::on_request(libtorrent::peer_request const & peerRequest) {

    //bittorrentPeerConnection_->incoming_request();

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_request";
        */

    return false;
}

bool PeerPlugin::on_unchoke() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_unchoke";
        */

    return false;
}

bool PeerPlugin::on_interested() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_interested";
        */

    return false;
}

bool PeerPlugin::on_allowed_fast(int index) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_allowed_fast(" << index << ")";
        */

    return false;
}

bool PeerPlugin::on_have_none() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_have_none";
        */

    return false;
}

bool PeerPlugin::on_choke() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_choke";
        */

    return false;
}

bool PeerPlugin::on_not_interested() {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_not_interested";
        */

    return false;
}

bool PeerPlugin::on_piece(libtorrent::peer_request const& piece, libtorrent::disk_buffer_holder & data) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_piece";
        */

    return false;
}

bool PeerPlugin::on_suggest(int index) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_suggest(" << index << ")";
        */

    return false;
}

bool PeerPlugin::on_cancel(libtorrent::peer_request const & peerRequest) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_cancel";
        */

    return false;
}

bool PeerPlugin::on_dont_have(int index) {

    /*
    if(peerBEP43SupportedStatus != not_supported)
        qCDebug(_category) << "on_dont_have(" << index << ")";
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
        qCDebug(_category) << "can_disconnect";
        */

    // CRITICAL
    return true;
}

// Called when an extended message is received. If returning true,
// the message is not processed by any other plugin and if false
// is returned the next plugin in the chain will receive it to
// be able to handle it this is not called for web seeds.
bool PeerPlugin::on_extended(int length, int msg, libtorrent::buffer::const_interval body) {

    // if(_peerPluginConfiguration.getPeerBEP43SupportedStatus() != BEPSupportStatus::not_supported)
    if(_peerBEP43SupportedStatus != BEPSupportStatus::not_supported)
        qCDebug(_category) << "buyer:on_extended(" << length << "," << msg << ")";

    // Update peerPluginState_

    // Create QByte array where you put data, then wrap witha stream, after mesage has b een parsed, check that pointer received is not null.

    // CRITICAL
    return false;
}

/*
 * This is not called for web seeds
 */
bool PeerPlugin::on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body) {

    // if(_peerPluginConfiguration.getPeerBEP43SupportedStatus() != BEPSupportStatus::not_supported)
    if(_peerBEP43SupportedStatus != BEPSupportStatus::not_supported)
        qCDebug(_category) << "on_unknown_message(" << length << "," << msg << ")";

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

    qCDebug(_category) << "PeerPlugin.tick()";

    // Send signal
    //PeerPluginStatus status(_peerPluginConfiguration.getPeerPluginId(), _peerPluginConfiguration.getPeerPluginState(), 0);
    PeerPluginStatus status(_peerPluginId, _peerPluginState, 0);

    //emit peerPluginStatusUpdated(status);
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

void PeerPlugin::processPeerPluginRequest(const PeerPluginRequest * peerPluginRequest) {

    qCDebug(_category) << "processPeerPluginRequest";

    switch(peerPluginRequest->getPeerPluginRequestType()) {

    }
}

void PeerPlugin::startPlugin(StartedPluginMode pluginMode) {

    // Start plugin
    _pluginStarted = true;

    // Set mode
    _pluginStartedMode = pluginMode;

    // Do something
    switch(_pluginStartedMode) {

        case StartedPluginMode::Buyer:

        // Check what you have received so far, and send some message

        break;

        case StartedPluginMode::Seller:

        // Check what you have received so far, and send some message

        break;

        case StartedPluginMode::Passive:

        // ?, send not interested mesasge or something?

        break;
    }
}

/*
void PeerPlugin::setConfiguration(PeerPluginConfiguration * peerPluginConfiguration) {


    // We have now started
    _pluginStarted = true;

    // Set configuration
    _torrentPluginConfiguration
}
*/

/*
const libtorrent::sha1_hash & PeerPlugin::getInfoHash() const {
    return torrentPlugin_->getInfoHash();
}

PeerPlugin::PEER_BEP_SUPPORTED_STATUS PeerPlugin::getPeerBEP10SupportedStatus() const {
    return _peerBEP10SupportedStatus;
}

PeerPlugin::PEER_BEP_SUPPORTED_STATUS PeerPlugin::getPeerBEP43SupportedStatus() const {
    return _peerBEP43SupportedStatus;
}

const libtorrent::tcp::endpoint & PeerPlugin::getEndPoint() const {
    return _bittorrentPeerConnection->remote();
}

const PeerPluginId & PeerPlugin::getPeerPluginId() const {
    return peerPluginId_;
}
*/
