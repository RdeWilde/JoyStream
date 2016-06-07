/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/PeerPlugin.hpp>
#include <extension/TorrentPlugin.hpp>
#include <protocol/MessageType.hpp>
#include <protocol/ExtendedMessagePayload.hpp> // remove later
#include <libtorrent/bt_peer_connection.hpp> // bt_peer_connection, bt_peer_connection::msg_extended
#include <libtorrent/socket_io.hpp>
#include <libtorrent/peer_info.hpp>

#include <QLoggingCategory>
#include <QDataStream>

namespace joystream {
namespace extension {

    PeerPlugin::PeerPlugin(TorrentPlugin * plugin,
                           libtorrent::bt_peer_connection * connection,
                           const std::string & bep10ClientIdentifier,
                           bool scheduledForDeletingInNextTorrentPluginTick,
                           QLoggingCategory & category)
        : _plugin(plugin)
        , _connection(connection)
        , _bep10ClientIdentifier(bep10ClientIdentifier)
        , _endPoint(connection->remote())
        , _peerModeAnnounced(PeerModeAnnounced::none)
        , _lastReceivedMessageWasMalformed(false)
        , _lastMessageWasStateIncompatible(false)
        , _scheduledForDeletingInNextTorrentPluginTick(scheduledForDeletingInNextTorrentPluginTick)
        , _peerBEP10SupportStatus(BEPSupportStatus::unknown)
        , _peerBitSwaprBEPSupportStatus(BEPSupportStatus::supported)
        , _category(category) {
    }

    PeerPlugin::~PeerPlugin() {

        // Lets log, so we understand when libtorrent disposes of shared pointer
        //qCDebug(_category) << "~PeerPlugin() called.";
    }

    /*
     * Can add entries to the extension handshake this is not called for web seeds
     */
    void PeerPlugin::add_handshake(libtorrent::entry & handshake) {

        //Q_ASSERT(!_scheduledForDeletingInNextTorrentPluginTick);

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

    void PeerPlugin::on_disconnect(libtorrent::error_code const & ec) {

        qCDebug(_category) << "on_disconnect ["<< (_connection->is_outgoing() ? "outgoing" : "incoming") << "]:" << ec.message().c_str();

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
    }
    */

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

        //Q_ASSERT(!_scheduledForDeletingInNextTorrentPluginTick);

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
    //bool PeerPlugin::on_extension_handshake(libtorrent::bdecode_node const & handshake) {

        // Check that peer plugin is still valid
        if(_scheduledForDeletingInNextTorrentPluginTick) {

            // http://www.libtorrent.org/reference-Plugins.html
            // if (on_extension_handshake) returns false, it means that this extension isn't supported by this peer.
            // It will result in this peer_plugin being removed from the peer_connection and destructed.
            // this is not called for web seeds
            qCDebug(_category) << "Extended handshake ignored since peer_plugin i scheduled for deletion.";
            return false;
        }

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
            //return false;

            // KEEP PLUGIN AROUND, SO WE CAN BLOCK REGULAR
            // BITTORRENT EXCHANGE WHICH MESSES THINGS UP
            return true;
        }

         //////////////////////////////////////////////////
         // We cannot trust structure of entry, since it is from peer,
         // hence we must check it properly.
         //////////////////////////////////////////////////

        // If its not a dictionary, we are done
        if(handshake.type() != libtorrent::lazy_entry::dict_t) {

            // Remember that this peer does not have extension
            _plugin->addToIrregularPeersSet(peerInfo.ip);

            // Mark peer as not supporting BEP43
            _peerBitSwaprBEPSupportStatus  = BEPSupportStatus::not_supported;

            qCWarning(_category) << "Malformed handshake received: not dictionary.";

            // Do no keep extension around
            //return false;

            // KEEP PLUGIN AROUND, SO WE CAN BLOCK REGULAR
            // BITTORRENT EXCHANGE WHICH MESSES THINGS UP
            return true;
        }

        // Check if plugin key is there
        int version = handshake.dict_find_int_value(PLUGIN_NAME,-1);

        if(version == -1) {

            // Remember that this peer does not have extension
            _plugin->addToPeersWithoutExtensionSet(peerInfo.ip);

            // Mark peer as not supporting BEP43
            _peerBitSwaprBEPSupportStatus  = BEPSupportStatus::not_supported;

            qCDebug(_category) << "Extension not supported.";

            // Do no keep extension around
            //return false;

            // KEEP PLUGIN AROUND, SO WE CAN BLOCK REGULAR
            // BITTORRENT EXCHANGE WHICH MESSES THINGS UP
            return true;

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
            //return false;

            // KEEP PLUGIN AROUND, SO WE CAN BLOCK REGULAR
            // BITTORRENT EXCHANGE WHICH MESSES THINGS UP
            return true;
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
            //return false;

            // KEEP PLUGIN AROUND, SO WE CAN BLOCK REGULAR
            // BITTORRENT EXCHANGE WHICH MESSES THINGS UP
            return true;
        }

        // Make conversion to dictionary entry
        libtorrent::entry::dictionary_type mDictionaryEntry;
        mDictionaryEntry = mEntry.dict();

        //////////////////////////////////////////////////
        //for(std::map<std::string, libtorrent::entry>::const_iterator i = mDictionaryEntry.cbegin(),
        //    end(mDictionaryEntry.cend()); i != end;i++) {
        //
        //    std::string key = (*i).first;
        //    libtorrent::entry value = (*i).second;
        //    std::string valueStr = value.to_string();
        //
        //    qCWarning(_category) << key.c_str() << ": " << valueStr.c_str();
        //
        //}
        //////////////////////////////////////////////////

        // Create peer mapping
        _peerMapping = ExtendedMessageIdMapping(mDictionaryEntry);

        // Check that peer mapping is valid: all messages are present, and duplicate ids
        if(!_peerMapping.isValid()) {

            // Remember that this peer does not have extension
            _plugin->addToIrregularPeersSet(peerInfo.ip);

            // Mark peer as not supporting BEP43
            _peerBitSwaprBEPSupportStatus = BEPSupportStatus::not_supported;

            qCDebug(_category) << "m key does not contain mapping for all messages.";

            // Do no keep extension around
            //return false;

            // KEEP PLUGIN AROUND, SO WE CAN BLOCK REGULAR
            // BITTORRENT EXCHANGE WHICH MESSES THINGS UP
            return true;
        }

        // Notify
        std::string endPointString = libtorrent::print_endpoint(peerInfo.ip);

        qCDebug(_category) << "Found extension handshake for peer " << endPointString.c_str();

        // All messages were present, hence the protocol is supported
        _peerBitSwaprBEPSupportStatus = BEPSupportStatus::supported;

        // Tell libtorrent that our extension should be kept in the loop for this peer
        //return false;

        //bool BuyerPeerPlugin::on_extension_handshake(const libtorrent::bdecode_node & handshake) {

        if(_clientState != BuyerPeerPluginClientState::no_bitswapr_message_sent) {
            throw std::runtime_error("Extended handshake initiated at incorrect state.");
        }

        qCDebug(_category) << "Extended handshake arrived.";

        // Use base class extension handhsake processor
        bool keepPlugin = PeerPlugin::on_extension_handshake(handshake);

        // If handshake was successful
        if(keepPlugin) {

            // send mode message
            sendExtendedMessage(joystream::protocol::Buy(_plugin->maxPrice(), _plugin->maxLock(), _plugin->numberOfSellers()));
            // _plugin->maxFeePerByte()

            // and update new client state correspondingly
            _clientState = BuyerPeerPluginClientState::buyer_mode_announced;
        }

        // Return status to libtorrent
        return keepPlugin;
    }

    // Called when an extended message is received. If returning true,
    // the message is not processed by any other plugin and if false
    // is returned the next plugin in the chain will receive it to
    // be able to handle it this is not called for web seeds.
    // IS NOT ACTUALLY CALLED FOR EXTENDED HANDSHAKE ITSELF.
    bool PeerPlugin::on_extended(int length, int msg, libtorrent::buffer::const_interval body) {

        // Check peer plugin integrity
        if(_scheduledForDeletingInNextTorrentPluginTick) {

            qCDebug(_category) << "Ignoring extended message since peer_plugin is scheduled for deletion.";
            return false;
        }

        // Does the peer even support extension?
        if(_peerBitSwaprBEPSupportStatus != BEPSupportStatus::supported) {

            qCDebug(_category) << "Ignoring extended message from peer without extension.";
            return false;
        }

        // Length of extended message, excluding the bep 10 id and extended message id.
        int lengthOfExtendedMessagePayload = body.left();

        // Do we have full message
        if(length != lengthOfExtendedMessagePayload) {

            // Output progress
            //qCDebug(_category) << "on_extended(id =" << msg << ", length =" << length << "): %" << ((float)(100*lengthOfExtendedMessagePayload))/length;

            // No other plugin should look at this
            return true;
        } else
            qCDebug(_category) << "on_extended(id =" << msg << ", length =" << length << ")";

        // Ignore message if peer has not successfully completed BEP43 handshake (yet, or perhaps never will)
        if(_peerBitSwaprBEPSupportStatus != BEPSupportStatus::supported) {

            qCDebug(_category) << "Received extended message despite BEP43 not supported, not for this plugin then, letting another plugin handle it.";

            // Let next plugin handle message
            return false;
        }

        // Is it a BitSwapr BEP message?
        joystream::protocol::MessageType messageType;

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

        // WRAP in QByteAray: No copying is done, and no ownership is taken!
        // http://doc.qt.io/qt-4.8/qbytearray.html#fromRawData
        QByteArray byteArray = QByteArray::fromRawData(body.begin, lengthOfExtendedMessagePayload);

        // Wrap data in byte array in stream
        QDataStream stream(&byteArray, QIODevice::ReadOnly);

        // Explicitly set endianness
        stream.setByteOrder(QDataStream::BigEndian);

        // Parse message
        qint64 preReadPosition = stream.device()->pos();
        joystream::protocol::ExtendedMessagePayload * m = joystream::protocol::ExtendedMessagePayload::fromRaw(messageType, stream, lengthOfExtendedMessagePayload);
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
              << joystream::protocol::MessageTypeToString(m->messageType());

            throw std::runtime_error(s.str());
        }

        // Drop if message was malformed
        if(m == NULL) {

            qCDebug(_category) << "Malformed extended message received, peer marked for removal.";

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

    void PeerPlugin::sendExtendedMessage(const joystream::protocol::ExtendedMessagePayload & extendedMessagePayload) {

        // Get length of
        quint32 extendedMessagePayloadLength = extendedMessagePayload.length();

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
        stream << _peerMapping.id(extendedMessagePayload.messageType());

        // Write message into buffer through stream
        qint64 preWritePosition = stream.device()->pos();
        extendedMessagePayload.write(stream);
        qint64 postWritePosition = stream.device()->pos();

        qint64 written = postWritePosition - preWritePosition;

        Q_ASSERT(written == extendedMessagePayloadLength);

        qCDebug(_category) << "SENT:" << joystream::protocol::messageName(extendedMessagePayload.messageType()) << " = " << written << "bytes";

        // If message was written properly buffer, then send buffer to peer
        if(stream.status() != QDataStream::Status::Ok)
            qCCritical(_category) << "Output stream in bad state after message write, message not sent.";
        else {

            // Get raw buffer
            const char * constData = byteArray.constData(); // is zero terminated, but we dont care

            // Send message buffer
            _connection->send_buffer(constData, byteArray.length());

            // Do some sort of catching of error if sending did not work??

            // Start/Restart timer
            if(_timeSinceLastMessageSent.isNull())
                _timeSinceLastMessageSent.start();
            else
                _timeSinceLastMessageSent.restart();
        }
    }

    void PeerPlugin::processExtendedMessage(joystream::protocol::ExtendedMessagePayload * m) {

        // Get message type
        joystream::protocol::MessageType messageType = m->messageType();

        qCDebug(_category) << "RECEIVED:" << joystream::protocol::messageName(messageType);

        //try {

            // Call relevant message handler
            switch(messageType) {

                case joystream::protocol::MessageType::observe:
                    processObserve(reinterpret_cast<joystream::protocol::Observe *>(m));
                    break;
                case joystream::protocol::MessageType::buy:
                    processBuy(reinterpret_cast<joystream::protocol::Buy *>(m));
                    break;
                case joystream::protocol::MessageType::sell:
                    processSell(reinterpret_cast<joystream::protocol::Sell *>(m));
                    break;
                case joystream::protocol::MessageType::join_contract:
                    processJoinContract(reinterpret_cast<joystream::protocol::JoinContract *>(m));
                    break;
                case joystream::protocol::MessageType::joining_contract:
                    processJoiningContract(reinterpret_cast<joystream::protocol::JoiningContract *>(m));
                    break;
                case joystream::protocol::MessageType::sign_refund:
                    processSignRefund(reinterpret_cast<joystream::protocol::SignRefund *>(m));
                    break;
                case joystream::protocol::MessageType::refund_signed:
                    processRefundSigned(reinterpret_cast<joystream::protocol::RefundSigned *>(m));
                    break;
                case joystream::protocol::MessageType::ready:
                    processReady(reinterpret_cast<joystream::protocol::Ready *>(m));
                    break;

                case joystream::protocol::MessageType::request_full_piece:
                    processRequestFullPiece(reinterpret_cast<joystream::protocol::RequestFullPiece *>(m));
                    break;

                case joystream::protocol::MessageType::full_piece:
                    processFullPiece(reinterpret_cast<joystream::protocol::FullPiece *>(m));
                    break;

                case joystream::protocol::MessageType::payment:
                    processPayment(reinterpret_cast<joystream::protocol::Payment *>(m));
                    break;

                default:

                    Q_ASSERT(false);
            }
    /**
        } catch (std::exception & e) {

            qCCritical(_category) << "Extended message was state incompatible:" << e.what();

            // Note incompatibility
            _lastMessageWasStateIncompatible = true;
        }
        */
    }

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

    PeerModeAnnounced PeerPlugin::peerModeAnnounced() const {
        return _peerModeAnnounced;
    }

    void PeerPlugin::setPeerModeAnnounced(PeerModeAnnounced peerModeAnnounced) {
        _peerModeAnnounced = peerModeAnnounced;
    }

    bool PeerPlugin::scheduledForDeletingInNextTorrentPluginTick() const {
        return _scheduledForDeletingInNextTorrentPluginTick;
    }

    void PeerPlugin::setScheduledForDeletingInNextTorrentPluginTick(bool scheduledForDeletingInNextTorrentPluginTick) {
        _scheduledForDeletingInNextTorrentPluginTick = scheduledForDeletingInNextTorrentPluginTick;
    }

    libtorrent::error_code PeerPlugin::deletionErrorCode() const
    {
        return _deletionErrorCode;
    }

    void PeerPlugin::setDeletionErrorCode(const libtorrent::error_code &deletionErrorCode)
    {
        _deletionErrorCode = deletionErrorCode;
    }

}
}
