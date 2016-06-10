/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/PeerPlugin.hpp>
#include <extension/TorrentPlugin.hpp>
#include <protocol_wire/protocol_wire.hpp>
#include <libtorrent/bt_peer_connection.hpp> // bt_peer_connection, bt_peer_connection::msg_extended
#include <libtorrent/socket_io.hpp>
#include <libtorrent/peer_info.hpp>

namespace joystream {
namespace extension {

    PeerPlugin::PeerPlugin(TorrentPlugin * plugin,
                           libtorrent::bt_peer_connection * connection,
                           const Policy & policy,
                           const std::string & bep10ClientIdentifier)
        : _plugin(plugin)
        , _connection(connection)
        , _policy(policy)
        , _bep10ClientIdentifier(bep10ClientIdentifier)
        , _endPoint(connection->remote())
        , _peerBEP10SupportStatus(BEPSupportStatus::unknown)
        , _peerPaymentBEPSupportStatus(BEPSupportStatus::supported) {
    }

    PeerPlugin::~PeerPlugin() {
        // Lets log, so we understand when libtorrent disposes of shared pointer
        //std::clog << "~PeerPlugin() called.";
    }

    char const* PeerPlugin::type() const {
        return "PeerPlugin";
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

        // Add top level key for extension version information
        handshake[PLUGIN_NAME] = PLUGIN_VERSION;

        // Add top lvel key for client name and version
        handshake["v"] = _bep10ClientIdentifier;

        // Add m keys for extended message ids
        libtorrent::entry::dictionary_type & m = handshake["m"].dict();

        // Write mapping to key
        _clientMapping.writeToDictionary(m);
    }

    /**
     * m_pc.disconnect(errors::pex_message_too_large, 2);
     * m_pc.disconnect(errors::too_frequent_pex);
     * m_pc.remote().address()
     */
    void PeerPlugin::on_disconnect(libtorrent::error_code const & ec) {

        std::clog << "on_disconnect ["<< (_connection->is_outgoing() ? "outgoing" : "incoming") << "]:" << ec.message().c_str();

        _plugin->removeConnection(_endPoint, protocol_session::DisconnectCause::client);

        // NB: this will be deleted when we return here,
        // hence exit without referencing any members or methods.

        /**

        // Remove from map of peers if present
        bool wasRemoved = removePluginIfInPeersMap(_endPoint);

        if(!_scheduledForDeletingInNextTorrentPluginTick) {

            // Scheduled for deletion <=> must NOT be in peers map
            Q_ASSERT(wasRemoved);

            // Schedule for prompt deletion
            _scheduledForDeletingInNextTorrentPluginTick = true;

            // MUST BE PLACED IN DELETION QLIST!!

            // Save error_code which
            _deletionErrorCode = ec;

        } else
            // Scheduled for deletion <=> must NOT be in peers map
            Q_ASSERT(!wasRemoved);
        */
    }

    void PeerPlugin::on_connected() {

    }

    bool PeerPlugin::on_handshake(char const * reserved_bits) {

        /**
         * The BEP10 docs say:
         * The bit selected for the extension protocol is bit 20 from
         * the right (counting starts at 0).
         * So (reserved_byte[5] & 0x10) is the expression to use for
         * checking if the client supports extended messaging.
         */

        //Q_ASSERT(!_scheduledForDeletingInNextTorrentPluginTick);

        // Check if BEP10 is enabled
        if(reserved_bits[5] & 0x10) {

            std::clog << "BEP10 supported in handshake.";

            // bep10 is supported
            _peerBEP10SupportStatus = BEPSupportStatus::supported;

            return true;

        } else {

            std::clog << "BEP10 not supported in handshake.";

            // bep10 is not supported
            _peerBEP10SupportStatus = BEPSupportStatus::not_supported;

            // hence it also cannot support this spesific extension
            _peerPaymentBEPSupportStatus  = BEPSupportStatus::not_supported;

            // note that peer does not have extension
            _plugin->addToPeersWithoutExtensionSet(_endPoint);

            return _policy.installPluginOnPeersWithoutExtension;
        }
    }

    bool PeerPlugin::on_extension_handshake(libtorrent::lazy_entry const & handshake) {
    //bool PeerPlugin::on_extension_handshake(libtorrent::bdecode_node const & handshake) {

        /**
        // Check that peer plugin is still valid
        if(_scheduledForDeletingInNextTorrentPluginTick) {

            // http://www.libtorrent.org/reference-Plugins.html
            // if (on_extension_handshake) returns false, it means that this extension isn't supported by this peer.
            // It will result in this peer_plugin being removed from the peer_connection and destructed.
            // this is not called for web seeds
            std::clog << "Extended handshake ignored since peer_plugin i scheduled for deletion.";
            return false;
        }
        */

        // Write what client is trying to handshake us, should now be possible given initial hand shake
        libtorrent::peer_info peerInfo;
        _connection->get_peer_info(peerInfo);

        std::clog << "on_extension_handshake[" << peerInfo.client.c_str() << "]";

        // Check that BEP10 was actually supported, if it wasnt, then the peer is misbehaving
        if(_peerBEP10SupportStatus != BEPSupportStatus::supported) {

            std::clog << "Peer didn't support BEP10, but it sent extended handshake.";

            // Policy dictates if we install plugin
            return _policy.installPluginOnPeersMisbehavingDuringExtendedHandshake;
        }

        //////////////////////////////////////////////////
        /// We cannot trust structure of entry, since it is from peer,
        /// hence we must check it properly.
        //////////////////////////////////////////////////

        // If its not a dictionary, we are done
        if(handshake.type() != libtorrent::lazy_entry::dict_t) {

            // Remember that this peer does not have extension
            _plugin->addToIrregularPeersSet(peerInfo.ip);

            // Mark peer as not supporting BEP43
            _peerPaymentBEPSupportStatus  = BEPSupportStatus::not_supported;

            std::clog << "Malformed handshake received: not dictionary.";

            // Policy dictates if we install plugin
            return _policy.installPluginOnPeersMisbehavingDuringExtendedHandshake;
        }

        // Check if plugin key is there
        int version = handshake.dict_find_int_value(PLUGIN_NAME,-1);

        if(version == -1) {

            // Remember that this peer does not have extension
            _plugin->addToPeersWithoutExtensionSet(peerInfo.ip);

            // Mark peer as not supporting BEP43
            _peerPaymentBEPSupportStatus  = BEPSupportStatus::not_supported;

            std::clog << "Extension not supported.";

            // Policy dictates if we install plugin
            return _policy.installPluginOnPeersMisbehavingDuringExtendedHandshake;

        } else
            std::clog << "Extension version" << version << "supported.";

        // Try to extract m key, if its not present, then we are done
        const libtorrent::lazy_entry * m = handshake.dict_find_dict("m");

        if(!m) {

            // Remember that this peer does not have extension
            _plugin->addToIrregularPeersSet(peerInfo.ip);

            // Mark peer as not supporting BEP43
            _peerPaymentBEPSupportStatus = BEPSupportStatus::not_supported;

            std::clog << "Malformed handshake received: m key not present.";

            // Policy dictates if we install plugin
            return _policy.installPluginOnPeersMisbehavingDuringExtendedHandshake;
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
            _peerPaymentBEPSupportStatus  = BEPSupportStatus::not_supported;

            std::clog << "Malformed handshake received: m key not mapping to dictionary.";

            // Policy dictates if we install plugin
            return _policy.installPluginOnPeersMisbehavingDuringExtendedHandshake;
        }

        // Make conversion to dictionary entry
        libtorrent::entry::dictionary_type mDictionaryEntry;
        mDictionaryEntry = mEntry.dict();

        // Create peer mapping
        _peerMapping = ExtendedMessageIdMapping(mDictionaryEntry);

        // Check that peer mapping is valid: all messages are present, and duplicate ids
        if(!_peerMapping.isValid()) {

            // Remember that this peer does not have extension
            _plugin->addToIrregularPeersSet(peerInfo.ip);

            // Mark peer as not supporting BEP43
            _peerPaymentBEPSupportStatus = BEPSupportStatus::not_supported;

            std::clog << "m key does not contain mapping for all messages.";

            // Policy dictates if we install plugin
            return _policy.installPluginOnPeersMisbehavingDuringExtendedHandshake;
        }

        // Notify
        std::string endPointString = libtorrent::print_endpoint(peerInfo.ip);

        std::clog << "Found extension handshake for peer " << endPointString.c_str();

        // All messages were present, hence the protocol is supported
        _peerPaymentBEPSupportStatus = BEPSupportStatus::supported;

        // Add peer to session
        _plugin->addPeerToSession(peerInfo.ip);

        // Install plugin
        return true;
    }

    bool PeerPlugin::on_have(int) {
        return true; // overrid default handler
    }

    bool PeerPlugin::on_bitfield(libtorrent::bitfield const &) {
        return true; // overrid default handler
    }

    bool PeerPlugin::on_have_all() {
        return true; // overrid default handler
    }

    bool PeerPlugin::on_reject(libtorrent::peer_request const &) {
        return true; // overrid default handler
    }

    bool PeerPlugin::on_request(libtorrent::peer_request const &) {
        return true; // overrid default handler
    }

    bool PeerPlugin::on_unchoke() {
        return true; // overrid default handler
    }

    bool PeerPlugin::on_interested() {
        return true; // overrid default handler
    }

    bool PeerPlugin::on_allowed_fast(int) {
        return true; // overrid default handler
    }

    bool PeerPlugin::on_have_none() {
        return true; // overrid default handler
    }

    bool PeerPlugin::on_choke() {
        return true; // overrid default handler
    }

    bool PeerPlugin::on_not_interested() {
        return true; // overrid default handler
    }

    bool PeerPlugin::on_piece(libtorrent::peer_request const &, libtorrent::disk_buffer_holder &) {
        return false; // let default handler process a piece
    }

    bool PeerPlugin::on_suggest(int) {
        return true; // overrid default handler
    }

    bool PeerPlugin::on_cancel(libtorrent::peer_request const &) {
        return true; // overrid default handler
    }

    bool PeerPlugin::on_dont_have(int) {
        return true; // overrid default handler
    }

    bool PeerPlugin::can_disconnect(libtorrent::error_code const & ec) {

        std::clog << "can_disconnect: " << ec.message() << std::endl;

        // rejecting request
        return false;
    }

    bool PeerPlugin::on_extended(int length, int msg, libtorrent::buffer::const_interval body) {

        /**
        // Check peer plugin integrity
        if(_scheduledForDeletingInNextTorrentPluginTick) {

            std::clog << "Ignoring extended message since peer_plugin is scheduled for deletion.";
            return false;
        }
        */

        // Does the peer even support extension?
        if(_peerPaymentBEPSupportStatus != BEPSupportStatus::supported) {

            std::clog << "Ignoring extended message from peer without extension.";
            return false;
        }

        // Length of extended message, excluding the bep 10 id and extended message id.
        int lengthOfExtendedMessagePayload = body.left();

        // Do we have full message
        if(length != lengthOfExtendedMessagePayload) {

            // Output progress
            std::clog << "on_extended(id =" << msg << ", length =" << length << "): %" << ((float)(100*lengthOfExtendedMessagePayload))/length;

            // No other plugin should look at this
            return true;
        } else
            std::clog << "on_extended(id =" << msg << ", length =" << length << ")";

        // Ignore message if peer has not successfully completed BEP43 handshake (yet, or perhaps never will)
        if(_peerPaymentBEPSupportStatus != BEPSupportStatus::supported) {

            std::clog << "Received extended message despite BEP43 not supported, not for this plugin then, letting another plugin handle it.";

            // Let next plugin handle message
            return false;
        }

        // Is it a BitSwapr BEP message?
        joystream::protocol_wire::MessageType messageType;

        try {
            messageType = _peerMapping.messageType(msg);
        } catch(std::exception & e) {

            std::clog << "Received extended message, but not with registered extended id, not for this plugin then, letting another plugin handle it.";

            // Not for us, Let next plugin handle message
            return false;
        }

        /**
        // Check that plugin is in good state
        if(_lastReceivedMessageWasMalformed || _lastMessageWasStateIncompatible) { // || !_connectionAlive) {

            std::clog << "Dropping extended message since peer plugin is in bad state.";

            // No other plugin should process message
            return true;
        }
        */

        // WRAP in QByteAray: No copying is done, and no ownership is taken!
        // http://doc.qt.io/qt-4.8/qbytearray.html#fromRawData
        QByteArray byteArray = QByteArray::fromRawData(body.begin, lengthOfExtendedMessagePayload);

        // Wrap data in byte array in stream
        QDataStream stream(&byteArray, QIODevice::ReadOnly);

        // Explicitly set endianness
        stream.setByteOrder(QDataStream::BigEndian);

        // Parse message
        qint64 preReadPosition = stream.device()->pos();
        joystream::protocol_wire::ExtendedMessagePayload * m = joystream::protocol_wire::ExtendedMessagePayload::fromRaw(messageType, stream, lengthOfExtendedMessagePayload);
        qint64 postReadPosition = stream.device()->pos();

        qint64 totalReadLength = postReadPosition - preReadPosition;

        // Check that the full extended payload was parsed
        if(totalReadLength != lengthOfExtendedMessagePayload) {

            std::stringstream s;

            // MAKE PROPER TYPED EXCEPTION AT SOME POINT
            s << "Extended message payload was expected to have length of"
              << lengthOfExtendedMessagePayload << "bytes"
              << ", however full valid message was parsed to be of length "
              << totalReadLength << "bytes"
              << " and of type "
              << joystream::protocol_wire::MessageTypeToString(m->messageType());

            throw std::runtime_error(s.str());
        }

        // Drop if message was malformed
        if(m == NULL) {

            std::clog << "Malformed extended message received, removing .";

            // Remove this peer
            _plugin->disconnectPeer(_endPoint);

        } else {

            // Process message
            _plugin->processExtendedMessage(_endPoint, *m);

            // Delete message
            delete m;
        }

        // No other plugin should process message
        return true;
    }

    bool PeerPlugin::on_unknown_message(int, int, libtorrent::buffer::const_interval) {
        return true; // allow other handlers to process
    }

    void PeerPlugin::on_piece_pass(int) {

    }

    void PeerPlugin::on_piece_failed(int) {

    }

    void PeerPlugin::tick() {

    }

    bool PeerPlugin::write_request(libtorrent::peer_request const &) {
        // no one gets to send to this peer but us!
        return false;
    }

    void PeerPlugin::send(const joystream::protocol_wire::ExtendedMessagePayload * extendedMessagePayload) {

        // Get length of
        quint32 extendedMessagePayloadLength = extendedMessagePayload->length();

        // Length of message full message
        quint32 fullMessageLength = 4 + 1 + 1 + extendedMessagePayloadLength;

        // Length value in outer BitTorrent header
        quint32 fullMessageLengthFieldValue = 1 + 1 + extendedMessagePayloadLength;

        // Allocate message array buffer
        QByteArray byteArray(fullMessageLength, 0);

        // Wrap buffer in stream
        QDataStream stream(&byteArray, QIODevice::WriteOnly);

        // Set byte order explicitly
        stream.setByteOrder(QDataStream::BigEndian);

        /**
         * Write both headers to stream:
         * [messageLength():uint32_t][(bt_peer_connection::msg_extended):uint8_t][id:uint8_t]
         */

        // Message length
        stream << fullMessageLengthFieldValue;

        // BEP10 message id
        stream << static_cast<quint8>(libtorrent::bt_peer_connection::msg_extended); // should always be 20 according to BEP10 spec

        // Extended message id
        stream << _peerMapping.id(extendedMessagePayload->messageType());

        // Write message into buffer through stream
        qint64 preWritePosition = stream.device()->pos();
        extendedMessagePayload->write(stream);
        qint64 postWritePosition = stream.device()->pos();

        qint64 written = postWritePosition - preWritePosition;

        Q_ASSERT(written == extendedMessagePayloadLength);

        std::clog << "SENT:" << joystream::protocol_wire::messageName(extendedMessagePayload->messageType()) << " = " << written << "bytes";

        // If message was written properly buffer, then send buffer to peer
        if(stream.status() != QDataStream::Status::Ok)
            std::clog << "Output stream in bad state after message write, message not sent.";
        else {

            // Get raw buffer
            const char * constData = byteArray.constData(); // is zero terminated, but we dont care

            // Send message buffer
            _connection->send_buffer(constData, byteArray.length());

            // Do some sort of catching of error if sending did not work??

            /**
            // Start/Restart timer
            if(_timeSinceLastMessageSent.isNull())
                _timeSinceLastMessageSent.start();
            else
                _timeSinceLastMessageSent.restart();
            */
        }
    }

    status::PeerPlugin PeerPlugin::status() const {
        return status::PeerPlugin(_endPoint,
                                  _peerBEP10SupportStatus,
                                  _peerPaymentBEPSupportStatus);
    }

    void PeerPlugin::disconnect(libtorrent::error_code & ec) {
        _connection->disconnect(ec);
    }

/**
    libtorrent::bt_peer_connection * PeerPlugin::connection() {
        return _connection;
    }

    bool PeerPlugin::peerTimedOut(int maxDelay) const {
        return (!_timeSinceLastMessageSent.isNull()) && (_timeSinceLastMessageSent.elapsed() > maxDelay);
    }

    BEPSupportStatus PeerPlugin::peerBEP10SupportStatus() const {
        return _peerBEP10SupportStatus;
    }

    BEPSupportStatus PeerPlugin::peerBitSwaprBEPSupportStatus() const {
        return _peerPaymentBEPSupportStatus;
    }

    libtorrent::tcp::endpoint PeerPlugin::endPoint() const {
        return _connection->remote();
    }

    libtorrent::error_code PeerPlugin::deletionErrorCode() const {
        return _deletionErrorCode;
    }
*/

}
}
