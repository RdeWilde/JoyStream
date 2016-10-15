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
                 gui::TorrentTableModel * torrentTableModel,
                 const BitcoinDisplaySettings * settings)
    : _torrent(torrent)
    , _numberOfBuyers(0)
    , _numberOfSellers(0)
    , _numberOfObservers(0)
    , _numberOfUnannounced(0)
    , _balance(0)
    , _torrentTableModel(torrentTableModel)
    , _torrentRowModel(nullptr)
    , _classicPeerTableModel(settings)
    , _buyerTableModel(settings)
    , _observerTableModel(settings)
    , _sellerTableModel(settings)
    , _sellersTableModel(settings)
    , _buyersTableModel(settings) {

    setName(_torrent->name());

    if(auto metadata = _torrent->metaData().lock())
        setSize(metadata->total_size());

    setState(_torrent->state(), _torrent->progress());
    setPaused(_torrent->isPaused());
    setDownloadSpeed(_torrent->downloadRate());
    setUploadSpeed(_torrent->uploadRate());

    for(auto mapping : _torrent->peers())
        addPeer(mapping.second);

    if(torrent->torrentPluginSet())
        setTorrentPluginPresent(torrent->torrentPlugin());

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

    // If visible in the main window, the remove from the corresponding
    // entry in the view model
    if(visibleInMainWindow())
        _torrentTableModel->remove(_torrentRowModel->row());
}

void Torrent::showInMainWindow(bool show) {

    if(show) {

        if(visibleInMainWindow())
            throw std::runtime_error("Cannot show torrent in main window: is already showing");
        else
            _torrentRowModel = _torrentTableModel->add(_torrent->infoHash());

    } else {

        if(!visibleInMainWindow())
            throw std::runtime_error("Cannot hide torrent in main window: is already not showing");
        else {
            _torrentTableModel->remove(_torrentRowModel->row());
            _torrentRowModel.release();
        }
    }
}

bool Torrent::visibleInMainWindow() const noexcept {
    return _torrentRowModel.get() != nullptr;
}

void Torrent::setName(const std::string & name) {

    if(visibleInMainWindow())
        _torrentRowModel->setName(name);
}

void Torrent::setSize(boost::int64_t totalSize) {

    if(visibleInMainWindow())
        _torrentRowModel->setSize(totalSize);
}

void Torrent::setState(libtorrent::torrent_status::state_t state, float progress) {

    if(visibleInMainWindow())
        _torrentRowModel->setState(state, progress);
}

void Torrent::setPaused(bool paused) {

    if(visibleInMainWindow())
        _torrentRowModel->setPaused(paused);
}

void Torrent::setDownloadSpeed(int speed) {

    if(visibleInMainWindow())
        _torrentRowModel->setDownloadSpeed(speed);
}

void Torrent::setUploadSpeed(int speed) {

    if(visibleInMainWindow())
        _torrentRowModel->setUploadSpeed(speed);
}

void Torrent::addPeer(core::Peer * peer) {

    if(_peers.count(peer->endPoint()) > 0)
        throw std::runtime_error("Peer already added.");

    // Create controller::Peer, and add to map
    _peers[peer->endPoint()] = std::unique_ptr<Peer>(new Peer(peer,
                                                              &_classicPeerTableModel,
                                                              &_buyerTableModel,
                                                              &_observerTableModel,
                                                              &_sellerTableModel,
                                                              &_sellersTableModel,
                                                              &_buyersTableModel));
}

void Torrent::removePeer(const libtorrent::tcp::endpoint & endPoint) {

   // Get corresponding controoler::Peer
   auto it = _peers.find(endPoint);

   if(it == _peers.cend())
       throw std::runtime_error("Peer not present.");

   // Remove peer
   _peers.erase(it);
}

void Torrent::setTorrentPluginPresent(core::TorrentPlugin *) {

    /// TorrentPlugin

    core::TorrentPlugin * plugin = _torrent->torrentPlugin();

    for(auto mapping : plugin->peers())
        addPeerPlugin(mapping.second);

    QObject::connect(plugin,
                     &core::TorrentPlugin::peerPluginAdded,
                     this,
                     &Torrent::addPeerPlugin);

    QObject::connect(plugin,
                     &core::TorrentPlugin::peerPluginRemoved,
                     this,
                     &Torrent::removePeerPlugin);

    /// Session

    core::Session * session = plugin->session();

    assert(session != nullptr);

    // Set peer counts

    {
        unsigned int numberOfBuyers, numberOfSellers, numberOfObservers, numberOfUnAnnounced;

        getPeerCounts(session, numberOfBuyers, numberOfSellers, numberOfObservers, numberOfUnAnnounced);

        setNumberOfBuyers(numberOfBuyers);
        setNumberOfSellers(numberOfSellers);
    }

    setBalance(getBalance(session));
    setSessionMode(session->mode());

    for(auto mapping : session->connections())
        addConnection(mapping.second);

    QObject::connect(session,
                     &core::Session::modeChanged,
                     this,
                     &Torrent::pluginModeChanged);

    QObject::connect(session,
                     &core::Session::connectionAdded,
                     this,
                     &Torrent::connectionAdded);

    QObject::connect(session,
                     &core::Session::connectionRemoved,
                     this,
                     &Torrent::connectionRemoved);
}

void Torrent::setTorrentPluginAbsent() {

    if(visibleInMainWindow()) {
        _torrentRowModel->unsetTorrentPluginPresence();

        // Iterate peers and unset peer plugin?
    }
}

void Torrent::addPeerPlugin(core::PeerPlugin * peerPlugin) {

    auto it = _peers.find(peerPlugin->endPoint());

    if(it == _peers.cend())
        throw std::runtime_error("Cannot add peer plugin: no corresponding peer exists");

    it->second->setPeerPlugin(peerPlugin);
}

void Torrent::removePeerPlugin(const libtorrent::tcp::endpoint & endPoint) {

    auto it = _peers.find(endPoint);

    if(it == _peers.cend())
        throw std::runtime_error("Cannot remove peer plugin: no correspondin peer exists");

    // Not yet supported: it->second->unSetPeerPlugin();
}

void Torrent::setSessionMode(protocol_session::SessionMode mode) {

    if(visibleInMainWindow())
        _torrentRowModel->setSessionMode(mode);

    // what happens to other values? like balance, etc
}

void Torrent::addConnection(core::Connection * connection) {

    auto it = _peers.find(peerPlugin->endPoint());

    if(it == _peers.cend())
        throw std::runtime_error("Cannot add connection: no corresponding peer exists");

    it->second->setConnection(connection);
}

void Torrent::removeConnection(const libtorrent::tcp::endpoint & endPoint) {

    auto it = _peers.find(endPoint);

    if(it == _peers.cend())
        throw std::runtime_error("Cannot remove connection: no correspondin peer exists");

    // Not yet supported: it->second->unSetConnection();
}

quint32 Torrent::numberOfSellers() const {
    return _numberOfSellers;
}

void Torrent::setNumberOfSellers(quint32 numberOfSellers) {
    _numberOfSellers = numberOfSellers;

   if(visibleInMainWindow())
       _torrentRowModel->setNumberOfSellers(numberOfSellers);
}

quint32 Torrent::numberOfBuyers() const {
    return _numberOfBuyers;
}

void Torrent::setNumberOfBuyers(quint32 numberOfBuyers) {
    _numberOfBuyers = numberOfBuyers;

    if(visibleInMainWindow())
        _torrentRowModel->setNumberOfBuyers(numberOfBuyers);
}

quint32 Torrent::numberOfUnannounced() const {
    return _numberOfUnannounced;
}

void Torrent::setNumberOfUnannounced(quint32 numberOfUnannounced) {
    _numberOfUnannounced = numberOfUnannounced;

    // no ui representation to update
}

quint32 Torrent::numberOfObservers() const {
    return _numberOfObservers;
}

void Torrent::setNumberOfObservers(quint32 numberOfObservers) {
    _numberOfObservers = numberOfObservers;

    // no ui representation to update
}

quint64 Torrent::balance() const {
    return _balance;
}

void Torrent::setBalance(quint64 balance) {
    _balance = balance;

    if(visibleInMainWindow())
        _torrentRowModel->setBalance(balance);
}

}
}
}
