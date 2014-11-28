#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
#include <QPoint>
#include <QMenu>
#include <QLoggingCategory>

#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/add_torrent_params.hpp>

#include "extension/TorrentPlugin.hpp"

namespace Ui {
class MainWindow;
}

// Forward declarations
class Controller;
class PeerPlugin;
class TorrentPluginStatus;
class PeerPluginStatus;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    // Constructor
    MainWindow(Controller * controller, QLoggingCategory & category);

    // Destructor
    ~MainWindow();

    // Controller calls
    void addTorrent(const libtorrent::sha1_hash & info_hash, const std::string & torrentName, int totalSize);
    void addTorrentFailed(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec);

    void removeTorrent(const libtorrent::sha1_hash & info_hash);

    void updateTorrentStatus(const std::vector<libtorrent::torrent_status> & torrentStatusVector);
    void updateTorrentStatus(const libtorrent::torrent_status & torrentStatus); // start, stopp, stats
    void updateTorrentPluginStatus(TorrentPluginStatus status);

    void addPaymentChannel(PeerPlugin * peerPlugin);
    void updatePeerPluginStatus(PeerPluginStatus status);


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

    // Torrent table view-model
    QStandardItemModel * model;

    /*
    * Maps torrent info_hash to the model item containing the name of the torrent.
    * This makes all model items associated with a given torrent recoverable
    * based on its info_hash. Using only name directly would have been unwise,
    * as two torrents with the same name, but different info_hash, could potentially
    * be added and then cause ambiguity.
    */
    std::vector<libtorrent::sha1_hash> infoHashInRow;

    // Context menu on torrents
    QMenu * tableViewContextMenu;
    QModelIndex lastIndexClicked;

    int findRowFromInfoHash(const libtorrent::sha1_hash & info_hash);
    libtorrent::torrent_handle getTorrentHandleLastClicked();

    // Payment channel table view-models (one per torrent)
    std::vector<QStandardItemModel *> paymentChannelTableViewModels;

    // Context menu on payment channel table
    QMenu * paymentChannelTableContextMenu;
    QModelIndex paymentChannelTableLastIndexClicked;

    /*
    struct TorrentViewModel {

        QStandardItemModel * paymentChannelTableViewModel;

    };

    std::vector<TorrentViewModel>
    */

protected:

    void closeEvent(QCloseEvent *event);

private slots: // These slots get signals from view objects.

    void on_addTorrentFilePushButton_clicked();
    void on_addMagnetLinkPushButton_clicked();

public slots:

    // These slots are used to tap into native QMenu ui signals.
    void customMenuRequested(QPoint pos);
    void pauseMenuAction();
    void startMenuAction();
    void removeMenuAction();

    void torrentQTableViewClicked(const QModelIndex & index);
};

#endif // MAIN_WINDOW_HPP
