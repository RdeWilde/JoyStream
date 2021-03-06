/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 4 2016
 */

#ifndef JOYSTREAM_CORE_DETAIL_LIBTORRENTALERTPROCESSOR_HPP
#define JOYSTREAM_CORE_DETAIL_LIBTORRENTALERTPROCESSOR_HPP
#include <QTimer>
#include <extension/extension.hpp>
#include <libtorrent/socket.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <memory>

namespace libtorrent {
    class session;
    class sha1_hash;
    class alert;
    struct dht_get_peers_reply_alert;
    struct listen_succeeded_alert;
    struct metadata_received_alert;
    struct metadata_failed_alert;
    struct add_torrent_alert;
    struct torrent_finished_alert;
    struct state_update_alert;
    struct torrent_removed_alert;
    struct torrent_resumed_alert;
    struct save_resume_data_alert;
    struct save_resume_data_failed_alert;
    struct torrent_paused_alert;
    struct torrent_checked_alert;
    struct peer_connect_alert;
    struct peer_disconnected_alert;
    struct read_piece_alert;
    struct piece_finished_alert;
}

namespace joystream {
namespace extension {
    class Plugin;

namespace alert {
    struct RequestResult;
    struct PluginStatus;
}
}

namespace core {
    class Torrent;
    class TorrentPlugin;
    class PeerPlugin;
    class Peer;

namespace detail {

struct NodeImpl {

    /// Callbacks used in response to alerts, are used by IMPL user
    typedef std::function<void(const libtorrent::tcp::endpoint &)> StartedListening;
    typedef std::function<void(core::Torrent * torrent)> AddedTorrent;
    typedef std::function<void(const libtorrent::sha1_hash &)> RemovedTorrent;
    typedef std::function<void(const std::map<libtorrent::sha1_hash, joystream::extension::status::TorrentPlugin>)> TorrentPluginStatusUpdate;
    typedef std::function<void(const libtorrent::alert * )> AlertArrived;

    /// Callbacks provided by user on operations
    typedef std::function<void()> Paused;

    NodeImpl(libtorrent::session * session,
             const boost::shared_ptr<extension::Plugin> & plugin,
             const StartedListening & startedListening,
             const AddedTorrent & addedTorrent,
             const RemovedTorrent & removedTorrent,
             const TorrentPluginStatusUpdate & torrentPluginStatusUpdate,
             const AlertArrived & alertArrived);

    ~NodeImpl();

    /// Service routines

    // Terminates all connections on all torrents, and stops all plugins, but
    // BitTorrent and DHT daemons still run.
    void pause(const Paused & paused);

    // Tries to add torrent
    void addTorrent(const libtorrent::add_torrent_params & params,
                    const extension::request::AddTorrent::AddTorrentHandler & addedTorrent);

    // Tries to remove torrent.
    void removeTorrent(const libtorrent::sha1_hash & info_hash, const extension::request::SubroutineHandler & handler);

    // Updates torrent statuses
    void updateTorrentStatus() const;

    // Updates torrent plugin statuses
    void postTorrentPluginStatusUpdates() const;

    // Port on which BitTorrent daemon listens
    unsigned short port() const;

    /// Alert processing

    // Processe pending libtorrent alert queue
    void processAlertQueue();

    // Processes a spesific alert
    void processAlert(const libtorrent::alert * a);

    // Processing (standard) libtorrent alerts of given type
    void process(const libtorrent::dht_get_peers_reply_alert *);
    void process(const libtorrent::listen_succeeded_alert *);
    void process(const libtorrent::metadata_received_alert *);
    void process(const libtorrent::metadata_failed_alert *);
    void process(const libtorrent::add_torrent_alert *);
    void process(const libtorrent::torrent_finished_alert *);
    void process(const libtorrent::state_update_alert *);
    void process(const libtorrent::torrent_removed_alert *);
    void process(const libtorrent::torrent_resumed_alert *);
    void process(const libtorrent::save_resume_data_alert *);
    void process(const libtorrent::save_resume_data_failed_alert * p);
    void process(const libtorrent::torrent_paused_alert *);
    void process(const libtorrent::torrent_checked_alert *);
    void process(const libtorrent::peer_connect_alert *);
    void process(const libtorrent::peer_disconnected_alert *);
    void process(const libtorrent::read_piece_alert *);
    void process(const libtorrent::piece_finished_alert *);

    // Processing (plugin) alerts
    void process(const extension::alert::RequestResult * p);
    void process(const extension::alert::TorrentPluginStatusUpdateAlert * p);
    void process(const extension::alert::PeerPluginStatusUpdateAlert * p);
    void process(const extension::alert::TorrentPluginAdded * p);
    void process(const extension::alert::TorrentPluginRemoved * p);
    void process(const extension::alert::PeerPluginAdded * p);
    void process(const extension::alert::PeerPluginRemoved * p);
    void process(const extension::alert::ConnectionAddedToSession * p);
    void process(const extension::alert::ConnectionRemovedFromSession * p);
    void process(const extension::alert::SessionStarted * p);
    void process(const extension::alert::SessionPaused * p);
    void process(const extension::alert::SessionStopped * p);
    void process(const extension::alert::SessionToObserveMode * p);
    void process(const extension::alert::SessionToSellMode * p);
    void process(const extension::alert::SessionToBuyMode * p);
    void process(const extension::alert::ValidPaymentReceived * p);
    void process(const extension::alert::InvalidPaymentReceived * p);
    void process(const extension::alert::BuyerTermsUpdated * p);
    void process(const extension::alert::SellerTermsUpdated * p);
    void process(const extension::alert::ContractConstructed * p);
    void process(const extension::alert::SentPayment * p);
    void process(const extension::alert::LastPaymentReceived * p);
    void process(const extension::alert::InvalidPieceArrived * p);
    void process(const extension::alert::ValidPieceArrived * p);

    void process(const extension::alert::AnchorAnnounced * p);

    // Underlying libtorrent session
    std::unique_ptr<libtorrent::session> _session;

    // Plugin
    boost::shared_ptr<extension::Plugin> _plugin;

    // Torrents in session
    std::map<libtorrent::sha1_hash, std::unique_ptr<Torrent>> _torrents;

    // Secondary hash for faster JS peer discovery
    std::map<libtorrent::sha1_hash, libtorrent::sha1_hash> _torrentsBySecondaryHash;
    Torrent *getTorrentBySecondaryHash(const libtorrent::sha1_hash &hash);

    bool _assistedPeerDiscovery;
    QTimer _announceTimer;
    QTimer _getPeersTimer;

    /// Callbacks called in response to inbound alerts
    StartedListening _startedListening;
    AddedTorrent _addedTorrent;
    RemovedTorrent _removedTorrent;
    TorrentPluginStatusUpdate _torrentPluginStatusUpdate;
    AlertArrived _alertArrived;

    void removeTorrent(std::map<libtorrent::sha1_hash, std::unique_ptr<Torrent>>::iterator it);


    /// TEMPORARY
    /// Tries to recover corresponding object

    core::Torrent * getTorrent(const libtorrent::sha1_hash &);
    core::Peer * getPeer(const libtorrent::sha1_hash & infoHash, const libtorrent::tcp::endpoint & ep);
    core::TorrentPlugin * getTorrentPlugin(const libtorrent::sha1_hash & infoHash);
    core::PeerPlugin * getPeerPlugin(const libtorrent::sha1_hash & infoHash, const libtorrent::tcp::endpoint & ep);

public slots:
    // Secondary hash for faster JS peer discovery
    void getPeersAllTorrentsSecondaryHash();
    void announceAllTorrentsSecondaryHash();

};

}
}
}

#endif // JOYSTREAM_CORE_DETAIL_LIBTORRENTALERTPROCESSOR_HPP
