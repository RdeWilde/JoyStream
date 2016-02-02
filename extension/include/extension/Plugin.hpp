/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_PLUGIN_HPP
#define JOYSTREAM_EXTENSION_PLUGIN_HPP

#include <extension/SellerTorrentPlugin.hpp>
#include <extension/BuyerTorrentPlugin.hpp>

#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/peer.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/lazy_entry.hpp>
#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/aux_/session_impl.hpp>

#include <boost/weak_ptr.hpp>

#include <QMutex>
#include <QQueue>

namespace Wallet {
    class Manager;
}

namespace libtorrent {
    class alert;
    class session_impl;
}

namespace Coin {
    class UnspentP2PKHOutput;
}

namespace joystream {
namespace extension {

    class TorrentPlugin;
    class TorrentPluginConfiguration;

    class BuyerTorrentPlugin;
    class BuyerTorrentPluginConfiguration;

    class SellerTorrentPlugin;
    class SellerTorrentPluginConfiguration;

    namespace request {
        class PluginRequest;
        class TorrentPluginRequest;
        class PeerPluginRequest;
    }

    class Plugin : public libtorrent::plugin {

    public:

        // Constructor
        Plugin(Wallet::Manager * wallet,
               const std::string & bep10ClientIdentifier,
               QLoggingCategory & category);

        // Destructor
        ~Plugin();

        /**
         * All virtual functions below should ONLY
         * be called by libtorrent network thread,
         * never by other threads, as this causes synchronization
         * failures.
         */
        virtual void added(libtorrent::aux::session_impl * session);
        virtual void on_alert(libtorrent::alert const * a);
        virtual void on_tick();
        //virtual bool on_optimistic_unchoke(std::vector<peer_connection_handle> & /* peers */);
        virtual void save_state(libtorrent::entry & stateEntry) const;
        //virtual void load_state(const libtorrent::bdecode_node & state);
        virtual void load_state(libtorrent::lazy_entry const & stateEntry);

        /**
         * LOL ALL OFF THESE ARE FLAWED, OF COURSE
         * IT THE VALUES HAVE TO ACCUMULATE FROM START,
         * NO USE JUST COUNTING TORRENTS ALIVE NOW.
         * MORONIC.
         *

        // Amount of funds (satoshies) received since start
        // across all seller plugins
        quint64 totalReceivedSinceStart() const;

        // Amount of funds (satoshies) sent since start
        quint64 totalSentSinceStart() const;

        // Amount of funds (satoshies) presently locked
        // in channels started during this session.
        // Obviosuly does not include change in channels!
        quint64 totalCurrentlyLockedInChannels() const;
        */

        /**
         * Routines called by libtorrent network thread via tick() entry point on
         * torrent plugins.
         *
         * THESE SHOULD NOT BE PUBLIC, RATHER MAKE THEM PRIVATE AND ADD FRIENDING WITH
         * TORRENT/PEER PLUGIN TYPES
        */

        // Setter routines which update status information
        quint64 registerReceivedFunds(quint64 value);
        quint64 registerSentFunds(quint64 value);
        quint64 registerLockedInChannelsFunds(quint64 value);
        quint64 registerUnLockedFromChannelFunds(quint64 value);


        /**
         * Synchronized routines called from controller by Qt thread.
         *
         * In all of these routines, plugin takes ownership of request object.
         */
        void submitPluginRequest(request::PluginRequest * pluginRequest);
        void submitTorrentPluginRequest(request::TorrentPluginRequest * torrentPluginRequest);
        void submitPeerPluginRequest(request::PeerPluginRequest * peerPluginRequest);

    private:

        // Client identifier used in bep10 handshake v-key
        std::string _bep10ClientIdentifier;

        // Wallet
        Wallet::Manager * _wallet;

        // Libtorrent session.
        // Is set by added() libtorrent hook, not constructor
        libtorrent::aux::session_impl * _session;

        // Maps info hash to pointer to torrent plugin,
        //QMap<libtorrent::sha1_hash, boost::shared_ptr<libtorrent::torrent_plugin> > _plugins;
        //QMap<libtorrent::sha1_hash, boost::shared_ptr<TorrentPlugin> > _plugins;

        QMap<libtorrent::sha1_hash, boost::weak_ptr<BuyerTorrentPlugin> > _buyerPlugins;
        QMap<libtorrent::sha1_hash, boost::weak_ptr<SellerTorrentPlugin> > _sellerPlugins;


        // BitCoind wrapper
        //BitCoindRPC::Client _btcClient;

        // Logging category
        QLoggingCategory & _category;

        // Has this plugin been added to session.
        // Do not use the _session pointer before this.
        bool _addedToSession;

        /**
         * Request processing
         */

        // Plugin Request
        QQueue<request::PluginRequest *> _pluginRequestQueue;
        QMutex _pluginRequestQueueMutex; // mutex protecting queue

        // Torrent Plugin Request
        QQueue<request::TorrentPluginRequest *> _torrentPluginRequestQueue; // queue
        QMutex _torrentPluginRequestQueueMutex; // mutex protecting queue

        // Peer Plugin Request
        QQueue<request::PeerPluginRequest *> _peerPluginRequestQueue; // queue
        QMutex _peerPluginRequestQueueMutex; // mutex protecting queue

        // Processing routines
        void processesRequests();
        void processPluginRequest(const request::PluginRequest * pluginRequest);
        void processTorrentPluginRequest(const request::TorrentPluginRequest * torrentPluginRequest);

        // Removes torrent plugin
        // 1) Remove plugin from torrentPlugins_ map
        // 2) Deletes peer_plugin object
        // 3) Notifies controller
        void removeTorrentPlugin(const libtorrent::sha1_hash & info_hash);

        // Start plugin
        //bool startTorrentPlugin(const libtorrent::sha1_hash & infoHash, const TorrentPlugin::Configuration * configuration);

        bool startBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPluginConfiguration & configuration, const Coin::UnspentP2PKHOutput & utxo);
        bool startSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const SellerTorrentPluginConfiguration & configuration);

        // Send alert to session object
        void sendAlertToSession(const libtorrent::alert & alert);

        /**
         * Status
         */

        // Amount of funds (satoshies) received since start
        quint64 _totalReceivedSinceStart;

        // Amount of funds (satoshies) sent since start
        quint64 _totalSentSinceStart;

        // Amount of funds (satoshies) presently locked
        // in channels started during this session.
        // Obviosuly does not include change in channels!
        quint64 _totalCurrentlyLockedInChannels;

    };

}
}

#endif // JOYSTREAM_EXTENSION_PLUGIN_HPP
