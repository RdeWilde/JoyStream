/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_TORRENTPLUGIN_HPP
#define JOYSTREAM_EXTENSION_TORRENTPLUGIN_HPP

#include <extension/PeerPlugin.hpp>
#include <protocol_session/protocol_session.hpp>

// For QSet: uint qHash(const libtorrent::tcp::endpoint & endpoint)
#include <common/LibtorrentUtilities.hpp>

#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>

#include <map>

namespace libtorrent {
    class read_piece_alert;
}

namespace joystream {
namespace extension {
namespace request {
    class TorrentPluginRequest;
}

    class Plugin;

    class TorrentPlugin : public libtorrent::torrent_plugin {

    public:

        struct Policy {

            Policy(bool banPeersWithoutExtension,
                   bool banPeersWithPastMalformedExtendedMessage,
                   const PeerPlugin::Policy & peerPolicy)
                : banPeersWithoutExtension(banPeersWithoutExtension)
                , banPeersWithPastMalformedExtendedMessage(banPeersWithPastMalformedExtendedMessage)
                , peerPolicy(peerPolicy) {
            }

            Policy() : Policy(false, false, PeerPlugin::Policy()) { }

            // Should TorrenPlugin::new_connection accept a peer which
            // is known to not have extension from before.
            bool banPeersWithoutExtension;

            // Should TorrenPlugin::new_connection accept a peer which
            // is known to have sent a malformed extended message before.
            bool banPeersWithPastMalformedExtendedMessage;

            // Policy for peer plugins
            PeerPlugin::Policy peerPolicy;
        };

        TorrentPlugin(Plugin * plugin,
                      const boost::shared_ptr<libtorrent::torrent> & torrent,
                      const std::string & bep10ClientIdentifier,
                      const Policy & policy);

        virtual ~TorrentPlugin();

        //// Libtorrent hooks

        // This function is called each time a new peer is connected to the torrent.
        // You may choose to ignore this by just returning a default constructed shared_ptr (in which case you don't need to override this member function).
        // If you need an extension to the peer connection (which most plugins do) you are supposed to return an instance of your peer_plugin class.
        // Which in turn will have its hook functions called on event specific to that peer.
        // The peer_connection_handle will be valid as long as the shared_ptr is being held by the torrent object. So, it is generally a good idea to not
        // keep a shared_ptr to your own peer_plugin. If you want to keep references to it, use weak_ptr.
        // If this function throws an exception, the connection will be closed.
        virtual boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection * connection);

        // These hooks are called when a piece passes the hash check or fails the hash check, respectively. The index is the piece index that was downloaded.
        // It is possible to access the list of peers that participated in sending the piece through the torrent and the piece_picker.
        virtual void on_piece_pass(int index);
        virtual void on_piece_failed(int index);

        // This hook is called approximately once per second. It is a way of making it easy for plugins to do timed events, for sending messages or whatever.
        virtual void tick();

        // These hooks are called when the torrent is paused and unpaused respectively. The return value indicates if the event was handled.
        // A return value of true indicates that it was handled, and no other plugin after this one will have this hook function called,
        // and the standard handler will also not be invoked. So, returning true effectively overrides the standard behavior of pause or unpause.
        // Note that if you call pause() or resume() on the torrent from your handler it will recurse back into your handler,
        // so in order to invoke the standard handler, you have to keep your own state on whether you want standard behavior or overridden behavior.
        virtual bool on_resume();
        virtual bool on_pause();

        // This function is called when the initial files of the torrent have been checked. If there are no files to check, this function is called immediately.
        // i.e. This function is always called when the torrent is in a state where it can start downloading.
        virtual void on_files_checked();

        // called when the torrent changes state the state is one of torrent_status::state_t enum members
        virtual void on_state(int s);

        // called every time a new peer is added to the peer list. This is before the peer is connected to.
        // For flags, see torrent_plugin::flags_t. The source argument refers to the source where we learned
        // about this peer from. It's a bitmask, because many sources may have told us about the same peer.
        // For peer source flags, see peer_info::peer_source_flags.
        virtual void on_add_peer(const libtorrent::tcp::endpoint & endPoint, int src, int flags);

        //// Plugin calls

        // Handle request from libtorrent client. Takes ownership of request object.
        void handle(const request::TorrentPluginRequest * r);

        // Alert from plugin about a piece being read.
        // Is required when session is selling.
        void pieceRead(const libtorrent::read_piece_alert * alert);

    private:

        //// PeerPlugin notifications

        friend class PeerPlugin;

        // Adds peer correspoinding to given endpoint to session,
        // is called when peer has sucessfully completed extended handshake.
        // Not when connection is established, as in TorrentPlugin::new_connection
        void addPeerToSession(const libtorrent::tcp::endpoint &);

        // Disocnnects peer, removes corresponding plugin from map
        void disconnectPeer(const libtorrent::tcp::endpoint &);

        // Determines the message type, calls correct handler, then frees message
        void processExtendedMessage(const libtorrent::tcp::endpoint &, const joystream::protocol_wire::ExtendedMessagePayload & extendedMessage);

        //// Protocol session hooks

        protocol_session::RemovedConnectionCallbackHandler removeConnection() const;
        protocol_session::BroadcastTransaction broadcastTransaction() const;
        protocol_session::FullPieceArrived fullPieceArrived() const;
        protocol_session::LoadPieceForBuyer loadPieceForBuyer() const;
        protocol_session::ClaimLastPayment claimLastPayment() const;
        protocol_session::AnchorAnnounced anchorAnnounced() const;

        //// Members

        // Parent plugin
        // Should this be boost::shared_ptr, since life time of object is managed by it?
        // on the other hand, we loose Plugin behaviour through libtorrent::plugin pointer, which we need!
        Plugin * _plugin;

        // Torrent for this torrent_plugin
        boost::weak_ptr<libtorrent::torrent> _torrent;

        // Client identifier used in bep10 handshake v-key
        std::string _bep10ClientIdentifier;

        // Parametrised runtime behaviour
        Policy _policy;

        // Endpoints corresponding to peers known to not have extension.
        // Is populated by previous failed extended handshakes.
        std::set<libtorrent::tcp::endpoint> _extensionless;

        // Endpoints corresponding to peers which have sent malformed extended message
        // including handshake.
        std::set<libtorrent::tcp::endpoint> _sentMalformedExtendedMessage;

        // Torrent info hash
        libtorrent::sha1_hash _infoHash;

        // Maps endpoint to weak peer plugin pointer, is peer_plugin, since this is
        // Libtorrent docs (http://libtorrent.org/reference-Plugins.html#peer_plugin):
        // The peer_connection will be valid as long as the shared_ptr is being held by the
        // torrent object. So, it is generally a good idea to not keep a shared_ptr to
        // your own peer_plugin. If you want to keep references to it, use weak_ptr.
        // NB: All peers are added, while not all are added to _session, see below.
        std::map<libtorrent::tcp::endpoint, boost::weak_ptr<PeerPlugin> > _peers;

        // Protocol session
        // NB: Only peers which support this extension will be added to session,
        // while all peers are added to _peers
        protocol_session::Session<libtorrent::tcp::endpoint> _session;

        /**
         * Hopefully we can ditch all of this, if we can delete connections in new_connection callback
         *
        // List of peer plugins scheduled for deletion
        //std::list<boost::weak_ptr<PeerPlugin> > _peersScheduledForDeletion;

        // Peers which should be deleted next tick().
        // A peer may end up here for one of the following reasons
        // (1) we determine in ::new_connection() that we don't want this connection.
        // Due to assertion constraint in libtorrent the connection cannot be disconneected here.
        //std::set<libtorrent::tcp::endpoint> _disconnectNextTick;
        */

        // While selling, this maintains mapping between piece index and peers that are
        // waiting for this piece to be read from disk.
        // Will typically just be one, but may be multiple - hence set is used
        std::map<int, std::set<libtorrent::tcp::endpoint> > _outstandingReadPieceRequests;

        //// Utilities

        // Send torrent plugin alert to libtorrent session
        void sendTorrentPluginAlert(const libtorrent::alert & alert);

        // Returns raw plugin pointer after asserted locking
        PeerPlugin * getRawPlugin(const libtorrent::tcp::endpoint &);

        // Returns raw torrent pointer after asserted locking
        libtorrent::torrent * getTorrent();

        // Returns torrent piece information based on current state of torrent
        protocol_session::TorrentPieceInformation torrentPieceInformation() const;
    };

}
}

#endif // JOYSTREAM_EXTENSION_TORRENTPLUGIN_HPP
