/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_CLASSIC_GUI_TORRENT_HPP
#define JOYSTREAM_CLASSIC_GUI_TORRENT_HPP

#include <core/core.hpp>

#include <QMenu>
#include <QAction>

class QStandardItem;

class BitcoinDisplaySettings;

namespace joystream {
namespace classic {
namespace gui {

class Torrent : public QObject
{
    Q_OBJECT

public:

    Torrent(QObject * parent,
                const std::weak_ptr<core::Torrent> & torrent,
                const BitcoinDisplaySettings * settings,
                QStandardItem * nameItem,
                QStandardItem * sizeItem,
                QStandardItem * stateItem,
                QStandardItem * uploadSpeed,
                QStandardItem * downloadSpeed,

                QStandardItem * buyersItem,
                QStandardItem * sellersItem,
                QStandardItem * pluginInstalledItem,
                QStandardItem * balanceItem);

    // Text conversion routines
    static QString sizeToString(qint64 size);
    static QString pluginInstalledToString(PluginInstalled pluginInstalled);
    static QString torrentStateToString(bool paused, libtorrent::torrent_status::state_t state, float progress);
    static QString speedToString(int downloadRate, int uploadRate);
    static QString peersToString(int numberOfPeers, int numberOfPeersWithExtension);
    static QString balanceToString(qint64 balance, const BitcoinDisplaySettings * settings);

public slots:

    // Update
    void updatePluginInstalled(PluginInstalled pluginInstalled);

    void updateStatus(const libtorrent::torrent_status & status);
    void updateSize(qint64 totalSize);
    void updateNumberOfBuyers(quint32 num);
    void updateNumberOfSellers(quint32 num);
    void updateBalance(qint64 balance);

    // Popup context menu in given point
    void showContextMenu(const QPoint & point);

    /// Slots below are used to peferform action on core model, and are intended to
    /// for use by view signals

    void pause();
    void start();
    void remove();
    void viewExtension();
    void startStreamPlayback();

private:

    // Info hash of torrent
    libtorrent::sha1_hash _infoHash;

    // Model of torrent
    std::weak_ptr<core::Torrent> _torrent;

    // Display settings for bitcoin
    const BitcoinDisplaySettings * _settings;

    // View model pointers
    // Objects are owned by QStandardItemModel, not us
    QStandardItem * _nameItem,
                  * _sizeItem,
                  * _stateItem,
                  * _uplaodSpeed,
                  * _downloadSpeed,
                  * _numberOfBuyerPeersItem,
                  * _numberOfSellerPeersitem,
                  * _sessionModeItem,
                  * _balanceItem;

    // Context menu
    QMenu _torrentTableContextMenu;

    // Context menu actions
    QAction _manageSessionAction,
            _viewPeersAction,
            _removeTorrentAction,
            _sessionToSellingModeAction,
            _sessionToBuyingModeAction,
            _sessionToObservingModeAction,

            _pauseAction,
            _startAction,
            _stopSessionAction,

            _streamMediaAction,
            _openFolderLocationAction;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_TORRENT_HPP
