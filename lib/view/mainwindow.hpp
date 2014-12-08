#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "extension/TorrentPlugin.hpp"

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
#include <QPoint>
#include <QMenu>
#include <QLoggingCategory>

#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/add_torrent_params.hpp>

#include <map>

namespace Ui {
class MainWindow;
}

// Forward declarations
class Controller;
class PeerPlugin;
class TorrentPluginStatus;
class PeerPluginStatus;
class TorrentViewModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    // Constructor
    MainWindow(Controller * controller, QLoggingCategory & category);

    // Destructor
    ~MainWindow();

    // Controller calls
    void addTorrent(const libtorrent::sha1_hash & info_hash, const QString & torrentName, int totalSize);
    void addTorrentFailed(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec);

    void removeTorrent(const libtorrent::sha1_hash & info_hash);

    void updateTorrentStatus(const std::vector<libtorrent::torrent_status> & torrentStatusVector);
    void updateTorrentStatus(const libtorrent::torrent_status & torrentStatus); // start, stopp, stats
    void updateTorrentPluginStatus(TorrentPluginStatus status);

    void addPeerPlugin(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endPoint);
    void updatePeerPluginStatus(PeerPluginStatus status);
    void removePeerPlugin(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endPoint);

private:

    // View
    Ui::MainWindow * ui;

    // Controller
    Controller * controller_;

    // Logging category
    QLoggingCategory & category_;

    /*
     * View-models
     */

    // Torrent table
    QStandardItemModel torrentTableViewModel_; // View model
    QMenu * torrentTableContextMenu; // Context menu
    QModelIndex torrentTableLastIndexClicked; // Last model index for mouse click
    std::map<libtorrent::sha1_hash, TorrentViewModel *> torrentViewModels; // Maps info_hash of models to corresponding TorrentViewModel

    // Peer Plugins table
    QMenu * peerPluginsTableContextMenu; // context menu
    QModelIndex peerPluginsTableLastIndexClicked; // Last model index for mouse click

    // Utilities
    const libtorrent::sha1_hash & getInfoHashOfLastClickedTorrent();

protected:

    void closeEvent(QCloseEvent *event);

private slots: // These slots get signals from view objects.

    void on_addTorrentFilePushButton_clicked();
    void on_addMagnetLinkPushButton_clicked();

public slots:

    // These slots are used to tap into native QMenu ui signals.
    void showContextMenu(QPoint pos);
    void pauseMenuAction();
    void startMenuAction();
    void removeMenuAction();

    void torrentTableClicked(const QModelIndex & index);
};

#endif // MAIN_WINDOW_HPP
