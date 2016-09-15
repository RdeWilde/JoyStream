/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#include <controller/Torrent.hpp>
#include <core/core.hpp>
#include <gui/gui.hpp>

namespace joystream {
namespace classic {
namespace controller {

Torrent::Torrent(ApplicationController * applicationController,
                 gui::TorrentTreeViewRow * row,
                 core::Torrent * torrent)
    : _applicationController(applicationController)
    , _row(row)
    , _torrent(torrent) {

    // Set inital values
    _row->setName(torrent->name());

    if(torrent->isPaused())
        _row->setPaused(true);
    else
        _row->setState(torrent->state(), torrent->progress());

    _row->setUploadSpeed(torrent->uploadRate());
    _row->setDownloadSpeed(torrent->downloadRate());

    // When plugin is present, then set it up, otherwise set it as absent
    if(torrent->torrentPlugin())
        setTorrentPluginPresent(torrent->torrentPlugin());
    else
        setTorrentPluginAbsent();

    // Connect model signals to slots on this object
    QObject::connect(torrent,
                     &core::Torrent::torrentPluginAdded,
                     this,
                     &Torrent::setTorrentPluginPresent);

    QObject::connect(torrent,
                     &core::Torrent::torrentPluginRemoved,
                     this,
                     &Torrent::setTorrentPluginAbsent);

    // Connect model signals to slots on row
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

void Torrent::pluginModeChanged(protocol_session::SessionMode mode) {

    _row->setSessionMode(mode);

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
    _row->setSessionMode(session->mode());

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
    _row->unsetTorrentPluginPresence();
}

void Torrent::updateBalance() {

    // Get session
    core::Session * session = getSession();

    // Recompute new balance
    int64_t balance = getBalance(session);

    // Update view
    _row->setBalance(balance);
}

void Torrent::setState(libtorrent::torrent_status::state_t state, float progress) {
    _row->setState(state, progress);
}

void Torrent::setPaused(bool paused) {
    _row->setPaused(paused);
}

void Torrent::setDownloadSpeed(int speed) {
    _row->setDownloadSpeed(speed);
}

void Torrent::setUploadSpeed(int speed) {
    _row->setUploadSpeed(speed);
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
