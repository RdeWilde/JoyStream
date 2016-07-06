/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_TORRENTPLUGIN_HPP
#define JOYSTREAM_EXTENSION_TORRENTPLUGIN_HPP

#include <extension/PeerPlugin.hpp>
#include <extension/Request.hpp>
#include <protocol_session/protocol_session.hpp>
#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>
#include <libtorrent/alert_types.hpp>
#include <map>

namespace joystream {
namespace extension {
namespace status {
    struct TorrentPlugin;
}

class Plugin;

class TorrentPlugin : public libtorrent::torrent_plugin {

public:

    struct Policy {

        Policy(bool banPeersWithPastMalformedExtendedMessage,
               bool banPeersWithPastMisbehavior,
               const PeerPlugin::Policy & peerPolicy)
            : banPeersWithPastMalformedExtendedMessage(banPeersWithPastMalformedExtendedMessage)
            , banPeersWithPastMisbehavior(banPeersWithPastMisbehavior)
            , peerPolicy(peerPolicy) {
        }

        Policy() : Policy(true, true, PeerPlugin::Policy()) { }

        // Should TorrenPlugin::new_connection accept a peer which
        // is known to have sent a malformed extended message before.
        bool banPeersWithPastMalformedExtendedMessage;

        // Should TorrenPlugin::new_connection accept a peer which
        // is known to have misbehaved prior.
        bool banPeersWithPastMisbehavior;

        // Policy for peer plugins
        PeerPlugin::Policy peerPolicy;
    };

    TorrentPlugin(Plugin * plugin,
                  const libtorrent::torrent_handle & torrent,
                  const std::string & bep10ClientIdentifier,
                  uint minimumMessageId,
                  const Policy & policy);

    virtual ~TorrentPlugin();

    /// Libtorrent hooks

    // This function is called each time a new peer is connected to the torrent.
    // You may choose to ignore this by just returning a default constructed shared_ptr (in which case you don't need to override this member function).
    // If you need an extension to the peer connection (which most plugins do) you are supposed to return an instance of your peer_plugin class.
    // Which in turn will have its hook functions called on event specific to that peer.
    // The peer_connection_handle will be valid as long as the shared_ptr is being held by the torrent object. So, it is generally a good idea to not
    // keep a shared_ptr to your own peer_plugin. If you want to keep references to it, use weak_ptr.
    // If this function throws an exception, the connection will be closed.
    virtual boost::shared_ptr<libtorrent::peer_plugin> new_connection(const libtorrent::peer_connection_handle &);

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
    //void handle(const request::TorrentPluginRequest * r);

    // Alert from plugin about a piece being read.
    // Is required when session is selling.
    void pieceRead(const libtorrent::read_piece_alert * alert);

    // Status
    status::TorrentPlugin status() const;

private:

    /// Torrent plugin request processing

    // Processes subroutine call
    friend class Plugin;
    template<class T> void process(const T &);

    /// PeerPlugin notifications

    friend class PeerPlugin;

    // State of session
    protocol_session::SessionState sessionState() const;

    // Adds peer correspoinding to given endpoint to session,
    // is called when peer has sucessfully completed extended handshake.
    // Not when connection is established, as in TorrentPlugin::new_connection
    void addToSession(const libtorrent::tcp::endpoint &);

    // Removes peer from session, if present
    void removeFromSession(const libtorrent::tcp::endpoint &);

    // Disconnects peer, removes corresponding plugin from map
    void drop(const libtorrent::tcp::endpoint &, const libtorrent::error_code &);

    // Determines the message type, calls correct handler, then frees message
    void processExtendedMessage(const libtorrent::tcp::endpoint &, const joystream::protocol_wire::ExtendedMessagePayload & extendedMessage);

    /// Protocol session hooks

    protocol_session::RemovedConnectionCallbackHandler<libtorrent::tcp::endpoint> removeConnection();
    protocol_session::BroadcastTransaction broadcastTransaction();
    protocol_session::FullPieceArrived<libtorrent::tcp::endpoint> fullPieceArrived();
    protocol_session::LoadPieceForBuyer<libtorrent::tcp::endpoint> loadPieceForBuyer();
    protocol_session::ClaimLastPayment<libtorrent::tcp::endpoint> claimLastPayment();
    protocol_session::AnchorAnnounced<libtorrent::tcp::endpoint> anchorAnnounced();

    /// Members

    // Parent plugin
    // Should this be boost::shared_ptr, since life time of object is managed by it?
    // on the other hand, we loose Plugin behaviour through libtorrent::plugin pointer, which we need!
    Plugin * _plugin;

    // Torrent for this torrent_plugin
    libtorrent::torrent_handle _torrent;

    // Client identifier used in bep10 handshake v-key
    const std::string _bep10ClientIdentifier;

    // Lowest all message id where libtorrent client can guarantee we will not
    // conflict with another libtorrent plugin (e.g. metadata, pex, etc.)
    const uint _minimumMessageId;

    // Parametrised runtime behaviour
    Policy _policy;

    // Endpoints corresponding to peers which have sent malformed extended message, including handshake.
    // Can be used to ban peers from connecting.
    std::set<libtorrent::tcp::endpoint> _sentMalformedExtendedMessage;

    // Endpoints corresponding to peers which have misbehaved
    // in one of following scenarios
    // a) PeerPlugin::on_extension_handshake: sent extended message, despite claiming not to support BEP10
    std::set<libtorrent::tcp::endpoint> _misbehavedPeers;

    // Peers which should have been deleted in ::new_connection event, but cannot, due to some invariant which
    // is asserted in on_disconnect about peer having to be on the peer_list.
    // NB: peers in this list have no plugin installed
    // NB: connection, rather than endpoint, is used as key, as connection is not recoverable later from libtorrent!!
    std::map<libtorrent::peer_connection *, libtorrent::error_code> _peerScheduledForDeletionForGivenError;

    // Torrent info hash
    const libtorrent::sha1_hash _infoHash;

    // Maps endpoint to corresponding peer_plugin, which is installed on all peers,
    // also those that don't even support BEP10, let alone this extension.
    // Is required to disrupt default libtorrent behaviour.
    //
    // Q: Why weak_ptr?
    // A: Libtorrent docs (http://libtorrent.org/reference-Plugins.html#peer_plugin):
    // The peer_connection will be valid as long as the shared_ptr is being held by the
    // torrent object. So, it is generally a good idea to not keep a shared_ptr to
    // your own peer_plugin. If you want to keep references to it, use weak_ptr.
    // NB: All peers are added, while not all are added to _session, see below.
    std::map<libtorrent::tcp::endpoint, boost::weak_ptr<PeerPlugin> > _peers;

    // Protocol session
    // Q: What peers are in session, and what are not.
    // A: All peers with plugins installed will be in this map, however, not all peers
    // will have plugins installed. Plugins are only installed if connection
    // was established when both peer and client side have extension enabled, and
    // even in that case it can be uninstalled later by either side. When starting
    // the session again, the client side will reinvite peer to do extended handshake
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

    /// Sell mode spesific state

    // While selling, this maintains mapping between piece index and peers that are
    // waiting for this piece to be read from disk.
    // Will typically just be one, but may be multiple - hence set is used
    std::map<int, std::set<libtorrent::tcp::endpoint> > _outstandingLoadPieceForBuyerCalls;

    /// Buy mode spesific state

    // While buying, this maintains mapping between piece index and the single
    // peer waiting for it to be validated and stored.
    std::map<int, libtorrent::tcp::endpoint> _outstandingFullPieceArrivedCalls;

    /// Utilities

    // Returns alert manager for torrent, is used to post messages to libtorrent user
    libtorrent::alert_manager & alert_manager() const;

    // Returns raw plugin pointer after asserted locking
    PeerPlugin * peer(const libtorrent::tcp::endpoint &);

    // Returns raw torrent pointer after asserted locking
    libtorrent::torrent * torrent() const;
    //libtorrent::torrent * torrent();

    // Returns torrent piece information based on current state of torrent
    protocol_session::TorrentPieceInformation torrentPieceInformation(const libtorrent::piece_picker &) const;
};

// These should really be private members, but it is not allowed
// to have specialized member functions (C++03, §14.7.3/2).

template <> void TorrentPlugin::process<request::Start>(const request::Start &);
template <> void TorrentPlugin::process<request::Stop>(const request::Stop &);
template <> void TorrentPlugin::process<request::Pause>(const request::Pause &);
template <> void TorrentPlugin::process<request::UpdateBuyerTerms>(const request::UpdateBuyerTerms &);
template <> void TorrentPlugin::process<request::UpdateSellerTerms>(const request::UpdateSellerTerms &);
template <> void TorrentPlugin::process<request::ToObserveMode>(const request::ToObserveMode &);
template <> void TorrentPlugin::process<request::ToSellMode>(const request::ToSellMode &);
template <> void TorrentPlugin::process<request::ToBuyMode>(const request::ToBuyMode &);
//template<> void changeDownloadLocation();

}
}

#endif // JOYSTREAM_EXTENSION_TORRENTPLUGIN_HPP
