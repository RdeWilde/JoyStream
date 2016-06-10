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

        struct Policy {

            Policy(bool installPluginOnPeersWithoutExtension, bool installPluginOnPeersMisbehavingDuringExtendedHandshake)
                : installPluginOnPeersWithoutExtension(installPluginOnPeersWithoutExtension)
                , installPluginOnPeersMisbehavingDuringExtendedHandshake(installPluginOnPeersMisbehavingDuringExtendedHandshake) {
            }

            Policy() : Policy(false, false) { }

            /**
             * I suspect these will be dropped when libtorrent behaviour is
             * better understood.
             */

            //
            bool installPluginOnPeersWithoutExtension;

            //
            bool installPluginOnPeersMisbehavingDuringExtendedHandshake;
        };

        PeerPlugin(TorrentPlugin * plugin,
                   libtorrent::bt_peer_connection * connection,
                   const Policy & policy,
                   const std::string & bep10ClientIdentifier);

        virtual ~PeerPlugin();

        //// Libtorrent hooks

        // This function is expected to return the name of
        // the plugin.
        virtual char const* type() const;

        // can add entries to the extension handshake
        // this is not called for web seeds
        virtual void add_handshake(libtorrent::entry & handshake);

        // called when the peer is being disconnected.
        virtual void on_disconnect(libtorrent::error_code const & ec);

        // called when the peer is successfully connected. Note that
        // incoming connections will have been connected by the time
        // the peer plugin is attached to it, and won't have this hook
        // called.
        virtual void on_connected();

        // throwing an exception from any of the handlers (except add_handshake)
        // closes the connection

        // this is called when the initial BT handshake is received. Returning false
        // means that the other end doesn't support this extension and will remove
        // it from the list of plugins.
        // this is not called for web seeds
        virtual bool on_handshake(char const* reserved_bits);

        // called when the extension handshake from the other end is received
        // if this returns false, it means that this extension isn't
        // supported by this peer. It will result in this peer_plugin
        // being removed from the peer_connection and destructed.
        // this is not called for web seeds
        virtual bool on_extension_handshake(libtorrent::lazy_entry const & handshake);
        //virtual bool on_extension_handshake(libtorrent::bdecode_node const&);

        // returning true from any of the message handlers
        // indicates that the plugin has handled the message.
        // it will break the plugin chain traversing and not let
        // anyone else handle the message, including the default
        // handler.
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

        // called when an extended message is received. If returning true,
        // the message is not processed by any other plugin and if false
        // is returned the next plugin in the chain will receive it to
        // be able to handle it. This is not called for web seeds.
        // thus function may be called more than once per incoming message, but
        // only the last of the calls will the ``body`` size equal the ``length``.
        // i.e. Every time another fragment of the message is received, this
        // function will be called, until finally the whole message has been
        // received. The purpose of this is to allow early disconnects for invalid
        // messages and for reporting progress of receiving large messages.
        virtual bool on_extended(int length, int msg, libtorrent::buffer::const_interval body);

        // this is not called for web seeds
        virtual bool on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body);

        // called when a piece that this peer participated in either
        // fails or passes the hash_check
        virtual void on_piece_pass(int index);
        virtual void on_piece_failed(int index);

        // called after a choke message has been sent to the peer
        virtual void sent_unchoke();

        // called when libtorrent think this peer should be disconnected.
        // if the plugin returns false, the peer will not be disconnected.
        virtual bool can_disconnect(libtorrent::error_code const & ec);

        // called approximately once every second
        virtual void tick();

        // called each time a request message is to be sent. If true
        // is returned, the original request message won't be sent and
        // no other plugin will have this function called.
        virtual bool write_request(libtorrent::peer_request const & peerRequest);

        // Sends extended message to peer, does not take ownership of pointer
        void send(const joystream::protocol_wire::ExtendedMessagePayload * extendedMessage);

        // Status of plugin
        status::PeerPlugin status() const;

        // Getters
        void disconnect(libtorrent::error_code &);
/**
        bool peerTimedOut(int maxDelay) const;

        BEPSupportStatus peerBEP10SupportStatus() const;

        BEPSupportStatus peerBitSwaprBEPSupportStatus() const;

        libtorrent::tcp::endpoint endPoint() const;

        libtorrent::error_code deletionErrorCode() const;
        */

    private:

        // Torrent plugin for torrent
        TorrentPlugin * _plugin;

        // Connection to peer for this plugin
        libtorrent::bt_peer_connection * _connection;

        // Policy governing runtime behaviour of plugin
        Policy _policy;

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
