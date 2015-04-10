#include "PeerPlugin.hpp"
#include "TorrentPlugin.hpp"
#include "Plugin.hpp"
#include "Config.hpp"
//#include "PeerPluginStatus.hpp"
//#include "Request/PeerPluginRequest.hpp"

#include "Message/MessageType.hpp"
#include "Message/ExtendedMessagePayload.hpp"


/**
 * PeerPlugin
 */

/*
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
*/
#include "Utilities.hpp"

#include <libtorrent/bt_peer_connection.hpp> // bt_peer_connection, bt_peer_connection::msg_extended
#include <libtorrent/socket_io.hpp>
#include <libtorrent/peer_info.hpp>

#include <QLoggingCategory>

PeerPlugin::PeerPlugin(TorrentPlugin * plugin,
                       libtorrent::bt_peer_connection * connection,
                       QLoggingCategory & category)
    : _plugin(plugin)
    , _connection(connection)
    , _endPoint(connection->remote())
    , _peerModeAnnounced(PeerModeAnnounced::none)
    , _lastReceivedMessageWasMalformed(false)
    , _lastMessageWasStateIncompatible(false)
    , _peerBEP10SupportStatus(BEPSupportStatus::unknown)
    , _peerBitSwaprBEPSupportStatus(BEPSupportStatus::supported)
    , _category(category) {
}

PeerPlugin::~PeerPlugin() {

    // Lets log, so we understand when libtorrent disposes of shared pointer
    qCDebug(_category) << "~PeerPlugin() called.";
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

    // Check if BEP10 is enabled
    if(reserved_bits[5] & 0x10) {

        //qCDebug(_category) << "BEP10 supported in handshake.";
        _peerBEP10SupportStatus = BEPSupportStatus::supported;
        return true;

    } else {

        qCDebug(_category) << "BEP10 not supported in handshake.";
        _peerBEP10SupportStatus = BEPSupportStatus::not_supported;
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

    // Write what client is trying to handshake us, should now be possible given initial hand shake
    libtorrent::peer_info peerInfo;
    _connection->get_peer_info(peerInfo);

    qCDebug(_category) << "on_extension_handshake[" << peerInfo.client.c_str() << "]";

    // Check that BEP10 was actually supported, if it wasnt, then the peer is misbehaving
    if(_peerBEP10SupportStatus != BEPSupportStatus::supported) {

        // Remember that this peer does not have extension
        _plugin->addToPeersWithoutExtensionSet(peerInfo.ip);

        // Mark peer as not supporting BEP43
        _peerBitSwaprBEPSupportStatus  = BEPSupportStatus::not_supported;

        qCWarning(_category) << "Peer didn't support BEP10, but it sent extended handshake.";

        // Do no keep extension around
        return false;
    }

    /**
     * We cannot trust structure of entry, since it is from peer,
     * hence we must check it properly.
     */

    // If its not a dictionary, we are done
    if(handshake.type() != libtorrent::lazy_entry::dict_t) {

        // Remember that this peer does not have extension
        _plugin->addToIrregularPeersSet(peerInfo.ip);

        // Mark peer as not supporting BEP43
        _peerBitSwaprBEPSupportStatus  = BEPSupportStatus::not_supported;

        qCWarning(_category) << "Malformed handshake received: not dictionary.";

        // Do no keep extension around
        return false;
    }

    // Check if plugin key is there
    int version = handshake.dict_find_int_value(EXTENSION_NAME,-1);

    if(version == -1) {

        // Remember that this peer does not have extension
        _plugin->addToPeersWithoutExtensionSet(peerInfo.ip);

        // Mark peer as not supporting BEP43
        _peerBitSwaprBEPSupportStatus  = BEPSupportStatus::not_supported;

        qCDebug(_category) << "Extension not supported.";

        // Do no keep extension around
        return false;

    } else
        qCDebug(_category) << "Extension version" << version << "supported.";

    // Try to extract m key, if its not present, then we are done
    const libtorrent::lazy_entry * m = handshake.dict_find_dict("m");

    if(!m) {

        // Remember that this peer does not have extension
        _plugin->addToIrregularPeersSet(peerInfo.ip);

        // Mark peer as not supporting BEP43
        _peerBitSwaprBEPSupportStatus = BEPSupportStatus::not_supported;

        qCWarning(_category) << "Malformed handshake received: m key not present.";

        // Do no keep extension around
        return false;
    }

    // Get peer mapping

    // Convert from lazy entry to entry
    libtorrent::entry mEntry;
    mEntry = *m;

    // Check if it is a dictionary entry
    if(mEntry.type() != libtorrent::entry::dictionary_t) {

        // Remember that this peer does not have extension
        _plugin->addToIrregularPeersSet(peerInfo.ip);

        // Mark peer as not supporting BEP43
        _peerBitSwaprBEPSupportStatus  = BEPSupportStatus::not_supported;

        qCWarning(_category) << "Malformed handshake received: m key not mapping to dictionary.";

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

        // Remember that this peer does not have extension
        _plugin->addToIrregularPeersSet(peerInfo.ip);

        // Mark peer as not supporting BEP43
        _peerBitSwaprBEPSupportStatus = BEPSupportStatus::not_supported;

        qCDebug(_category) << "m key does not contain mapping for all messages.";

        // Do no keep extension around
        return false;
    }

    // Notify
    std::string endPointString = libtorrent::print_endpoint(peerInfo.ip);

    qCDebug(_category) << "Found extension handshake for peer " << endPointString.c_str();

    // All messages were present, hence the protocol is supported
    _peerBitSwaprBEPSupportStatus = BEPSupportStatus::supported;

    // Tell libtorrent that our extension should be kept in the loop for this peer
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
    if(_peerBitSwaprBEPSupportStatus != BEPSupportStatus::supported) {

        qCDebug(_category) << "Received extended message despite BEP43 not supported, not for this plugin then, letting another plugin handle it.";

        // Let next plugin handle message
        return false;
    }

    // Is it a BitSwapr BEP message?
    MessageType messageType;

    try {
        messageType = _peerMapping.messageType(msg);
    } catch(std::exception & e) {

        qCDebug(_category) << "Received extended message, but not with registered extended id, not for this plugin then, letting another plugin handle it.";

        // Not for us, Let next plugin handle message
        return false;
    }

    // Check that plugin is in good state
    if(_lastReceivedMessageWasMalformed || _lastMessageWasStateIncompatible) { // || !_connectionAlive) {

        qCDebug(_category) << "Dropping extended message since peer plugin is in bad state.";

        // No other plugin should process message
        return true;
    }

    // Wrap data in QDataStream
    QByteArray byteArray(body.begin, body.end - body.begin);
    QDataStream dataStream(&byteArray, QIODevice::ReadOnly);

    // Parse message
    ExtendedMessagePayload * m = ExtendedMessagePayload::fromRaw(messageType, dataStream);

    // Drop if message was malformed
    if(m == NULL) {

        qCDebug(_category) << "Malformed message BitSwapr BEP message received, peer marked for removal.";

        // Note that message was malformed
        _lastReceivedMessageWasMalformed = true;

    } else {

        // Process message
        processExtendedMessage(m);

        // Delete message
        delete m;
    }

    // No other plugin should process message
    return true;
}

/**
void PeerPlugin::processPeerPluginRequest(const PeerPluginRequest * peerPluginRequest) {

    qCDebug(_category) << "processPeerPluginRequest";

    switch(peerPluginRequest->getPeerPluginRequestType()) {

    }
}
*/

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
        _connection->send_buffer(constData, messageLength);

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

    try {

        // Call relevant message handler
        switch(messageType) {

            case MessageType::observe:
                processObserve(reinterpret_cast<Observe *>(m));
                break;
            case MessageType::buy:
                processBuy(reinterpret_cast<Buy *>(m));
                break;
            case MessageType::sell:
                processSell(reinterpret_cast<Sell *>(m));
                break;
            case MessageType::join_contract:
                processJoinContract(reinterpret_cast<JoinContract *>(m));
                break;
            case MessageType::joining_contract:
                processJoiningContract(reinterpret_cast<JoiningContract *>(m));
                break;
            case MessageType::sign_refund:
                processSignRefund(reinterpret_cast<SignRefund *>(m));
                break;
            case MessageType::refund_signed:
                processRefundSigned(reinterpret_cast<RefundSigned *>(m));
                break;
            case MessageType::ready:
                processReady(reinterpret_cast<Ready *>(m));
                break;
            case MessageType::payment:
                processPayment(reinterpret_cast<Payment *>(m));
                break;

                /*
            case MessageType::end:
                processEnd(static_cast<End *>(m));
                break;
                */
        }

    } catch (std::exception & e) {

        qCCritical(_category) << "Extended message was state incompatible:" << e.what();

        // Note incompatibility
        _lastMessageWasStateIncompatible = true;
    }
}

bool PeerPlugin::peerTimedOut(int maxDelay) const {
    return (!_timeSinceLastMessageSent.isNull()) && (_timeSinceLastMessageSent.elapsed() > maxDelay);
}

BEPSupportStatus PeerPlugin::peerBEP10SupportStatus() const {
    return _peerBEP10SupportStatus;
}

BEPSupportStatus PeerPlugin::peerBitSwaprBEPSupportStatus() const {
    return _peerBitSwaprBEPSupportStatus ;
}

libtorrent::tcp::endpoint PeerPlugin::endPoint() const {
    return _connection->remote();
}

/**
bool PeerPlugin::connectionAlive() const {
    return _connectionAlive;
}
*/

bool PeerPlugin::lastReceivedMessageWasMalformed() const {
    return _lastReceivedMessageWasMalformed;
}
