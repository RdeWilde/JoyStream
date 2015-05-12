#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "extension/TorrentPlugin.hpp"
#include "extension/Plugin.hpp" // Plugin::Status
#include "extension/BuyerTorrentPlugin.hpp" // BuyerTorrentPlugin::Status
#include "extension/SellerTorrentPlugin.hpp" // SellerTorrentPlugin::Status

#include <QMainWindow>
#include <QStandardItemModel>

#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/add_torrent_params.hpp>

#include <QMap>
#include <QPoint>
#include <QLoggingCategory>

namespace Ui {
class MainWindow;
}

class Controller;
class PeerPlugin;
class PeerPluginStatus;
class TorrentView;
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

public slots:

    // Manage torrents
    void addTorrent(const TorrentViewModel * model);
    void removeTorrent(const libtorrent::sha1_hash & info_hash);
    void addTorrentFailed(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec);

    // Torrent Plugin starting events
    //void addTorrentPlugin(const libtorrent::sha1_hash & infoHash, PluginMode mode);
    void registerSellerTorrentPluginStarted(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration);
    void registerBuyerTorrentPluginStarted(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration & configuration, const UnspentP2PKHOutput & utxo);

    // Torrent Plugin status events
    //void updateTorrentPluginStatus(const TorrentPluginStatusAlert * torrentPluginStatusAlert);// TorrentPluginStatus status
    void updateBuyerTorrentPluginStatus(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Status & status);
    void updateSellerTorrentPluginStatus(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Status & status);

    // Plugin status event
    void updatePluginStatus(const Plugin::Status & status);

    //
    void updateWalletBalance(quint64 balance);

    void addPeerPlugin(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endPoint);
    //void updatePeerPluginStatus(const PeerPluginStatus & status);
    void removePeerPlugin(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endPoint);

    /**
     * Torrent table events
     */

    // These slots are used to tap into native QMenu ui signals.
    void showContextMenu(QPoint pos);

    // Mouse click on row in torrent table
    void torrentTableClicked(const QModelIndex & index);

    /**
     * Show parts of view
     */
    void showAddTorrentFromTorrentFileDialog(const QString & torrentFile);
    void showAddTorrentFromMagnetLinkDialog(const QString & magnetLink);
    void showAddTorrentPluginConfigurationDialog(const libtorrent::torrent_info & torrentInfo, const libtorrent::torrent_status & torrentStatus);
    void showSellerTorrentPluginDialog(const SellerTorrentPluginViewModel * sellerTorrentPluginViewModel);
    void showBuyerTorrentPluginDialog(const BuyerTorrentPluginViewModel * buyerTorrentPluginViewModel);

    /**
     * Mouse click events
     */
    void on_addTorrentFilePushButton_clicked();
    void on_addMagnetLinkPushButton_clicked();
    void on_walletPushButton_clicked();

private:

    // View
    Ui::MainWindow * ui;

    // Controller
    Controller * _controller;

    // Wallet
    Wallet * _wallet;

    // Logging category
    QLoggingCategory & _category;

    // Torrent table view model
    QStandardItemModel _torrentTableViewModel;

    // Maps info_hash of models to corresponding torrent view
    // Must use pointer values since its a QObject
    QMap<libtorrent::sha1_hash, TorrentView *> _torrentViews;

    // Get view model for torrent in given row
    TorrentView * torrentViewInTableRow(int row);

protected:

    void closeEvent(QCloseEvent *event);
};

#endif // MAIN_WINDOW_HPP
