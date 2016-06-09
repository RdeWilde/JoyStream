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

    class Plugin;
    class TorrentPluginConfiguration;
    enum class PluginMode;

    namespace alert {
        class TorrentPluginAlert;
    }

    class TorrentPlugin : public libtorrent::torrent_plugin {

    public:

        TorrentPlugin(Plugin * plugin,
                      const boost::shared_ptr<libtorrent::torrent> & torrent,
                      const std::string & bep10ClientIdentifier);

        virtual ~TorrentPlugin();

        // Libtorrent callbacks
        virtual boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection * connection);
        virtual void on_piece_pass(int index);
        virtual void on_piece_failed(int index);
        virtual void tick();
        virtual bool on_resume();
        virtual bool on_pause();
        virtual void on_files_checked();
        virtual void on_state(int s);
        virtual void on_add_peer(const libtorrent::tcp::endpoint & endPoint, int src, int flags);
        
        // Determines the message type, calls correct handler, then frees message
        void processExtendedMessage(joystream::protocol_wire::ExtendedMessagePayload * extendedMessage);

        // Schedules asynchronous read of piece to this peer
        void readPiece(PeerPlugin * peer, int piece);

        // Call back after piece read
        void pieceRead(const libtorrent::read_piece_alert * alert);

        // Adds peer to respective set, and returns whether it was actually added or existed in the set from before.
        void addToPeersWithoutExtensionSet(const libtorrent::tcp::endpoint & endPoint);
        void addToIrregularPeersSet(const libtorrent::tcp::endpoint & endPoint);

    private:

        /**
         * PeerPlugin notifications
         */

        // Adds peer correspoinding to given endpoint to session,
        // is called when peer has sucessfully completed extended handshake.
        // Not when connection is established, as in TorrentPlugin::new_connection
        void addPeerToSession(const libtorrent::tcp::endpoint &);

        // Disocnnects peer, removes corresponding plugin from map
        void disconnectPeer(const libtorrent::tcp::endpoint &);


        //

        /**
         * Protocol session hooks
         */

        // Removes peer plugin corresponding to given endpoint for given reason
        void removeConnection(const libtorrent::tcp::endpoint &, protocol_session::DisconnectCause);

        //
        std::vector<Coin::KeyPair> generateKeyPairsCallbackHandler(int);

        //
        std::vector<Coin::P2PKHAddress> generateP2PKHAddressesCallbackHandler(int);

        //// Buying hooks

        //
        bool broadcastTransaction(const Coin::Transaction &);

        //
        void fullPieceArrived(const libtorrent::tcp::endpoint &, const protocol_wire::PieceData &, int);

        //// Selling hooks

        //
        void loadPieceForBuyer(const libtorrent::tcp::endpoint &, unsigned int);

        //
        void claimLastPayment(const libtorrent::tcp::endpoint &, const joystream::paymentchannel::Payee &);

        // Buyer with given connection id announced anchor
        void anchorAnnounced(const libtorrent::tcp::endpoint &, quint64, const Coin::typesafeOutPoint &, const Coin::PublicKey &, const Coin::PubKeyHash &);

        //// Members

        // Parent plugin
        // Should this be boost::shared_ptr, since life time of object is managed by it?
        // on the other hand, we loose Plugin behaviour through libtorrent::plugin pointer, which we need!
        Plugin * _plugin;

        // Torrent for this torrent_plugin
        // Should this be weak_ptr really?
        boost::shared_ptr<libtorrent::torrent> _torrent;

        // Client identifier used in bep10 handshake v-key
        std::string _bep10ClientIdentifier;

        // Set of all endpoints known to not have extension. Is populated by previous failed extended handshakes.
        std::set<libtorrent::tcp::endpoint> _peersWithoutExtension;

        // Set of endpoints banned for irregular conduct during extended protocol
        std::set<libtorrent::tcp::endpoint> _irregularPeer;

        // Torrent info hash
        libtorrent::sha1_hash _infoHash;

        // Maps endpoint to weak peer plugin pointer, is peer_plugin, since this is
        // the type of weak_ptr libtrrrent requires, hence might as well put it
        // in this type, rather than corresponding subclass of TorrentPlugin.
        // NB: All peers are added, while not all are added to _session, see below.
        std::map<libtorrent::tcp::endpoint, boost::weak_ptr<PeerPlugin> > _peers;

        // Protocol session
        // NB: Only peers which support this extension will be added to session,
        // while all peers are added to _peers
        protocol_session::Session<libtorrent::tcp::endpoint> _session;

        // List of peer plugins scheduled for deletion
        // ** NEEDS TO BE ABSTRACTED TO PARENT CLASS **
        //std::list<boost::weak_ptr<PeerPlugin> > _peersScheduledForDeletion;

        // Maintains mapping between piece index and peers that are waiting for this.
        // Will typically just be one, but may be multiple - hence QSet is used
        std::map<int, std::set<PeerPlugin *> > _outstandingPieceRequests;

        // Checks that peer is not banned and that it is a bittorrent connection
        bool isPeerWellBehaved(libtorrent::peer_connection * connection) const;

        // Send torrent plugin alert to libtorrent session
        void sendTorrentPluginAlert(const alert::TorrentPluginAlert & alert);

        // Use banning of peers
        //bool _enableBanningSets;
    };

}
}

#endif // JOYSTREAM_EXTENSION_TORRENTPLUGIN_HPP
