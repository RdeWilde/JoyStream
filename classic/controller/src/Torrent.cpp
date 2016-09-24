/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#include <controller/Torrent.hpp>
#include <controller/Peer.hpp>
#include <core/core.hpp>
#include <gui/gui.hpp>

namespace joystream {
namespace classic {
namespace controller {

Torrent::Torrent(core::Torrent * torrent,
                 ApplicationController * applicationController,
                 gui::TorrentTreeViewRow * torrentTreeViewRow)
    : _torrent(torrent)
    , _applicationController(applicationController)
    , _torrentTreeViewRow(torrentTreeViewRow) {

    // Connect model signals to slots on this object
    QObject::connect(torrent,
                     &core::Torrent::torrentPluginAdded,
                     this,
                     &Torrent::setTorrentPluginPresent);

    QObject::connect(torrent,
                     &core::Torrent::torrentPluginRemoved,
                     this,
                     &Torrent::setTorrentPluginAbsent);

    QObject::connect(torrent,
                     &core::Torrent::stateChanged,
                     this,
                     &Torrent::setState);

    QObject::connect(torrent,
                     &core::Torrent::pausedChanged,
                     this,
                     &Torrent::setPaused);

    QObject::connect(torrent,
                     &core::Torrent::downloadRateChanged,
                     this,
                     &Torrent::setDownloadSpeed);

    QObject::connect(torrent,
                     &core::Torrent::uploadRateChanged,
                     this,
                     &Torrent::setUploadSpeed);
}

Torrent::~Torrent() {
}

void Torrent::addPeer(core::Peer * peer) {

    if(_peers.count(peer->endPoint()) > 0)
        throw std::runtime_error("Peer already added.");

    // Row for peers dialog
    gui::PeerTreeViewRow * row = nullptr;

    // If Peers dialogue is displayed
    if(peersDialogDisplayed()) {

        // Create row
        row = _peersDialog->addPeerTreeViewRow();

        // Set initial values
        row->setHost(peer->endPoint());
        row->setClientName(peer->client());
        row->setBEPSupport(extension::BEPSupportStatus::unknown);
    }

    // Create controller::Peer, and add to map
    _peers[peer->endPoint()] = std::unique_ptr<Peer>(new Peer(peer, row));
}

void Torrent::removePeer(const libtorrent::tcp::endpoint & endPoint) {

   // Get corresponding controoler::Peer
   auto it = _peers.find(endPoint);

   if(it == _peers.cend())
       throw std::runtime_error("Peer not present.");

   std::unique_ptr<Peer> & peer = it->second;

   // If Peers dialuge is displayed, then we need to remove this peer
   if(peersDialogDisplayed()) {

       // Currently, all rows should be added
       assert(peer->peerTreeViewRowSet());

       // Find the row
       gui::PeerTreeViewRow * row = peer->peerTreeViewRow();

       int rowIndex = row->row();

       // Remove from dialog
       /////_peersDialog->removePeer(rowIndex);
   }

   // Remove peer
   _peers.erase(it);
}

void Torrent::pluginModeChanged(protocol_session::SessionMode mode) {

    _torrentTreeViewRow->setSessionMode(mode);

    updateBalance();
}

void Torrent::connectionAdded(const core::Connection * connection)  {

    assert(_torrent->torrentPlugin());

    core::Session * session = _torrent->torrentPlugin()->session();

    // update connectino counts
    // NB: here we could do more intelligent approach by just subtracting balance in this connection
    updateConnectionCounts(session);

    // and update balance when a payment is made/received

    QObject::connect(connection->machine()->payor(),
                     &core::Payor::numberOfPaymentsMadeChanged,
                     this,
                     &Torrent::updateBalance);

    QObject::connect(connection->machine()->payee(),
                     &core::Payee::numberOfPaymentsMadeChanged,
                     this,
                     &Torrent::updateBalance);

}

void Torrent::connectionRemoved(const libtorrent::tcp::endpoint &) {

    // here we could do more intelligent approach by just subtracting balance in this connection
    updateConnectionCounts(getSession());
}


void Torrent::setTorrentPluginPresent(core::TorrentPlugin * model) {

    core::Session * session = model->session();

    // Set session mode
    _torrentTreeViewRow->setSessionMode(session->mode());

    // and update when it is changed
    QObject::connect(session,
                     &core::Session::modeChanged,
                     this,
                     &Torrent::pluginModeChanged);

    // Set initial peer counts
    updateConnectionCounts(session);

    // When a connection is made
    QObject::connect(session,
                     &core::Session::connectionAdded,
                     this,
                     &Torrent::connectionAdded);

    // or removed
    QObject::connect(session,
                     &core::Session::connectionRemoved,
                     this,
                     &Torrent::connectionRemoved);

    // Set initial balance
    updateBalance();
}

void Torrent::setTorrentPluginAbsent() {
    _torrentTreeViewRow->unsetTorrentPluginPresence();
}

void Torrent::updateBalance() {

    // Get session
    core::Session * session = getSession();

    // Recompute new balance
    int64_t balance = getBalance(session);

    // Update view
    _torrentTreeViewRow->setBalance(balance);
}

void Torrent::setState(libtorrent::torrent_status::state_t state, float progress) {
    _torrentTreeViewRow->setState(state, progress);
}

void Torrent::setPaused(bool paused) {
    _torrentTreeViewRow->setPaused(paused);
}

void Torrent::setDownloadSpeed(int speed) {
    _torrentTreeViewRow->setDownloadSpeed(speed);
}

void Torrent::setUploadSpeed(int speed) {
    _torrentTreeViewRow->setUploadSpeed(speed);
}

void Torrent::showPeersDialog() {

    if(peersDialogDisplayed())
        throw std::runtime_error("Already displayed");

    // Create a peers dialog window
    // add all sorts of peers and connections

    /**
    /// Populate models

    // Add all peers
    for(auto mapping : torrent->peers())
        addPeer(mapping.second);

    // Add all peer plugins
    std::shared_ptr<core::TorrentPlugin> plugin = torrent->torrentPlugin();

    if(plugin.get() != NULL) {
        for(auto mapping : plugin->peers())
            addPeerPlugin(mapping.second);
    }

    // Add all connections
    std::shared_ptr<core::Session> session = plugin->session();
    for(auto mapping : session->connections())
        addConnection(mapping.second);
        */
}

void Torrent::hidePeersDialog() {

    if(!peersDialogDisplayed())
        throw std::runtime_error("Not displayed");

    // Go through all peers and unset PeerTreeViewRow

    _peersDialog.release();
}

bool Torrent::peersDialogDisplayed() {
    return _peersDialog.get() != nullptr;
}

gui::TorrentTreeViewRow * Torrent::torrentTreeViewRow() const noexcept {
    return _torrentTreeViewRow.get();
}

void Torrent::setTorrentTreeViewRow(gui::TorrentTreeViewRow * torrentTreeViewRow) {
    _torrentTreeViewRow.reset(torrentTreeViewRow);
}

void Torrent::dropTorrentTreeViewRow() {
    _torrentTreeViewRow.reset();
}

bool Torrent::peerTorrentTreeViewRow() const noexcept {
    return _torrentTreeViewRow.get() != nullptr;
}

int64_t Torrent::getBalance(const core::Session * session) {

    if(session->mode() == protocol_session::SessionMode::buying)
        return getBalanceInBuyingMode(session);
    else if(session->mode() == protocol_session::SessionMode::selling)
        return getBalanceInSellingMode(session);
    else
        return 0;
}

int64_t Torrent::getBalanceInBuyingMode(const core::Session * session) {

    int64_t balance = 0;

    // Does not include contract fees,
    // we need to know state information in state machine
    // to properly account for it.

    // Iterate connections and count towards balance
    for(auto mapping : session->connections()) {

        core::CBStateMachine * machine = mapping.second->machine();

        auto payor = machine->payor();

        balance -= payor->numberOfPaymentsMade() * payor->price();
    }

    return balance;
}

int64_t Torrent::getBalanceInSellingMode(const core::Session * session) {

    int64_t balance = 0;

    // Does not include settlement fees,
    // we need to know state information in state machine
    // to properly account for it.

    // Iterate connections and count towards balance
    for(auto mapping : session->connections()) {

        core::CBStateMachine * machine = mapping.second->machine();

        auto payee = machine->payee();

        balance += payee->numberOfPaymentsMade() * payee->price();
    }

    return balance;
}

core::Session * Torrent::getSession() const {

    if(_torrent == nullptr || _torrent->torrentPlugin() == nullptr)
        throw std::runtime_error("Session not present");
    else
        return _torrent->torrentPlugin()->session();

}


}
}
}
