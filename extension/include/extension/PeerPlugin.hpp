/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_PEERPLUGIN_HPP
#define JOYSTREAM_EXTENSION_PEERPLUGIN_HPP

#include <extension/BEPSupportStatus.hpp>
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

#include <string>
#include <chrono>

namespace joystream {
namespace protocol_wire {
    class ExtendedMessagePayload;
}
namespace extension {
namespace status {
    struct PeerPlugin;
}

    class TorrentPlugin;

    class PeerPlugin : public libtorrent::peer_plugin {

    public:

        PeerPlugin(TorrentPlugin * plugin,
                   libtorrent::bt_peer_connection * connection,
                   const std::string & bep10ClientIdentifier);

        virtual ~PeerPlugin();

        /**
         * All virtual functions below should ONLY be called by libtorrent network thread,
         * never by other threads, as this causes synchronization failures.
         */

        // Libtorrent callback
        virtual char const* type() const;
        virtual void add_handshake(libtorrent::entry & handshake);
        virtual void on_disconnect(libtorrent::error_code const & ec);
        virtual void on_connected();
        virtual bool on_handshake(char const* reserved_bits);
        virtual bool on_extension_handshake(libtorrent::lazy_entry const & handshake);
        //virtual bool on_extension_handshake(libtorrent::bdecode_node const&);
        virtual bool on_have(int index);
        virtual bool on_bitfield(libtorrent::bitfield const & bitfield);
        virtual bool on_have_all();
        virtual bool on_reject(libtorrent::peer_request const & peerRequest);
        virtual bool on_request(libtorrent::peer_request const & peerRequest);
        virtual bool on_unchoke();
        virtual bool on_interested();
        virtual bool on_allowed_fast(int index);
        virtual bool on_have_none();
        virtual bool on_choke();
        virtual bool on_not_interested();
        virtual bool on_piece(libtorrent::peer_request const & piece, libtorrent::disk_buffer_holder & data);
        virtual bool on_suggest(int index);
        virtual bool on_cancel(libtorrent::peer_request const & peerRequest);
        virtual bool on_dont_have(int index);
        virtual void sent_unchoke();
        virtual bool can_disconnect(libtorrent::error_code const & ec);
        virtual bool on_extended(int length, int msg, libtorrent::buffer::const_interval body);
        virtual bool on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body);
        virtual void on_piece_pass(int index);
        virtual void on_piece_failed(int index);
        virtual void tick();
        virtual bool write_request(libtorrent::peer_request const & peerRequest);

        /**
         * Subroutines for libtorrent thread.
         */

        // Sends extended message to peer, does not take ownership of pointer
        void send(const joystream::protocol_wire::ExtendedMessagePayload & extendedMessage);

        status::PeerPlugin status() const;

        // Getters
        libtorrent::bt_peer_connection * connection();

        bool peerTimedOut(int maxDelay) const;

        BEPSupportStatus peerBEP10SupportStatus() const;

        BEPSupportStatus peerBitSwaprBEPSupportStatus() const;

        libtorrent::tcp::endpoint endPoint() const;

        /**
        libtorrent::error_code deletionErrorCode() const;
        void setDeletionErrorCode(const libtorrent::error_code &deletionErrorCode);
        */

    private:

        // Torrent plugin for torrent
        TorrentPlugin * _plugin;

        // Connection to peer for this plugin
        libtorrent::bt_peer_connection * _connection;

        // Client identifier used in bep10 handshake v-key
        std::string _bep10ClientIdentifier;

        // Endpoint: can be deduced from connection, but is worth keeping if connection pointer becomes invalid
        libtorrent::tcp::endpoint _endPoint;

        // Time since last message was sent to peer, is used to judge if peer has timed out
        //std::chrono::time_point<std::chrono::system_clock> _whenLastMessageSent;

        // Last message arriving in on_extended() which was malformed according
        // to ExtendedMessagePayload::fromRaw().
        //bool _lastReceivedMessageWasMalformed;

        // Set when peer plugin should be disconnected and deleted by corresponding torrent plugin
        //bool _scheduledForDeletingInNextTorrentPluginTick;
        //libtorrent::error_code _deletionErrorCode;

        // Mapping from messages to BEP10 ID of client
        ExtendedMessageIdMapping _clientMapping;

        // Mapping from messages to BEP10 ID of peer
        ExtendedMessageIdMapping _peerMapping;

        // Indicates whether peer supports BEP10
        BEPSupportStatus _peerBEP10SupportStatus;

        // Indicates whether peer supports Payments BEP
        BEPSupportStatus _peerPaymentBEPSupportStatus;
    };

}
}

#endif // JOYSTREAM_EXTENSION_PEERPLUGIN_HPP
