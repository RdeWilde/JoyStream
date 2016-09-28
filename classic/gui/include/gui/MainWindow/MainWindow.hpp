/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_CLASSIC_GUI_MAINWINDOW_HPP
#define JOYSTREAM_CLASSIC_GUI_MAINWINDOW_HPP

#include <QMainWindow>
#include <QStandardItemModel>
#include <QLabel>
#include <QStatusBar>

#include <map>
#include <memory>

class BitcoinDisplaySettings;

namespace Ui {
class MainWindow;
}

namespace joystream {
namespace classic {
namespace gui {

class TorrentTreeViewRow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(const QString & title,
               unsigned int applicationMajorVersion,
               unsigned int applicationMinorVersion,
               unsigned int applicationPatchVersion,
               const BitcoinDisplaySettings * settings);

    ~MainWindow();

    void setTorrentTreeViewModel(QStandardItemModel * model);

public slots:

    /// Bitcoin balances

    void setTotalSentSinceStart(quint64 sentSinceStart);

    void setTotalReceivedSinceStart(quint64 receivedSinceStart);

    void setConfirmedBalance(quint64 confirmedBalance);

    void setUnconfirmedBalance(quint64 unconfirmedBalance);

    /// Wallet events

    void walletSynched();

    void walletSynching();

    void walletConnected();

    void walletDisconnected();

    /// Alter view

    void maximize();

    void minimize();

    /// Utilities

    QModelIndex indexAtTorrentTreeViewPosition(const QPoint & pos) const noexcept;

signals:

    void contextMenuRequestedAtTreeViewPoint(const QPoint & point);

    void torrentTreeViewClicked(const QModelIndex & index);

    void ignoredCloseEventOccured();

    // Add signals for menu buttons?

    // Add signals for toolbar buttons?

private:

    // View
    Ui::MainWindow * ui;

    // Status bar at bottom of the page
    QStatusBar _statusBar;
    QLabel _statusLabel;

    // How bitcoin should be displayed in entire view
    const BitcoinDisplaySettings * _bitcoinDisplaySettings;

    void closeEvent(QCloseEvent *event);
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_MAINWINDOW_HPP
