#include "PeerPlugin.hpp"
#include "TorrentPlugin.hpp"
#include "Plugin.hpp"
#include "Config.hpp"
#include "PeerPluginStatus.hpp"
#include "Request/PeerPluginRequest.hpp"
#include "Message/MessageType.hpp"
#include "Message/ExtendedMessagePayload.hpp"
#include "Message/Observe.hpp"
#include "Message/Buy.hpp"
#include "Message/Sell.hpp"
#include "Message/JoinContract.hpp"
#include "Message/JoiningContract.hpp"
#include "Message/SignRefund.hpp"
#include "Message/RefundSigned.hpp"
#include "Message/Ready.hpp"
#include "Message/Payment.hpp"
#include "Message/End.hpp"
#include "Utilities.hpp"

#include <libtorrent/bt_peer_connection.hpp> // bt_peer_connection, bt_peer_connection::msg_extended
#include <libtorrent/socket_io.hpp>
#include <libtorrent/peer_info.hpp>

#include <QLoggingCategory>

PeerPlugin::PeerPlugin(TorrentPlugin * torrentPlugin, libtorrent::bt_peer_connection * bittorrentPeerConnection, QLoggingCategory & category) //, PeerPluginConfiguration & peerPluginConfiguration)
    : _torrentPlugin(torrentPlugin)
    , _bittorrentPeerConnection(bittorrentPeerConnection)
    , _category(category)
    , _peerPluginModeObserved(false)
    , _isConnected(true)
    , _lastPeerMessageWasValid(true)
    , _peerBEP10SupportedStatus(BEPSupportStatus::unknown)
    , _peerBitSwaprBEPSupportedStatus (BEPSupportStatus::unknown)
    , _lastPeerAction(PeerAction::not_acted)
    , _peerPluginModeObserved(false)
    , _invitedToJoinContract(false)
    , _invitedToSignRefund(false)
    , _failedToSignRefund(false) {
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
    _lastPeerAction = PeerAction::BEP10_handshake_received;

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
        _peerBitSwaprBEPSupportedStatus  = BEPSupportStatus::not_supported;
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
        _peerBitSwaprBEPSupportedStatus  = BEPSupportStatus::not_supported;
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
        _peerBitSwaprBEPSupportedStatus  = BEPSupportStatus::not_supported;
        qCWarning(_category) << "Malformed handshake received: m key not present.";

        // Remember that this peer does not have extension
        _torrentPlugin->addToIrregularPeersSet(peerInfo.ip);

        // Remove this plugin from torrent plugin
        _torrentPlugin->removePeerPlugin(this);

        // Do no keep extension around
        return false;
    }

    // Get peer mapping

    // Convert from lazy entry to entry
    libtorrent::entry mEntry;
    mEntry = *m;

    // Check if it is a dictionary entry
    if(mEntry.type() != libtorrent::entry::dictionary_t) {

        // Mark peer as not supporting BEP43
        _peerBitSwaprBEPSupportedStatus  = BEPSupportStatus::not_supported;
        qCWarning(_category) << "Malformed handshake received: m key not mapping to dictionary.";

        // Remember that this peer does not have extension
        _torrentPlugin->addToIrregularPeersSet(peerInfo.ip);

        // Remove this plugin from torrent plugin
        _torrentPlugin->removePeerPlugin(this);

        // Do no keep extension around
        return false;
    }

    // Make conversion to dictionary entry
    libtorrent::entry::dictionary_type mDictionaryEntry;
    mDictionaryEntry = mEntry.dict();

    // Create peer mapping
    ExtendedMessageIdMapping peerMapping(mDictionaryEntry);

    // Set peer mapping in configuration
    //_peerPluginConfiguration.setPeerMapping(peerMapping);
    _peerMapping = peerMapping;

    // Check that peer mapping is valid: all messages are present, and duplicate ids
    if(!_peerMapping.isValid()) {

        // Mark peer as not supporting BEP43
        _peerBitSwaprBEPSupportedStatus  = BEPSupportStatus::not_supported;
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
    _peerBitSwaprBEPSupportedStatus  = BEPSupportStatus::supported;

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

    _isConnected = false;
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
// IS NOT ACTUALLY CALLED FOR EXTENDED HANDSHAKE ITSELF.
bool PeerPlugin::on_extended(int length, int msg, libtorrent::buffer::const_interval body) {

    qCDebug(_category) << "buyer:on_extended(" << length << "," << msg << ")";

    // Ignore message if peer has not successfully completed BEP43 handshake (yet, or perhaps never will)
    if(_peerBitSwaprBEPSupportedStatus  != BEPSupportStatus::supported) {
        qCDebug(_category) << "Received extended message despite BEP43 not supported, not for this plugin then, letting another plugin handle it.";

        // Let next plugin handle message
        return false;
    }

    // Is it a BEP43 message?
    MessageType messageType;

    try {
        messageType = _peerMapping.messageType(msg);
    } catch(std::exception & e) {

        qCDebug(_category) << "Received extended message, but not with registered extended id, not for this plugin then, letting another plugin handle it.";

        // Not for us, Let next plugin handle message
        return false;
    }

    // Wrap data in QDataStream
    QByteArray byteArray(body.begin, body.end - body.begin);
    QDataStream dataStream(&byteArray, QIODevice::ReadOnly);

    // Parse message
    ExtendedMessagePayload * m = ExtendedMessagePayload::fromRaw(messageType, dataStream);

    // Drop if message was malformed
    if(m == NULL) {

        qCDebug(_category) << "Malformed message BEP43 message received, peer marked for removal.";

        // Mark for removal of this peer
        _lastPeerMessageWasValid = false;

        return true;
    } else {

        // If plugin has not started, then enqueue messages
        if(!_pluginStarted) {
            qCDebug(_category) << "Received extended message despite plugin not started, queue messages.";

            // Add to queue
            _beforePluginStartsMessageQueue.enqueue(m);

        } else {

            // Process message
            processExtendedMessage(m);

            // Delete message
            delete m;
        }
    }

    // No other plugin should process
    return true;
}

/*
 * This is not called for web seeds
 */
bool PeerPlugin::on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body) {

    // if(_peerPluginConfiguration.getPeerBEP43SupportedStatus() != BEPSupportStatus::not_supported)
    if(_peerBitSwaprBEPSupportedStatus  != BEPSupportStatus::not_supported)
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

void PeerPlugin::startPlugin(const Observe & m) {

    // Start plugin
    _pluginStarted = true;

    // Set mode
    _clientPluginMode = PluginMode::Observe;

    // Send observe message
    sendExtendedMessage(m);
}

void PeerPlugin::startPlugin(const Sell & m) {

    // Start plugin
    _pluginStarted = true;

    // Set mode
    _clientPluginMode = PluginMode::Sell;

    // Send message
    sendExtendedMessage(m);
}

void PeerPlugin::startPlugin(const Buy & m) {

    // Start plugin
    _pluginStarted = true;

    // Set mode
    _clientPluginMode = PluginMode::Sell;

    // Send message
    sendExtendedMessage(m);
}


void PeerPlugin::sendExtendedMessage(const ExtendedMessagePayload & extendedMessage) {

    // Length of message full message
    quint32 messageLength = 4 + 1 + 1 + extendedMessage.length();

    // Allocate message buffer
    QByteArray byteArray(messageLength, 0);

    // Wrap buffer in stream
    QDataStream stream(byteArray);

    /**
     * Write both headers to stream:
     * [messageLength():uint32_t][(bt_peer_connection::msg_extended):uint8_t][id:uint8_t]
     */

    // Message length
    stream << messageLength;

    // BEP10 message id
    stream << static_cast<quint8>(libtorrent::bt_peer_connection::msg_extended); // should always be 20

    // Extended message id
    stream << _peerMapping.id(extendedMessage.messageType());

    // Write message into buffer through stream
    extendedMessage.write(stream);

    // If message was written properly buffer, then send buffer to peer
    if(stream.status() != QDataStream::Status::Ok)
        qCCritical(_category) << "Output stream in bad state after message write, message not sent.";
    else {

        // Get raw buffer
        const char * constData = byteArray.constData(); // is zero terminated, but we dont care

        // Send message buffer
        _bittorrentPeerConnection->send_buffer(constData, messageLength);

        // Start/Restart timer
        if(_timeSinceLastMessageSent.isNull())
            _timeSinceLastMessageSent.start();
        else
            _timeSinceLastMessageSent.restart();
    }
}

void PeerPlugin::processExtendedMessage(ExtendedMessagePayload * m) {

    // Get message type
    MessageType messageType = m->messageType();

    qCDebug(_category) << Utilities::messageName(messageType);

    // Call relevant message handler
    switch(messageType) {

        case MessageType::observe:

            _lastPeerMessageWasValid = processObserve(static_cast<Observe *>(m));
            break;
        case MessageType::buy:

            _lastPeerMessageWasValid = processBuy(static_cast<Buy *>(m));
            break;
        case MessageType::sell:

            _lastPeerMessageWasValid = processSell(static_cast<Sell *>(m));
            break;
        case MessageType::join_contract:

            break;
        case MessageType::joining_contract:

            _lastPeerMessageWasValid = processJoiningContract(static_cast<JoiningContract *>(m));
            break;
        case MessageType::sign_refund:

            break;
        case MessageType::refund_signed:

            break;
        case MessageType::ready:

            break;
        case MessageType::payment:

            break;
        case MessageType::end:

            break;
    }

    if(!_lastPeerMessageWasValid)
        qCDebug(_category) << "Received message at incorrect state, peer marked for removal.";

}

bool PeerPlugin::processObserve(const Observe * m) {

    // Check that message is compatible with last peer action
    if(_lastPeerAction != PeerAction::extended_handshake_completed &&
       _lastPeerAction != PeerAction::observe_mode_announced &&
       _lastPeerAction != PeerAction::buy_mode_announced &&
       _lastPeerAction != PeerAction::sell_mode_announced &&
       _lastPeerAction != PeerAction::ended)
        return false;

    // Update last peer action
    _lastPeerAction = PeerAction::observe_mode_announced;

    // Note that peer is in observe mode
    _peerPluginMode = PluginMode::Observe;

    // Message was valid
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

bool PeerPlugin::peerTimedOut(int maxDelay) const {
    return _timeSinceLastMessageSent.elapsed() > maxDelay;
}

BEPSupportStatus PeerPlugin::peerBEP10SupportStatus() const {
    return _peerBEP10SupportedStatus;
}

BEPSupportStatus PeerPlugin::peerBEP43SupportStatus() const {
    return _peerBitSwaprBEPSupportedStatus ;
}

PeerAction PeerPlugin::peerState() const {
   return _lastPeerAction;
}

libtorrent::tcp::endpoint PeerPlugin::endPoint() const {
    return _bittorrentPeerConnection->remote();
}

bool PeerPlugin::isConnected() const {
    return _isConnected;
}

bool PeerPlugin::peerSentInvalidMessage() const {
    return _lastPeerMessageWasValid;
}
