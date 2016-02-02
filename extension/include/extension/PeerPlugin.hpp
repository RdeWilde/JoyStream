/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_PEER_PLUGIN_HPP
#define JOYSTREAM_EXTENSION_PEER_PLUGIN_HPP

#include <extension/BEPSupportStatus.hpp>
#include <extension/PeerModeAnnounced.hpp>
#include <extension/ExtendedMessageIdMapping.hpp>
#include <libtorrent/extensions.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/lazy_entry.hpp>
#include <libtorrent/bitfield.hpp>
#include <libtorrent/peer_request.hpp>
#include <libtorrent/bt_peer_connection.hpp>
#include <libtorrent/disk_buffer_holder.hpp>
#include <libtorrent/buffer.hpp>
#include <libtorrent/peer_id.hpp> // sha1_hash

#include <QDateTime>
#include <QQueue>

#include <string> // can't forward declare std::string

namespace joystream {

namespace protocol {

    class ExtendedMessagePayload;
    class Observe;
    class Buy;
    class Sell;
    class JoinContract;
    class JoiningContract;
    class SignRefund;
    class RefundSigned;
    class Ready;
    class RequestFullPiece;
    class FullPiece;
    class Payment;
}

namespace extension {

    class TorrentPlugin;
    enum class PluginMode;

    /**
     * @brief Abstract parent type for peer plugins, in seller, buyer and observer mode.
     */
    class PeerPlugin : public libtorrent::peer_plugin {

    public:

        // Constructor
        PeerPlugin(TorrentPlugin * plugin,
                   libtorrent::bt_peer_connection * connection,
                   const std::string & bep10ClientIdentifier,
                   bool scheduledForDeletingInNextTorrentPluginTick,
                   QLoggingCategory & category);

        /**
         * All virtual functions below should ONLY be called by libtorrent network thread,
         * never by other threads, as this causes synchronization failures.
         */
        // Destructor
        virtual ~PeerPlugin() = 0;

        // Libtorrent callback
        virtual char const* type() const = 0;
        virtual void add_handshake(libtorrent::entry & handshake);
        virtual void on_disconnect(libtorrent::error_code const & ec) = 0;
        virtual void on_connected() = 0;
        virtual bool on_handshake(char const* reserved_bits);
        virtual bool on_extension_handshake(libtorrent::lazy_entry const & handshake);
        //virtual bool on_extension_handshake(libtorrent::bdecode_node const&);
        virtual bool on_have(int index) = 0;
        virtual bool on_bitfield(libtorrent::bitfield const & bitfield) = 0;
        virtual bool on_have_all() = 0;
        virtual bool on_reject(libtorrent::peer_request const & peerRequest) = 0;
        virtual bool on_request(libtorrent::peer_request const & peerRequest) = 0;
        virtual bool on_unchoke() = 0;
        virtual bool on_interested() = 0;
        virtual bool on_allowed_fast(int index) = 0;
        virtual bool on_have_none() = 0;
        virtual bool on_choke() = 0;
        virtual bool on_not_interested() = 0;
        virtual bool on_piece(libtorrent::peer_request const & piece, libtorrent::disk_buffer_holder & data) = 0;
        virtual bool on_suggest(int index) = 0;
        virtual bool on_cancel(libtorrent::peer_request const & peerRequest) = 0;
        virtual bool on_dont_have(int index) = 0;
        virtual void sent_unchoke() = 0;
        virtual bool can_disconnect(libtorrent::error_code const & ec) = 0;
        virtual bool on_extended(int length, int msg, libtorrent::buffer::const_interval body);
        virtual bool on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body) = 0;
        virtual void on_piece_pass(int index) = 0;
        virtual void on_piece_failed(int index) = 0;
        virtual void tick() = 0;
        virtual bool write_request(libtorrent::peer_request const & peerRequest) = 0;

        /**
         * Subroutines for libtorrent thread.
         */

        // Processig routine for peer plugin requests, request pointer is owned by plugin dispatcher
        //void processPeerPluginRequest(const PeerPluginRequest * peerPluginRequest);

        // Sends extended message to peer, does not take ownership of pointer
        void sendExtendedMessage(const joystream::protocol::ExtendedMessagePayload & extendedMessage);

        // Determines the message type, calls correct handler, then frees message
        void processExtendedMessage(joystream::protocol::ExtendedMessagePayload * extendedMessage);

        // Getters
        libtorrent::bt_peer_connection * connection();

        bool peerTimedOut(int maxDelay) const;
        BEPSupportStatus peerBEP10SupportStatus() const;
        BEPSupportStatus peerBitSwaprBEPSupportStatus() const;
        libtorrent::tcp::endpoint endPoint() const;

        bool connectionAlive() const;
        bool lastReceivedMessageWasMalformed() const;
        virtual PluginMode mode() const = 0;
        virtual quint64 balance() const = 0;

        PeerModeAnnounced peerModeAnnounced() const;
        void setPeerModeAnnounced(PeerModeAnnounced peerModeAnnounced);

        bool scheduledForDeletingInNextTorrentPluginTick() const;
        void setScheduledForDeletingInNextTorrentPluginTick(bool scheduledForDeletingInNextTorrentPluginTick);

        libtorrent::error_code deletionErrorCode() const;
        void setDeletionErrorCode(const libtorrent::error_code &deletionErrorCode);

    private:

        // Torrent plugin for torrent
        TorrentPlugin * _plugin;

    protected:

        // Connection to peer for this plugin
        libtorrent::bt_peer_connection * _connection;

        // Client identifier used in bep10 handshake v-key
        std::string _bep10ClientIdentifier;

        // Endpoint: can be deduced from connection, but is worth keeping if connection pointer becomes invalid
        libtorrent::tcp::endpoint _endPoint;

        // Announced peer mode
        PeerModeAnnounced _peerModeAnnounced;

        // Time since last message was sent to peer, is used to judge if peer has timed out
        QTime _timeSinceLastMessageSent;

        // Last message arriving in on_extended() which was malformed accoridng
        // to ExtendedMessagePayload::fromRaw().
        bool _lastReceivedMessageWasMalformed;

        // Last message was not compatible with state of plugin
        bool _lastMessageWasStateIncompatible;

        // Set when peer plugin should be disconnected and deleted by corresponding torrent plugin
        bool _scheduledForDeletingInNextTorrentPluginTick;
        libtorrent::error_code _deletionErrorCode;

        // Mapping from messages to BEP10 ID of client
        ExtendedMessageIdMapping _clientMapping;

        // Mapping from messages to BEP10 ID of peer
        ExtendedMessageIdMapping _peerMapping;

        // Indicates whether peer supports BEP10
        BEPSupportStatus _peerBEP10SupportStatus;

        // Indicates whether peer supports BEP43 .. BitSwapr
        BEPSupportStatus _peerBitSwaprBEPSupportStatus;

        // Logging category
        QLoggingCategory & _category;

        // Processess message in subclass,
        // are only called if extended handshake has been completed successfully.
        virtual void processObserve(const joystream::protocol::Observe * m) = 0;
        virtual void processBuy(const joystream::protocol::Buy * m) = 0;
        virtual void processSell(const joystream::protocol::Sell * m) = 0;
        virtual void processJoinContract(const joystream::protocol::JoinContract * m) = 0;
        virtual void processJoiningContract(const joystream::protocol::JoiningContract * m) = 0;
        virtual void processSignRefund(const joystream::protocol::SignRefund * m) = 0;
        virtual void processRefundSigned(const joystream::protocol::RefundSigned * m) = 0;
        virtual void processReady(const joystream::protocol::Ready * m) = 0;
        virtual void processRequestFullPiece(const joystream::protocol::RequestFullPiece * m) = 0;
        virtual void processFullPiece(const joystream::protocol::FullPiece * m) = 0;
        virtual void processPayment(const joystream::protocol::Payment * m) = 0;

    };

}
}

#endif // JOYSTREAM_EXTENSION_PEER_PLUGIN_HPP
