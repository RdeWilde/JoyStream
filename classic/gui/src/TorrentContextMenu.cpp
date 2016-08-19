/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 18 2016
 */

#include <gui/TorrentContextMenu.hpp>
#include <protocol_session/protocol_session.hpp>

namespace joystream {
namespace classic {
namespace gui {

TorrentContextMenu::TorrentContextMenu(QWidget * parent,
                                       bool torrentPluginActionsVisible,
                                       bool paused,
                                       const protocol_session::SessionMode & mode)
    : QMenu(parent) {

    // Create and add actions to menu, which takes ownership
    addAction(tr("Manage session"));
    addAction(tr("View peers"));
    addAction(tr("Remove Torrent"));

    addSection("")->setText(tr("Mode"));
    QActionGroup * sessionModeActionGroup = new QActionGroup(this);
    addAction(sessionModeActionGroup->addAction(tr("Selling")));
    addAction(sessionModeActionGroup->addAction(tr("Buying")));
    addAction(sessionModeActionGroup->addAction(tr("Observing")));

    addSection("")->setText(tr("State"));
    QActionGroup * stateActionGroup = new QActionGroup(this);
    addAction(stateActionGroup->addAction(tr("Pause")));
    addAction(stateActionGroup->addAction(tr("Start")));

    addSection("");
    addAction(tr("Stream media"));
    addAction(tr("Open Folder Location"));

    // Set torrent plugin action visibility
    setTorrentPluginActionVisibility(torrentPluginActionsVisible);

    // Set paused status of torrent
    setPauseStatus(paused);

    // Set mode
    setSessionMode(mode);
}

void TorrentContextMenu::setTorrentPluginActionVisibility(bool visible) {

    manageSessionAction()->setEnabled(visible);
    sessionModeActionGroup()->setEnabled(visible);
}

void TorrentContextMenu::setPauseStatus(bool paused) {

    pauseAction()->setChecked(paused);
    startAction()->setChecked(paused); // not sure if we need this one when they are in same group
}

void TorrentContextMenu::setSessionMode(const protocol_session::SessionMode & mode) {

    switch(mode) {

        case protocol_session::SessionMode::not_set: break;
        case protocol_session::SessionMode::buying: sessionToBuyingModeAction()->setChecked(true); break;
        case protocol_session::SessionMode::selling: sessionToSellingModeAction()->setChecked(true); break;
        case protocol_session::SessionMode::observing: sessionToObservingModeAction()->setChecked(true); break;

        default:
        assert(false);
    }
}

QAction * TorrentContextMenu::manageSessionAction() const noexcept {
    return actions().at(0);
}

QAction * TorrentContextMenu::viewPeersAction() const noexcept {
    return actions().at(1);
}

QAction * TorrentContextMenu::removeTorrentAction() const noexcept {
    return actions().at(2);
}

QAction * TorrentContextMenu::sessionToSellingModeAction() const noexcept {
    return actions().at(3);
}

QAction * TorrentContextMenu::sessionToBuyingModeAction() const noexcept {
    return actions().at(4);
}

QAction * TorrentContextMenu::sessionToObservingModeAction() const noexcept {
    return actions().at(5);
}

QAction * TorrentContextMenu::pauseAction() const noexcept {
    return actions().at(6);
}

QAction * TorrentContextMenu::startAction() const noexcept {
    return actions().at(7);
}

QAction * TorrentContextMenu::streamMediaAction() const noexcept {
    return actions().at(8);
}

QAction * TorrentContextMenu::openFolderLocationAction() const noexcept {
    return actions().at(9);
}

QActionGroup * TorrentContextMenu::sessionModeActionGroup() const noexcept {
    return sessionToBuyingModeAction()->actionGroup();
}

QActionGroup * TorrentContextMenu::stateActionGroup() const noexcept {
    return pauseAction()->actionGroup();
}

}
}
}
