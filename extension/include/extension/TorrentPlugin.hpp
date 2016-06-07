/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_TORRENT_PLUGIN_HPP
#define JOYSTREAM_EXTENSION_TORRENT_PLUGIN_HPP

// For QSet: uint qHash(const libtorrent::tcp::endpoint & endpoint)
#include <common/LibtorrentUtilities.hpp>

#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>

#include <map>

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

        // Constructor from member fields
        TorrentPlugin(Plugin * plugin,
                      const boost::shared_ptr<libtorrent::torrent> & torrent,
                      const std::string & bep10ClientIdentifier,
                      const TorrentPluginConfiguration & configuration);

        /**
         * Virtual routines
         */

        // Destructor
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

        // Schedules asynchronous read of piece to this peer
        void readPiece(SellerPeerPlugin * peer, int piece);

        // Call back after piece read
        void pieceRead(const libtorrent::read_piece_alert * alert);

        /**
         * Routines called by libtorrent network thread from other plugin objects
         */

        // Adds peer to respective set, and returns whether it was actually added or existed in the set from before.
        void addToPeersWithoutExtensionSet(const libtorrent::tcp::endpoint & endPoint);
        void addToIrregularPeersSet(const libtorrent::tcp::endpoint & endPoint);

        virtual PluginMode pluginMode() const = 0;
        virtual QList<libtorrent::tcp::endpoint> endPoints() const = 0;

        boost::shared_ptr<libtorrent::torrent> torrent() const;
        void setTorrent(const boost::shared_ptr<libtorrent::torrent> & torrent);

        bool enableBanningSets() const;
        void setEnableBanningSets(bool enableBanningSets);

    private:

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
        std::map<libtorrent::tcp::endpoint, boost::weak_ptr<PeerPlugin> > _peers;

        // List of peer plugins scheduled for deletion
        // ** NEEDS TO BE ABSTRACTED TO PARENT CLASS **
        std::list<boost::weak_ptr<SellerPeerPlugin> > _peersScheduledForDeletion;

        // Maintains mapping between piece index and peers that are waiting for this.
        // Will typically just be one, but may be multiple - hence QSet is used
        std::map<int, QSet<SellerPeerPlugin *> > _outstandingPieceRequests;

        // Checks that peer is not banned and that it is a bittorrent connection
        bool isPeerWellBehaved(libtorrent::peer_connection * connection) const;

        // Send torrent plugin alert to libtorrent session
        void sendTorrentPluginAlert(const alert::TorrentPluginAlert & alert);

        // Use banning of peers
        bool _enableBanningSets;
    };

}
}

#endif // JOYSTREAM_EXTENSION_TORRENT_PLUGIN_HPP
