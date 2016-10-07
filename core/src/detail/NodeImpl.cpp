/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 4 2016
 */

#include <core/detail/NodeImpl.hpp>
#include <core/Torrent.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <extension/extension.hpp>
#include <QObject>
#include <boost/asio/ip/detail/endpoint.hpp>

namespace joystream {
namespace core {
namespace detail {

NodeImpl::NodeImpl(libtorrent::session * session,
                   const boost::shared_ptr<extension::Plugin> & plugin,
                   const StartedListening & startedListening,
                   const AddedTorrent & addedTorrent,
                   const RemovedTorrent & removedTorrent)
    : _session(session)
    , _plugin(plugin)
    , _assistedPeerDiscovery(false)
    , _startedListening(startedListening)
    , _addedTorrent(addedTorrent)
    , _removedTorrent(removedTorrent)
{
    _announceTimer.setInterval(2*3600*1000); // Every 2 hours
    _getPeersTimer.setInterval(5*60*1000); // Every 5 minutes

    _announceTimer.connect(&_announceTimer, &QTimer::timeout, [&](){ announceAllTorrentsSecondaryHash(); });
    _getPeersTimer.connect(&_announceTimer, &QTimer::timeout, [&](){ getPeersAllTorrentsSecondaryHash(); });
}

NodeImpl::~NodeImpl() {

    for(auto it = _torrents.begin();it != _torrents.end();)
        removeTorrent(it++);
}

void NodeImpl::pause(const Paused & paused) {

    // Pause libtorrent session
    _plugin->submit(extension::request::PauseLibtorrent([this, paused]() {

        std::clog << "Libtorrent session paused" << std::endl;

        // Stop all plugins
        _plugin->submit(extension::request::StopAllTorrentPlugins([this, paused]() {

            std::clog << "All plugins stopped" << std::endl;

            // Service user callback
            paused();
        }));

    }));

}

void NodeImpl::addTorrent(const libtorrent::add_torrent_params & params,
                          const extension::request::AddTorrent::AddTorrentHandler & addedTorrent) {
    _plugin->submit(extension::request::AddTorrent(params, addedTorrent));
}

void NodeImpl::removeTorrent(const libtorrent::sha1_hash & info_hash, const extension::request::SubroutineHandler & handler) {
    _plugin->submit(extension::request::RemoveTorrent(info_hash, handler));
}

void NodeImpl::updateTorrentStatus() const {

    // Regular torrent level state update
    _session->post_torrent_updates();
}

void NodeImpl::updateTorrentPluginStatus() const {
    _plugin->submit(extension::request::UpdateStatus());
}

void NodeImpl::updatePeerStatus() const {

    for(auto & mapping : _torrents) {
        // Get handle for torrent
        //libtorrent::sha1_hash infoHash = mapping.second->infoHash();
        //libtorrent::torrent_handle h = _session->find_torrent(infoHash);

        // Get peer_info for peer, which unfortunately requires
        // getting it for all peers
        std::vector<libtorrent::peer_info> v;

        try {
            mapping.second->_handle.get_peer_info(v);
        } catch (const libtorrent::libtorrent_exception &) {
            // Handle was invalidated, drop torrent,
            // torrent-removed_alert will come in due time.
            continue;
        }

        // Update peer statuses on torrent
        mapping.second->updatePeerStatuses(v);
    }

}

unsigned short NodeImpl::port() const{
    return _session->listen_port();
}

void NodeImpl::processAlertQueue() {

    // Populate vector with alerts
    std::vector<libtorrent::alert *> alerts;
    _session->pop_alerts(&alerts);

    // NOTE on owernship of alerts (alert.hpp)
    // Alerts returned by pop_alerts() are only valid until the next call to
    // pop_alerts(). You may not copy an alert object to access it after the next
    // call to pop_alerts(). Internal members of alerts also become invalid once
    // pop_alerts() is called again.

    // Process alerts in queue
    for(libtorrent::alert * alert : alerts)
        processAlert(alert);

}

void NodeImpl::processAlert(const libtorrent::alert * a) {

    std::clog << "Processing alert " << a->message() << std::endl;

    // Select alert type
    if(libtorrent::metadata_received_alert const * p = libtorrent::alert_cast<libtorrent::metadata_received_alert>(a))
        process(p);
    else if(libtorrent::metadata_failed_alert const * p = libtorrent::alert_cast<libtorrent::metadata_failed_alert>(a))
        process(p);
    else if(libtorrent::listen_succeeded_alert const * p = libtorrent::alert_cast<libtorrent::listen_succeeded_alert>(a))
        process(p);
    else if(libtorrent::add_torrent_alert const * p = libtorrent::alert_cast<libtorrent::add_torrent_alert>(a))
        process(p);
    else if (libtorrent::torrent_finished_alert const * p = libtorrent::alert_cast<libtorrent::torrent_finished_alert>(a))
        process(p);
    else if (libtorrent::torrent_paused_alert const * p = libtorrent::alert_cast<libtorrent::torrent_paused_alert>(a))
        process(p);
    else if (libtorrent::state_update_alert const * p = libtorrent::alert_cast<libtorrent::state_update_alert>(a))
        process(p);
    else if(libtorrent::save_resume_data_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_alert>(a))
        process(p);
    else if(libtorrent::save_resume_data_failed_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_failed_alert>(a))
        process(p);
    else if(libtorrent::torrent_removed_alert const * p = libtorrent::alert_cast<libtorrent::torrent_removed_alert>(a))
        process(p);
    else if(libtorrent::torrent_checked_alert const * p = libtorrent::alert_cast<libtorrent::torrent_checked_alert>(a))
        process(p);
    //else if(libtorrent::read_piece_alert const * p = libtorrent::alert_cast<libtorrent::read_piece_alert>(a))
    //    processReadPieceAlert(p);
    //else if(libtorrent::piece_finished_alert const * p = libtorrent::alert_cast<libtorrent::piece_finished_alert>(a))
    //    processPieceFinishedAlert(p);
    else if(extension::alert::RequestResult const * p = libtorrent::alert_cast<extension::alert::RequestResult>(a))
        process(p);
    else if(extension::alert::PluginStatus const * p = libtorrent::alert_cast<extension::alert::PluginStatus>(a))
        process(p);
    else
        std::clog << "Ignored alert, not processed." << std::endl;

}

void NodeImpl::process(const libtorrent::dht_get_peers_reply_alert *p) {
    if(!getTorrentBySecondaryHash(p->info_hash)) return; // Didn't find the torrent corresponding to this hash
    Torrent &t = *getTorrentBySecondaryHash(p->info_hash);

    std::vector<libtorrent::tcp::endpoint> peers;
    p->peers(peers); // Fill vector with new peers

    for(const libtorrent::tcp::endpoint &ep : peers) {
        t.handle().connect_peer(ep);
    }
}

void NodeImpl::process(const libtorrent::listen_succeeded_alert * p) {
    _startedListening(p->endpoint);
}

/**
int LibtorrentAlertProcessor::requestResumeData() {

    // Keeps track of how many calls were made
    int resumeCallsMade = 0;

    for(auto mapping : _torrents) {

        // Grab torrent;
        detail::Torrent & t = mapping.second;

        // Skip torrents no yet added
        if (t.state == detail::Torrent::State::being_added)
            continue;

        // Get handle
        libtorrent::torrent_handle h = _session->find_torrent(mapping.first);

        // Dont save data if
        if (!h.is_valid() || // dont have valid handle
            !h.need_save_resume_data() || // dont need to
            !h.status().has_metadata) // or dont have metadata
            continue;

        // Save resume data
        h.save_resume_data();

        // Count call
        resumeCallsMade++;
    }

    return resumeCallsMade;
}
*/

void NodeImpl::process(const libtorrent::torrent_paused_alert * p) {

    // Get info hash
    libtorrent::sha1_hash infoHash = p->handle.info_hash();

    // Did we get a valid info hash
    if(!infoHash.is_all_zeros()) {

        // then mark as paused
        auto it = _torrents.find(infoHash);
        assert(it != _torrents.cend());

        it->second->updatePaused(true);
    }
}

void NodeImpl::process(const libtorrent::torrent_removed_alert * p) {

    /*
     * NOTICE: Docs say p->handle may be invalid at this time - likely because this is a removal operation,
     * so we must use p->info_hash instead.
     */

    // Then erase from container
    auto it = _torrents.find(p->info_hash);
    assert(it != _torrents.cend());

    removeTorrent(it);
}

void NodeImpl::process(const libtorrent::torrent_resumed_alert * p) {

    // Get info hash
    libtorrent::sha1_hash infoHash = p->handle.info_hash();

    // Did we get a valid info hash
    if(!infoHash.is_all_zeros()) {

        auto it = _torrents.find(infoHash);
        assert(it != _torrents.cend());

        // then mark as resmed
        it->second->updatePaused(false);
    }

}

void NodeImpl::process(const libtorrent::metadata_received_alert * p) {

    // Syncronous call which returns nullptr if call failes
    libtorrent::torrent_handle h = p->handle;
    boost::shared_ptr<const libtorrent::torrent_info> torrent_info = h.torrent_file();
    libtorrent::sha1_hash infoHash = h.info_hash();

    // If handle and info_hahs is still valid
    if(torrent_info && !infoHash.is_all_zeros()) {

        auto it = _torrents.find(infoHash);

        assert(it != _torrents.cend());

        // then set metadata
        it->second->setMetadata(torrent_info);

    } else
        std::clog << "Invalid handle for received metadata." << std::endl;
}

void NodeImpl::process(const libtorrent::metadata_failed_alert *) {

    // what to do?
}

void NodeImpl::process(const libtorrent::add_torrent_alert * p) {

    // Did adding succeed?
    if(!p->error) {

        // Get torrent info_hash
        libtorrent::torrent_handle h = p->handle;
        libtorrent::sha1_hash infoHash = h.info_hash();

        if(infoHash.is_all_zeros()) {
            std::clog << "Added torrent has already expired." << std::endl;
            return;
        }

        // This should always hold, as the latter of two outstanding
        // add_torrent_async call will have its add_torrent_alert::error value set.
        assert(_torrents.count(infoHash) == 0);

        // Get current status
        libtorrent::torrent_status status;

        try {
            status = h.status();
        } catch (const libtorrent::libtorrent_exception &) {
            std::clog << "Handle has already expired." << std::endl;
        }

        int uploadLimit = h.upload_limit();
        int downloadLimit = h.download_limit();

        // Create torrent
        auto t = new Torrent(h, status, p->params.resume_data, uploadLimit, downloadLimit, _plugin);

        // add to map
        _torrents.insert(std::make_pair(infoHash, std::unique_ptr<Torrent>(t)));

        // send notification signal
        _addedTorrent(t);

        std::clog << "Adding torrent succeeded." << std::endl;
    } else
        std::clog << "Adding torrent failed:" << p->error.message() << std::endl;
}

void NodeImpl::process(const libtorrent::torrent_finished_alert *) {

    /// nothing to do?
}

void NodeImpl::process(const libtorrent::state_update_alert * p) {

    for(auto s : p->status) {

        // Try to get torrent refence
        auto it = _torrents.find(s.info_hash);

        // and update status if possible
        if(it != _torrents.cend())
            it->second->updateStatus(s);
        else
            std::clog << "Uknown torrent updated status." << std::endl;
    }

}

void NodeImpl::process(const libtorrent::save_resume_data_alert * p) {

    /// No guard required

    // Recover info_hash of torrent
    libtorrent::torrent_handle h = p->handle;

    libtorrent::sha1_hash infoHash = h.info_hash();

    if(infoHash.is_all_zeros()) {
        std::clog << "Handle already expired." << std::endl;
        return;
    }

    // Get reference ot corresponding torrent
    auto it = _torrents.find(h.info_hash());

    if(it == _torrents.cend()) {
        std::clog << "Dropped alert, no correspondign torrent found." << std::endl;
        return;
    }

    // Create resume data buffer
    std::vector<char> resumeData;

    // Write new content to it
    libtorrent::bencode(std::back_inserter(resumeData), *(p->resume_data));

    // Save resume data in torrent
    it->second->setResumeDataGenerationResult(resumeData);
}

void NodeImpl::process(const libtorrent::save_resume_data_failed_alert * p) {

    /// No guard required

    // Recover info_hash of torrent
    libtorrent::torrent_handle h = p->handle;

    libtorrent::sha1_hash infoHash = h.info_hash();

    if(infoHash.is_all_zeros()) {
        std::clog << "Handle already expired." << std::endl;
        return;
    }

    // Get reference ot corresponding torrent
    auto it = _torrents.find(h.info_hash());

    if(it == _torrents.cend()) {
        std::clog << "Dropped alert, no correspondign torrent found." << std::endl;
        return;
    }

    // Save empty resume data in torrent, is required
    // to trigger event signal
    it->second->setResumeDataGenerationResult(std::vector<char>());
}

void NodeImpl::process(const libtorrent::torrent_checked_alert *) {

    /// nothing to do
}

void NodeImpl::process(const libtorrent::peer_connect_alert * p) {

    // Get peer_info for peer, which unfortunately requires
    // getting it for all peers, and as such, we try to update
    // the all statuses.
    std::vector<libtorrent::peer_info> v;

    try {
        p->handle.get_peer_info(v);
    } catch (const libtorrent::libtorrent_exception &) {
        // Handle was invalidated, drop alert
        return;
    }

    // Get info_hash, drop alert if the handle gave us invalid info hash
    libtorrent::sha1_hash infoHash = p->handle.info_hash();

    if(infoHash.is_all_zeros())
        return;

    // Find torrent
    auto it = _torrents.find(infoHash);

    // If its not registerd, then we ignore alert
    if(it == _torrents.cend())
        return;

    // Update peer statuses on torrent
    it->second->updatePeerStatuses(v);
}

void NodeImpl::process(const libtorrent::peer_disconnected_alert * p) {

    // Get info_hash, drop alert if the handle gave us invalid info hash
    libtorrent::sha1_hash infoHash = p->handle.info_hash();

    if(infoHash.is_all_zeros())
        return;

    // Find torrent
    auto it = _torrents.find(infoHash);

    // If its not registerd, then we ignore alert
    if(it == _torrents.cend())
        return;

    // (try to) remove peer from torrent
    it->second->removePeer(p->ip);
}

void NodeImpl::process(const libtorrent::read_piece_alert *) {

//    // Get info hash for torrent from which this read piece comes from
//    const libtorrent::sha1_hash infoHash = p->handle.info_hash();

//    Q_ASSERT(_torrents.contains(infoHash));

//    /**
//    // Notify torrent view model
//    _torrents[infoHash]->model()->pieceRead(p->ec,
//                                            p->buffer,
//                                            p->piece,
//                                            p->size);
//    */

//    if(p->ec) {
//        std::clog << "There was some sort of error in reading a piece: " << QString::fromStdString(p->ec.message())  << std::endl;
//    } else {

//        // Notify torrent
//        _torrents[infoHash]->pieceRead(p->buffer, p->piece, p->size);
//    }
}

void NodeImpl::process(const libtorrent::piece_finished_alert *) {

//    // Get info hash for torrent from which this read piece comes from
//    const libtorrent::sha1_hash infoHash = p->handle.info_hash();

//    Q_ASSERT(_torrents.contains(infoHash));

//    // Notify torrent
//    _torrents[infoHash]->pieceFinished(p->piece_index);
}

void NodeImpl::process(const extension::alert::RequestResult * p) {

    /// No guard here, we allow user code to run

    // Make loaded callback
    p->loadedCallback();
}

void NodeImpl::process(const extension::alert::PluginStatus * p) {

    // Update torrent plugin statuses
    for(auto status: p->status.plugins) {

        // Get torrent for this plugin
        auto it = _torrents.find(status.first);

        if(it != _torrents.cend()) {

            std::unique_ptr<Torrent> & t = it->second;

            if(t->torrentPluginSet())
                t->updateTorrentPluginStatus(status.second);
            else
                t->addTorrentPlugin(status.second);

            // NB: we could keep track of missing plugin statuses also, but
            // introducing explicit alerts for these events
        }
    }

    // Do other stuff when plugin status is extended
}

Torrent *NodeImpl::getTorrentBySecondaryHash(const libtorrent::sha1_hash &hash)
{
    if(_torrentsBySecondaryHash.count(hash) == 0) return nullptr;
    const libtorrent::sha1_hash &primaryHash = _torrentsBySecondaryHash[hash];
    Q_ASSERT(_torrents.count(primaryHash)>0);

    return _torrents[primaryHash].get();
}

void NodeImpl::announceAllTorrentsSecondaryHash()
{
    for(auto &pair : _torrents) {
        Torrent &t = *pair.second;
        _session->dht_announce(t.secondaryInfoHash(), _session->listen_port());
    }
}

void NodeImpl::getPeersAllTorrentsSecondaryHash()
{
    for(auto &pair : _torrents) {
        Torrent &t = *pair.second;
        _session->dht_get_peers(t.secondaryInfoHash());
    }
}


void NodeImpl::removeTorrent(std::map<libtorrent::sha1_hash, std::unique_ptr<Torrent>>::iterator it) {

    libtorrent::sha1_hash info_hash = it->first;

    _torrents.erase(it);

    _removedTorrent(info_hash);
}

}
}
}
