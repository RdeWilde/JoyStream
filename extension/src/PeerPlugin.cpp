/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/PeerPlugin.hpp>
#include <extension/TorrentPlugin.hpp>
#include <extension/Exception.hpp>
#include <extension/Status.hpp>
#include <extension/Alert.hpp>
#include <protocol_wire/protocol_wire.hpp>
#include <libtorrent/bt_peer_connection.hpp> // bt_peer_connection, bt_peer_connection::msg_extended
#include <libtorrent/socket_io.hpp>
#include <libtorrent/peer_info.hpp>
#include <libtorrent/alert_manager.hpp>

namespace joystream {
namespace extension {

    PeerPlugin::PeerPlugin(TorrentPlugin * plugin,
                           const libtorrent::torrent_handle & torrent,
                           const libtorrent::peer_connection_handle & connection,
                           uint minimumMessageId,
                           libtorrent::alert_manager * alertManager)
        : _undead(false)
        , _plugin(plugin)
        , _torrent(torrent)
        , _connection(connection)
        , _minimumMessageId(minimumMessageId)
        , _alertManager(alertManager)
        , _endPoint(connection.remote())
        , _clientMapping(ExtendedMessageIdMapping::consecutiveIdsStartingAt(_minimumMessageId))
        , _sendUninstallMappingOnNextExtendedHandshake(false)
        , _peerBEP10SupportStatus(BEPSupportStatus::unknown)
        , _peerPaymentBEPSupportStatus(BEPSupportStatus::supported) {

        // 0 is not a valid minimum message id
        if(_minimumMessageId == 0)
            throw exception::InvalidMinimumMessageIdException();
    }

    PeerPlugin::~PeerPlugin() {
        std::clog << "~PeerPlugin() called.";

        // Send removal notification
        _alertManager->emplace_alert<alert::PeerPluginRemoved>(_torrent, _endPoint, _connection.pid());
    }

    char const* PeerPlugin::type() const {
        return BEP10_EXTENSION_NAME;
    }

    void PeerPlugin::add_handshake(libtorrent::entry & handshake) {

        assert(!_undead);

        // We can safely assume hanshake has proper structure, that is
        // 1) is dictionary entry
        // 2) has key m which maps to a dictionary entry

        // Add top level key for extension which encodes protocol version
        handshake[BEP10_EXTENSION_NAME] = protocol_statemachine::CBStateMachine::protocolVersion.toString();

        // Add m keys for extended message ids
        libtorrent::entry::dictionary_type & m = handshake["m"].dict();

        // If sessino is stopped, then we only send uninstall mapping, at most
        if(_plugin->sessionState() == protocol_session::SessionState::stopped) {

            // If this is first handshake after stopping the session, then
            // and uninstall mapping is sent
            if(_sendUninstallMappingOnNextExtendedHandshake) {

                // Write uninstall mapping
                // May throw exception::MessageAlreadyPresentException if
                // plugin is being used incorrectly by developer
                ExtendedMessageIdMapping::writeUninstallMappingToMDictionary(m);

                // Don't do on next handshake
                _sendUninstallMappingOnNextExtendedHandshake = false;
            }

        } else {

            assert(!_sendUninstallMappingOnNextExtendedHandshake);
            assert(!_clientMapping.empty());

            // Write proper mapping to dictionary
            // May throw exception::MessageAlreadyPresentException if
            // plugin is being used incorrectly by developer
            _clientMapping.writeToMDictionary(m);
        }
    }

    void PeerPlugin::on_disconnect(libtorrent::error_code const & ec) {

        std::clog << "on_disconnect ["<< (_connection.is_outgoing() ? "outgoing" : "incoming") << "]:" << ec.message().c_str();

        // If connection is undead, then this callback should be ignored.
        if(_undead)
            return;

        // Otherwise, the disconnect was iniated by peer, and we should notify
        // the torrent plugin.
        _plugin->drop(_endPoint, ec, false);
    }

    void PeerPlugin::on_connected() {

        assert(!_undead);
    }

    bool PeerPlugin::on_handshake(char const * reserved_bits) {

        assert(!_undead);

        // The BEP10 docs say:
        // The bit selected for the extension protocol is bit 20 from
        // the right (counting starts at 0).
        // So (reserved_byte[5] & 0x10) is the expression to use for
        // checking if the client supports extended messaging.

        // Check if BEP10 is enabled
        if(reserved_bits[5] & 0x10) {

            std::clog << "BEP10 supported in handshake." << std::endl;

            // bep10 is supported
            _peerBEP10SupportStatus = BEPSupportStatus::supported;

        } else {

            std::clog << "BEP10 not supported in handshake." << std::endl;

            // bep10 is not supported
            _peerBEP10SupportStatus = BEPSupportStatus::not_supported;

            // hence it also cannot support this spesific extension
            _peerPaymentBEPSupportStatus  = BEPSupportStatus::not_supported;
        }

        // Plugin is installed on all peers
        return true;
    }

    bool PeerPlugin::on_extension_handshake(libtorrent::bdecode_node const & handshake) {

        assert(!_undead);

        // In all cases, we ask libtorrent to keep us around (by always returning true),
        // even if this extension is not supported, or even if we just initiated a peer
        // disconnect just prior.

        // Write what client is trying to handshake us, should now be possible given initial hand shake
        libtorrent::peer_info peerInfo;
        _connection.get_peer_info(peerInfo);

        std::clog << "on_extension_handshake[" << peerInfo.client.c_str() << "]";

        // Check that BEP10 was actually supported, if it wasnt, then the peer is misbehaving
        if(_peerBEP10SupportStatus != BEPSupportStatus::supported) {

            // Remove peer
            libtorrent::error_code ec; // "Peer misbehaved: didn't support BEP10, but it sent extended handshake."
            drop(ec);

            return true;
        }

        /// Validate structure of handshake dictionary

        // If its not a dictionary, we are done
        if(handshake.type() != libtorrent::bdecode_node::dict_t) {

            // Mark peer as not supporting this extension
            _peerPaymentBEPSupportStatus = BEPSupportStatus::not_supported;

            // Remove peer
            libtorrent::error_code ec; // "Malformed handshake received: not dictionary."
            drop(ec);

            // Keep plugin around
            return true;
        }

        // Check if plugin key is there, and maps to valid protocol version
        std::string versionString = handshake.dict_find_string_value(BEP10_EXTENSION_NAME);

        if(versionString == "") {

            // Mark peer as not supporting this extension
            _peerPaymentBEPSupportStatus = BEPSupportStatus::not_supported;

            // Keep plugin around
            return true;
        }

        // Attempt to decode protocol version
        try {

            _protocolVersionOfPeer = common::MajorMinorSoftwareVersion::fromString(versionString);

        } catch (const common::MajorMinorSoftwareVersion::InvalidProtocolVersionStringException &) {

            // Mark peer as not supporting this extension
            _peerPaymentBEPSupportStatus = BEPSupportStatus::not_supported;

            // Remove peer
            libtorrent::error_code ec; // "Malformed protocol version format provided: " << versionString
            drop(ec);

            // Keep us around
            return true;
        }

        // Try to extract m key, if its not present, then we are done
        libtorrent::bdecode_node m = handshake.dict_find_dict("m");

        if(!m) {

            // Mark peer as not supporting this extension
            _peerPaymentBEPSupportStatus = BEPSupportStatus::not_supported;

            // Remove peer
            libtorrent::error_code ec; // "Malformed handshake received: m key not present."
            drop(ec);

            // Keep us around
            return true;
        }

        // Check if it is a dictionary entry
        if(m.type() != libtorrent::bdecode_node::dict_t) {

            // Mark peer as not supporting this extension
            _peerPaymentBEPSupportStatus  = BEPSupportStatus::not_supported;

            // Remove peer
            libtorrent::error_code ec; // "Malformed handshake received: m key not mapping to dictionary."
            drop(ec);

            // Keep us around
            return true;
        }

        // Get peer mapping
        try {

            // Store fully valid (non-uninstall) mapping of peer
            _peerMapping = ExtendedMessageIdMapping::fromMDictionary(m);

        } catch(const exception::InvalidMessageMappingDictionary & e) {

            if(e.problem == exception::InvalidMessageMappingDictionary::Problem::UninstallMappingFound) {

                std::clog << "Uninstall mapping found." << std::endl;

                // If peer hasn't previously sent a valid mapping,
                // then it is misbehaving
                if(_peerMapping.empty()) {

                    // Remove peer
                    libtorrent::error_code ec; // "Peer misbehaved: sent uninstall mapping, despite not recently annoncing valid mapping to uninstall."
                    drop(ec);

                    // Keep us around
                    return true;
                }

                // Discard old mapping
                _peerMapping.clear();

                // Mark peer as not supporting this extension
                _peerPaymentBEPSupportStatus  = BEPSupportStatus::not_supported;

                // Remove from session, if present
                _plugin->removeFromSession(_endPoint);

                return true;

            } else {

                // Remove peer
                libtorrent::error_code ec;
                drop(ec);

                // Keep us around
                return true;
            }

        }

        std::clog << "Found extension handshake for peer " << libtorrent::print_endpoint(_endPoint) << std::endl;

        // All messages were present, hence the protocol is supported
        _peerPaymentBEPSupportStatus = BEPSupportStatus::supported;

        // Add peer to session if it is currently not stopped
        if(_plugin->sessionState() != protocol_session::SessionState::stopped) {

            std::clog << "Added peer to non-stopped session" << std::endl;

            // NB: in the future, supply _protocolVersionOfPeer to session?

            _plugin->addToSession(_endPoint);

        } else
            std::clog << "Peer not added to stopped session" << std::endl;

        // Keep us around
        return true;
    }

    bool PeerPlugin::on_have(int) {
        assert(!_undead);
        return true; // overrid default handler
    }

    bool PeerPlugin::on_bitfield(libtorrent::bitfield const &) {
        assert(!_undead);
        return true; // overrid default handler
    }

    bool PeerPlugin::on_have_all() {
        assert(!_undead);
        return true; // overrid default handler
    }

    bool PeerPlugin::on_reject(libtorrent::peer_request const &) {
        assert(!_undead);
        return true; // overrid default handler
    }

    bool PeerPlugin::on_request(libtorrent::peer_request const &) {

        assert(!_undead);

        TorrentPlugin::LibtorrentInteraction e = _plugin->libtorrentInteraction();

        if(e == TorrentPlugin::LibtorrentInteraction::BlockUploading ||
           e == TorrentPlugin::LibtorrentInteraction::BlockUploadingAndDownloading)
            return true; // don't let anyone else handle the message, including the default
        else
            return false; // allow next handler
    }

    bool PeerPlugin::on_unchoke() {
        assert(!_undead);
        return true; // overrid default handler
    }

    bool PeerPlugin::on_interested() {
        assert(!_undead);
        return true; // overrid default handler
    }

    bool PeerPlugin::on_allowed_fast(int) {
        assert(!_undead);
        return true; // overrid default handler
    }

    bool PeerPlugin::on_have_none() {
        assert(!_undead);
        return true; // overrid default handler
    }

    bool PeerPlugin::on_choke() {
        assert(!_undead);
        return true; // overrid default handler
    }

    bool PeerPlugin::on_not_interested() {
        assert(!_undead);
        return true; // overrid default handler
    }

    bool PeerPlugin::on_piece(libtorrent::peer_request const &, libtorrent::disk_buffer_holder &) {

        assert(!_undead);

        TorrentPlugin::LibtorrentInteraction e = _plugin->libtorrentInteraction();

        if(e == TorrentPlugin::LibtorrentInteraction::BlockDownloading ||
           e == TorrentPlugin::LibtorrentInteraction::BlockUploadingAndDownloading)
            return true; // don't let anyone else handle the message, including the default
        else
            return false; // allow next handler
    }

    bool PeerPlugin::on_suggest(int) {
        assert(!_undead);
        return true; // overrid default handler
    }

    bool PeerPlugin::on_cancel(libtorrent::peer_request const &) {
        assert(!_undead);
        return true; // overrid default handler
    }

    bool PeerPlugin::on_dont_have(int) {
        assert(!_undead);
        return true; // overrid default handler
    }

    bool PeerPlugin::can_disconnect(libtorrent::error_code const &) {

        //std::clog << "can_disconnect: " << ec.message() << std::endl;

        return false;
    }

    bool PeerPlugin::on_extended(int length, int msg, libtorrent::buffer::const_interval body) {

        assert(!_undead);

        /**
        // Check peer plugin integrity
        if(_scheduledForDeletingInNextTorrentPluginTick) {

            std::clog << "Ignoring extended message since peer_plugin is scheduled for deletion.";
            return false;
        }
        */

        // If this peer is not part of this session, then
        // we ignore the message and ask all other plugins to do the same.
        // This could happen if the peer is malicious, or if the connection .... <what here?>
        if(_plugin->_session.mode() == protocol_session::SessionMode::not_set) {
            std::clog << "Ignoring extended message, session mode not set" << std::endl;
            return true;
        }

        if(!_plugin->_session.hasConnection(_endPoint)) {
            std::clog << "Ignoring extended message, connection not in session" << std::endl;
            return true;
        }

        assert(_peerPaymentBEPSupportStatus == BEPSupportStatus::supported);

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

        // Is it a message for this extension?
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
              << joystream::protocol_wire::messageName(m->messageType());

            // Remove this peer
            libtorrent::error_code ec; // <- s
            drop(ec);

            return true;
        }

        // Was message malformed
        if(m == NULL) {

            std::clog << "Extended Message was Malformed" << std::endl;
            // Remove this peer
            libtorrent::error_code ec; // <-- "Malformed extended message received, removing."
            drop(ec);

        } else {

            // Process message
            try {
                _plugin->processExtendedMessage(_endPoint, *m);
            } catch(std::exception &e) {
                std::clog << "Error processing Extended Message "<< e.what() << std::endl;
                libtorrent::error_code ec;
                drop(ec);
            }

            // Delete message
            delete m;
        }

        // No other plugin should process message
        return true;
    }

    bool PeerPlugin::on_unknown_message(int, int, libtorrent::buffer::const_interval) {
        assert(!_undead);

        return true; // allow other handlers to process
    }

    void PeerPlugin::on_piece_pass(int) {
        assert(!_undead);
    }

    void PeerPlugin::on_piece_failed(int) {
        assert(!_undead);
    }

    void PeerPlugin::sent_unchoke() {
        assert(!_undead);
    }

    void PeerPlugin::tick() {
        assert(!_undead);
    }

    bool PeerPlugin::write_request(libtorrent::peer_request const &) {

        assert(!_undead);

        TorrentPlugin::LibtorrentInteraction e = _plugin->libtorrentInteraction();

        if(e == TorrentPlugin::LibtorrentInteraction::BlockDownloading ||
           e == TorrentPlugin::LibtorrentInteraction::BlockUploadingAndDownloading)
            return true; // block sending request
        else
            return false; // allow sending request
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

        // BEP10 message id: should always be 20 according to BEP10 spec
        stream << static_cast<quint8>(libtorrent::bt_peer_connection::msg_extended);

        // Extended message id
        stream << static_cast<quint8>(_peerMapping.id(extendedMessagePayload->messageType()));

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
            _connection.send_buffer(constData, byteArray.length());

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

    status::PeerPlugin PeerPlugin::status(const boost::optional<protocol_session::status::Connection<libtorrent::tcp::endpoint>> & connections) const {
        return status::PeerPlugin(_endPoint,
                                  _peerBEP10SupportStatus,
                                  _peerPaymentBEPSupportStatus,
                                  connections);
    }

    bool PeerPlugin::undead() const  {
        return _undead;
    }

    void PeerPlugin::setUndead(bool undead) {
        _undead = undead;
    }

    libtorrent::peer_connection_handle PeerPlugin::connection() const {
        return _connection;
    }

    void PeerPlugin::setSendUninstallMappingOnNextExtendedHandshake(bool s) {
        _sendUninstallMappingOnNextExtendedHandshake = s;
    }

    void PeerPlugin::drop(const libtorrent::error_code & ec) {
        _plugin->drop(_endPoint, ec);
    }

    /**
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
