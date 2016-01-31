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

#include <QObject>
#include <QMap>
#include <QSet>

class Plugin;
class TorrentPluginConfiguration;
class TorrentPluginAlert;
enum class PluginMode;

namespace joystream {
namespace extension {

    class TorrentPlugin : public libtorrent::torrent_plugin {

    public:

        // Constructor from member fields
        TorrentPlugin(Plugin * plugin,
                      const boost::shared_ptr<libtorrent::torrent> & torrent,
                      const TorrentPluginConfiguration & configuration,
                      QLoggingCategory & category);

        /**
         * Virtual routines
         */

        // Destructor
        virtual ~TorrentPlugin();

        // Libtorrent callbacks
        virtual boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection * connection) = 0;
        virtual void on_piece_pass(int index) = 0;
        virtual void on_piece_failed(int index) = 0;
        virtual void tick() = 0;
        virtual bool on_resume() = 0;
        virtual bool on_pause() = 0;
        virtual void on_files_checked() = 0;
        virtual void on_state(int s) = 0;
        virtual void on_add_peer(const libtorrent::tcp::endpoint & endPoint, int src, int flags) = 0;

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

    protected:

        // Parent plugin for BitSwapr
        // Should this be boost::shared_ptr, since life time of object is managed by it?
        // on the other hand, we loose Plugin behaviour through libtorrent::plugin pointer, which we need!
        Plugin * _plugin;

        // Torrent for this torrent_plugin
        // Should this be weak_ptr really?
        boost::shared_ptr<libtorrent::torrent> _torrent;

        // Set of all endpoints known to not have extension. Is populated by previous failed extended handshakes.
        QSet<libtorrent::tcp::endpoint> _peersWithoutExtension;

        // Set of endpoints banned for irregular conduct during extended protocol
        QSet<libtorrent::tcp::endpoint> _irregularPeer;

        // Logging category
        QLoggingCategory & _category;

        // Torrent info hash
        //libtorrent::sha1_hash _infoHash;

        /**
        // Plugin is active and therefore does tick() processing.
        // Is set by controller after file torrent metadata is acquired and/or
        // resume data has been validated.
        bool _pluginStarted;

        // Tick processor
        void _tick();
        */

        // Checks that peer is not banned and that it is a bittorrent connection
        bool isPeerWellBehaved(libtorrent::peer_connection * connection) const;

        // Send torrent plugin alert to libtorrent session
        void sendTorrentPluginAlert(const TorrentPluginAlert & alert);

    private:

        // Use banning of peers
        bool _enableBanningSets;
    };

}
}

#endif // JOYSTREAM_EXTENSION_TORRENT_PLUGIN_HPP
