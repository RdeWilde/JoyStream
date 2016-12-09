/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 4 2016
 */

#include <core/detail/NodeImpl.hpp>
#include <core/Torrent.hpp>
#include <core/TorrentPlugin.hpp>
#include <core/Peer.hpp>
#include <core/PeerPlugin.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <extension/extension.hpp>

namespace joystream {
namespace core {
namespace detail {

NodeImpl::NodeImpl(libtorrent::session * session,
                   const boost::shared_ptr<extension::Plugin> & plugin,
                   const StartedListening & startedListening,
                   const AddedTorrent & addedTorrent,
                   const RemovedTorrent & removedTorrent,
                   const TorrentPluginStatusUpdate & torrentPluginStatusUpdate,
                   const AlertArrived & alertArrived)
    : _session(session)
    , _plugin(plugin)
    , _startedListening(startedListening)
    , _addedTorrent(addedTorrent)
    , _removedTorrent(removedTorrent)
    , _torrentPluginStatusUpdate(torrentPluginStatusUpdate)
    , _alertArrived(alertArrived) {
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

void NodeImpl::postTorrentPluginStatusUpdates() const {
    _plugin->submit(extension::request::PostTorrentPluginStatusUpdates());
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
    else if(extension::alert::TorrentPluginStatusUpdateAlert const * p = libtorrent::alert_cast<extension::alert::TorrentPluginStatusUpdateAlert>(a))
        process(p);
    else if(extension::alert::PeerPluginStatusUpdateAlert const * p = libtorrent::alert_cast<extension::alert::PeerPluginStatusUpdateAlert>(a))
        process(p);
    else if(extension::alert::TorrentPluginAdded const * p = libtorrent::alert_cast<extension::alert::TorrentPluginAdded>(a))
        process(p);
    else if(extension::alert::TorrentPluginRemoved const * p = libtorrent::alert_cast<extension::alert::TorrentPluginRemoved>(a))
        process(p);
    else if(extension::alert::PeerPluginAdded const * p = libtorrent::alert_cast<extension::alert::PeerPluginAdded>(a))
        process(p);
    else if(extension::alert::PeerPluginRemoved const * p = libtorrent::alert_cast<extension::alert::PeerPluginRemoved>(a))
        process(p);
    else if(extension::alert::ConnectionAddedToSession const * p = libtorrent::alert_cast<extension::alert::ConnectionAddedToSession>(a))
        process(p);
    else if(extension::alert::ConnectionRemovedFromSession const * p = libtorrent::alert_cast<extension::alert::ConnectionRemovedFromSession>(a))
        process(p);
    else if(extension::alert::SessionStarted const * p = libtorrent::alert_cast<extension::alert::SessionStarted>(a))
        process(p);
    else if(extension::alert::SessionPaused const * p = libtorrent::alert_cast<extension::alert::SessionPaused>(a))
        process(p);
    else if(extension::alert::SessionStopped const * p = libtorrent::alert_cast<extension::alert::SessionStopped>(a))
        process(p);
    else if(extension::alert::SessionToObserveMode const * p = libtorrent::alert_cast<extension::alert::SessionToObserveMode>(a))
        process(p);
    else if(extension::alert::SessionToSellMode const * p = libtorrent::alert_cast<extension::alert::SessionToSellMode>(a))
        process(p);
    else if(extension::alert::SessionToBuyMode const * p = libtorrent::alert_cast<extension::alert::SessionToBuyMode>(a))
        process(p);
    else if(extension::alert::ValidPaymentReceived const * p = libtorrent::alert_cast<extension::alert::ValidPaymentReceived>(a))
        process(p);
    else if(extension::alert::InvalidPaymentReceived const * p = libtorrent::alert_cast<extension::alert::InvalidPaymentReceived>(a))
        process(p);
    else if(extension::alert::BuyerTermsUpdated const * p = libtorrent::alert_cast<extension::alert::BuyerTermsUpdated>(a))
        process(p);
    else if(extension::alert::SellerTermsUpdated const * p = libtorrent::alert_cast<extension::alert::SellerTermsUpdated>(a))
        process(p);
    else if(extension::alert::ContractConstructed const * p = libtorrent::alert_cast<extension::alert::ContractConstructed>(a))
        process(p);
    else if(extension::alert::SentPayment const * p = libtorrent::alert_cast<extension::alert::SentPayment>(a))
        process(p);
    else if(extension::alert::LastPaymentReceived const * p = libtorrent::alert_cast<extension::alert::LastPaymentReceived>(a))
        process(p);
    else
        std::clog << "Ignored alert, not processed." << std::endl;

    _alertArrived(a);
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

        emit (it->second)->pausedChanged(true);
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

        // send signal
        emit it->second->pausedChanged(false);
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
        emit (it->second)->metadataReceived(torrent_info);

    } else
        std::clog << "Invalid handle for received metadata." << std::endl;
}

void NodeImpl::process(const libtorrent::metadata_failed_alert *) {

    // what to do?
}

void NodeImpl::process(const libtorrent::add_torrent_alert * p) {

    // Did adding succeed?
    if(!p->error) {

        libtorrent::sha1_hash infoHash = p->handle.info_hash();

        core::Torrent * torrent = getTorrent(p->handle.info_hash());

        if(torrent == nullptr) {

            // Create torrent
            torrent = new Torrent(p->handle, p->params.resume_data, _plugin);

            // add to map
            _torrents.insert(std::make_pair(infoHash, std::unique_ptr<Torrent>(torrent)));

            // send notification signal
            _addedTorrent(torrent);

        } else
            torrent->_resumeData = p->params.resume_data;

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
            emit (it->second)->stateChanged(s.state, s.progress);
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

    for(auto ep : v) {

        // Should only happen once
        if(ep.ip == p->ip)
            (it->second)->addPeer(ep);
    }
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
    (it->second)->removePeer(p->ip);
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

void NodeImpl::process(const extension::alert::TorrentPluginStatusUpdateAlert * p) {

    // Update torrent plugin statuses
    for(auto m: p->statuses) {

        if(core::TorrentPlugin * plugin = getTorrentPlugin(m.first))
            plugin->update(m.second);
    }

    _torrentPluginStatusUpdate(p->statuses);
}

void NodeImpl::process(const extension::alert::PeerPluginStatusUpdateAlert * p) {

    core::TorrentPlugin * plugin = getTorrentPlugin(p->handle.info_hash());

    if(plugin == nullptr)
        return;

    // Broadcast alert from torrent plugin
    emit plugin->updatePeerPluginStatuses(p->statuses);

    // Update status for
    for(auto m : p->statuses) {

        if(PeerPlugin * peerPlugin = getPeerPlugin(plugin->infoHash(), m.first))
            emit peerPlugin->update(m.second);
    }
}

void NodeImpl::process(const extension::alert::TorrentPluginAdded * p) {

    /// Add the corresponding torrent.
    // We ignore the libtorrent::add_torrent_alert.

    libtorrent::sha1_hash infoHash = p->handle.info_hash();

    assert(getTorrent(infoHash) == nullptr);

    // Create torrent
    auto t = new Torrent(p->handle, std::vector<char>(), _plugin);

    // add to map
    _torrents.insert(std::make_pair(infoHash, std::unique_ptr<Torrent>(t)));

    // send notification signal
    _addedTorrent(t);

    /// Add torrent plugin
    t->addTorrentPlugin(p->status);
}

void NodeImpl::process(const extension::alert::TorrentPluginRemoved * p) {

    libtorrent::sha1_hash infoHash = p->handle.info_hash();

    /// We remain agnostic

    if(core::Torrent * t = getTorrent(infoHash)) {

        if(t->torrentPluginSet())
            t->removeTorrentPlugin();
    }

}

void NodeImpl::process(const extension::alert::PeerPluginAdded * p) {

    if(core::Peer * peer = getPeer(p->handle.info_hash(), p->ip)) {

        assert(!peer->peerPluginSet());

        peer->addPeerPlugin(p->status);

    }

}

void NodeImpl::process(const extension::alert::PeerPluginRemoved * p) {

    if(core::Peer * peer = getPeer(p->handle.info_hash(), p->ip)) {

        assert(peer->peerPluginSet());

        peer->removePeerPlugin();
    }

}

void NodeImpl::process(const extension::alert::ConnectionAddedToSession * p) {

    if(core::PeerPlugin * plugin = getPeerPlugin(p->handle.info_hash(), p->ip))
        emit plugin->connectionAdded(p->status);
}

void NodeImpl::process(const extension::alert::ConnectionRemovedFromSession * p) {

    if(core::PeerPlugin * plugin = getPeerPlugin(p->handle.info_hash(), p->ip))
        emit plugin->connectionRemoved();
}

void NodeImpl::process(const extension::alert::SessionStarted * p) {

    if(core::TorrentPlugin * plugin = getTorrentPlugin(p->handle.info_hash()))
        emit plugin->sessionStarted();
}

void NodeImpl::process(const extension::alert::SessionPaused * p) {

    if(core::TorrentPlugin * plugin = getTorrentPlugin(p->handle.info_hash()))
        emit plugin->sessionPaused();
}

void NodeImpl::process(const extension::alert::SessionStopped * p) {

    if(core::TorrentPlugin * plugin = getTorrentPlugin(p->handle.info_hash()))
        emit plugin->sessionStopped();
}

void NodeImpl::process(const extension::alert::SessionToObserveMode * p) {

    if(core::TorrentPlugin * plugin = getTorrentPlugin(p->handle.info_hash()))
        emit plugin->sessionToObserveMode();

}

void NodeImpl::process(const extension::alert::SessionToSellMode * p) {

    if(core::TorrentPlugin * plugin = getTorrentPlugin(p->handle.info_hash()))
        emit plugin->sessionToSellMode(p);

}

void NodeImpl::process(const extension::alert::SessionToBuyMode * p) {

    if(core::TorrentPlugin * plugin = getTorrentPlugin(p->handle.info_hash()))
        emit plugin->sessionToBuyMode(p);

}

void NodeImpl::process(const extension::alert::ValidPaymentReceived * p) {

    if(core::TorrentPlugin * plugin = getTorrentPlugin(p->handle.info_hash()))
        emit plugin->validPaymentReceived(p);

}

void NodeImpl::process(const extension::alert::InvalidPaymentReceived * p) {

    if(core::TorrentPlugin * plugin = getTorrentPlugin(p->handle.info_hash()))
        emit plugin->invalidPaymentReceived(p);
}

void NodeImpl::process(const extension::alert::BuyerTermsUpdated * p) {

    if(core::TorrentPlugin * plugin = getTorrentPlugin(p->handle.info_hash()))
        emit plugin->buyerTermsUpdated(p);
}

void NodeImpl::process(const extension::alert::SellerTermsUpdated * p) {

    if(core::TorrentPlugin * plugin = getTorrentPlugin(p->handle.info_hash()))
        emit plugin->sellerTermsUpdated(p);
}

void NodeImpl::process(const extension::alert::ContractConstructed * p) {

    if(core::TorrentPlugin * plugin = getTorrentPlugin(p->handle.info_hash()))
        emit plugin->contractConstructed(p);
}

void NodeImpl::process(const extension::alert::SentPayment * p) {

    if(core::TorrentPlugin * plugin = getTorrentPlugin(p->handle.info_hash()))
        emit plugin->sentPayment(p);
}

void NodeImpl::process(const extension::alert::LastPaymentReceived * p) {

    if(core::TorrentPlugin * plugin = getTorrentPlugin(p->handle.info_hash()))
        emit plugin->lastPaymentReceived(p);
}

void NodeImpl::process(const extension::alert::AnchorAnnounced * p) {

    if(core::TorrentPlugin * plugin = getTorrentPlugin(p->handle.info_hash()))
        emit plugin->anchorAnnounced(p);
}


void NodeImpl::removeTorrent(std::map<libtorrent::sha1_hash, std::unique_ptr<Torrent>>::iterator it) {

    libtorrent::sha1_hash info_hash = it->first;

    _torrents.erase(it);

    _removedTorrent(info_hash);
}

core::Torrent * NodeImpl::getTorrent(const libtorrent::sha1_hash & infoHash) {

    auto it = _torrents.find(infoHash);

    if(it == _torrents.cend())
        return nullptr;
    else
        return (it->second).get();

}

core::Peer * NodeImpl::getPeer(const libtorrent::sha1_hash & infoHash, const libtorrent::tcp::endpoint & ep) {

    core::Torrent * torrent = getTorrent(infoHash);

    if(torrent == nullptr)
        return nullptr;
    else {

        auto it = torrent->_peers.find(ep);

        if(it == torrent->_peers.cend())
            return nullptr;
        else
            return(it->second).get();
    }

}

core::TorrentPlugin * NodeImpl::getTorrentPlugin(const libtorrent::sha1_hash & infoHash) {

    core::Torrent * torrent = getTorrent(infoHash);

    if(torrent == nullptr)
        return nullptr;
    else {

        if(torrent->torrentPluginSet())
            return torrent->torrentPlugin();
        else
            return nullptr;
    }
}

core::PeerPlugin * NodeImpl::getPeerPlugin(const libtorrent::sha1_hash & infoHash, const libtorrent::tcp::endpoint & ep) {

    core::Peer * peer = getPeer(infoHash, ep);

    if(peer == nullptr)
        return nullptr;
    else {

        if(peer->peerPluginSet())
            return peer->peerPlugin();
        else
            return nullptr;
    }
}

}
}
}
