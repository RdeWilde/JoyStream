#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "extension/TorrentPlugin.hpp"
#include "extension/Plugin.hpp" // Plugin::Status
#include "extension/BuyerTorrentPlugin.hpp" // BuyerTorrentPlugin::Status
#include "extension/SellerTorrentPlugin.hpp" // SellerTorrentPlugin::Status

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
#include <QMap>
#include <QPoint>

#include <QLoggingCategory>

#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/add_torrent_params.hpp>

#include <map>

namespace Ui {
class MainWindow;
}

class Controller;
class PeerPlugin;
class PeerPluginStatus;
class TorrentViewModel;
class Wallet;
class SellerTorrentPluginViewModel;
class BuyerTorrentPluginViewModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    // Constructor
    MainWindow(Controller * controller, Wallet * wallet, QLoggingCategory & category);

    // Destructor
    ~MainWindow();

    // Controller calls
    void addTorrent(const libtorrent::sha1_hash & info_hash, const QString & torrentName, int totalSize);
    void removeTorrent(const libtorrent::sha1_hash & info_hash);

    void addTorrentFailed(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec);

    //void addTorrentPlugin(const libtorrent::sha1_hash & infoHash, PluginMode mode);
    void registerSellerTorrentPluginStarted(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration);
    void registerBuyerTorrentPluginStarted(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration & configuration);

    void updateTorrentStatus(const std::vector<libtorrent::torrent_status> & torrentStatusVector);
    void updateTorrentStatus(const libtorrent::torrent_status & torrentStatus); // start, stopp, stats
    //void updateTorrentPluginStatus(const TorrentPluginStatusAlert * torrentPluginStatusAlert);// TorrentPluginStatus status

    void updateBuyerTorrentPluginStatus(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Status & status);
    void updateSellerTorrentPluginStatus(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Status & status);

    void updatePluginStatus(const Plugin::Status & status);

    //
    void updateWalletBalance(quint64 balance);

    void addPeerPlugin(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endPoint);
    //void updatePeerPluginStatus(const PeerPluginStatus & status);
    void removePeerPlugin(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endPoint);

    // Show parts of view
    void showAddTorrentFromTorrentFileDialog(const QString & torrentFile);
    void showAddTorrentFromMagnetLinkDialog(const QString & magnetLink);
    void showAddTorrentPluginConfigurationDialog(const libtorrent::torrent_info & torrentInfo, const libtorrent::torrent_status & torrentStatus);

public slots:

    // These slots are used to tap into native QMenu ui signals.
    void showContextMenu(QPoint pos);

    void torrentTableClicked(const QModelIndex & index);

    // TorrentViewModel signals by cor
    void showSellerTorrentPluginDialog(const SellerTorrentPluginViewModel * sellerTorrentPluginViewModel);
    void showBuyerTorrentPluginDialog(const BuyerTorrentPluginViewModel * buyerTorrentPluginViewModel);

private:

    // View
    Ui::MainWindow * ui;

    // Controller
    Controller * _controller;

    // Wallet
    Wallet * _wallet;

    // Logging category
    QLoggingCategory & _category;

    /*
     * View-model
     */

    // Torrent table view model
    QStandardItemModel _torrentTableViewModel;

    // Last model index for mouse click
    //QModelIndex _torrentTableLastIndexClicked;

    // Info hash of torrent in the position the torrent holds in the torrent table view
    QVector<libtorrent::sha1_hash> _torrentInTableRow;

    // Maps info_hash of models to corresponding TorrentViewModel
    // Must use pointer values since members are QObjects, which cannot be copied and assigned
    QMap<libtorrent::sha1_hash, TorrentViewModel *> _torrentViewModels;

    /**
    // Peer Plugins table
    QMenu * _peerPluginsTableContextMenu; // context menu
    QModelIndex _peerPluginsTableLastIndexClicked; // Last model index for mouse click
    */

    // Get view model for torrent in given row
    TorrentViewModel * torrentViewModelInTableRow(int row);

protected:

    void closeEvent(QCloseEvent *event);

private slots: // These slots get signals from view objects.

    void on_addTorrentFilePushButton_clicked();
    void on_addMagnetLinkPushButton_clicked();

    void on_walletPushButton_clicked();
};

#endif // MAIN_WINDOW_HPP
