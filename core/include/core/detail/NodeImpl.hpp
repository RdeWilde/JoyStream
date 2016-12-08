/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 4 2016
 */

#ifndef JOYSTREAM_CORE_DETAIL_LIBTORRENTALERTPROCESSOR_HPP
#define JOYSTREAM_CORE_DETAIL_LIBTORRENTALERTPROCESSOR_HPP

#include <extension/extension.hpp>
#include <libtorrent/socket.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <memory>

namespace libtorrent {
    class session;
    class sha1_hash;
    class alert;
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

namespace detail {

struct NodeImpl {

    /// Callbacks used in response to alerts, are used by IMPL user
    typedef std::function<void(const libtorrent::tcp::endpoint &)> StartedListening;
    typedef std::function<void(core::Torrent * torrent)> AddedTorrent;
    typedef std::function<void(const libtorrent::sha1_hash &)> RemovedTorrent;

    /// Callbacks provided by user on operations
    typedef std::function<void()> Paused;

    NodeImpl(libtorrent::session * session,
             const boost::shared_ptr<extension::Plugin> & plugin,
             const StartedListening & startedListening,
             const AddedTorrent & addedTorrent,
             const RemovedTorrent & removedTorrent);

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

    // Underlying libtorrent session
    std::unique_ptr<libtorrent::session> _session;

    // Plugin
    boost::shared_ptr<extension::Plugin> _plugin;

    // Torrents in session
    std::map<libtorrent::sha1_hash, std::unique_ptr<Torrent>> _torrents;

    /// Callbacks called in response to inbound alerts
    StartedListening _startedListening;
    AddedTorrent _addedTorrent;
    RemovedTorrent _removedTorrent;

    void removeTorrent(std::map<libtorrent::sha1_hash, std::unique_ptr<Torrent>>::iterator it);
};

}
}
}

#endif // JOYSTREAM_CORE_DETAIL_LIBTORRENTALERTPROCESSOR_HPP
