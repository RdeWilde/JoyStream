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
#include <QProcess>

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
    MainWindow(Controller * controller, Wallet * wallet);

    // Destructor
    ~MainWindow();

public slots:

    // Manage torrents
    void addTorrent(const TorrentViewModel * model);
    void addTorrentFailed(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec);

    /**
    // Torrent Plugin starting events
    //void addTorrentPlugin(const libtorrent::sha1_hash & infoHash, PluginMode mode);
    void startedSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration);
    void startedBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration & configuration, const UnspentP2PKHOutput & utxo);

    // Peer plugin events
    void addSellerPeerPlugin();
    void addBuyerPeerPlugin();

    // Torrent Plugin status events
    //void updateTorrentPluginStatus(const TorrentPluginStatusAlert * torrentPluginStatusAlert);// TorrentPluginStatus status
    void updateBuyerTorrentPluginStatus(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Status & status);
    void updateSellerTorrentPluginStatus(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Status & status);
    */

    // Plugins started
    //void startedSellerTorrentPlugin(const SellerTorrentPluginViewModel * model);
    //void startedBuyerTorrentPlugin(const BuyerTorrentPluginViewModel * model);
    //void startedTorrentPlugin(const libtorrent::sha1_hash & infoHash);

    // Plugin status event
    void updatePluginStatus(const Plugin::Status & status);

    // Wallet balance has changed
    void updateWalletBalance(quint64 balance);

    /**
     * Torrent table events
     */

    // Shows context menu, at given position,
    // for corresponding torrent
    void showContextMenu(const QPoint & pos);

    // Show parts of view
    void showAddTorrentFromTorrentFileDialog(const QString & torrentFile);
    void showAddTorrentFromMagnetLinkDialog(const QString & magnetLink);
    void showAddTorrentPluginConfigurationDialog(const libtorrent::torrent_info & torrentInfo, const libtorrent::torrent_status & torrentStatus);

    void showTorrentPluginDialog(const libtorrent::sha1_hash & infoHash);
    void showSellerTorrentPluginDialog(const SellerTorrentPluginViewModel * sellerTorrentPluginViewModel);
    void showBuyerTorrentPluginDialog(const BuyerTorrentPluginViewModel * buyerTorrentPluginViewModel);

    // Mouse click events
    void on_addTorrentFilePushButton_clicked();
    void on_addMagnetLinkPushButton_clicked();
    void on_walletPushButton_clicked();

    // Mouse click on row in torrent table
    void torrentTableClicked(const QModelIndex & index);

    // Start VLC video player
    void startVLC(const libtorrent::sha1_hash & infoHash);

private:

    // View
    Ui::MainWindow * ui;

    // Controller
    Controller * _controller;

    // Wallet
    Wallet * _wallet;

    // Torrent table view model
    QStandardItemModel _torrentTableViewModel;

    // View for torrent with corresponding info hash
    // Must use pointer values since its a QObject
    QMap<libtorrent::sha1_hash, TorrentView *> _torrentViews;

    // Vector with info hash of torrent in corresponding table position
    // Is used to lookup views based on row selected by user
    QVector<libtorrent::sha1_hash> _rowToInfoHash;

    // View model for torrent with corresponding info hash
    //QMap<libtorrent::sha1_hash, const TorrentViewModel *> _torrentViewModels;

    // Used to launch processes, e.g. VLC,
    // has to be member, since it cannot go out of scope, since that kills
    // all started processes
    QProcess _processLauncher;

    // Uses _rowToViewMapping to lookup view in _torrentViews
    TorrentView * rowToView(int row);

protected:

    void closeEvent(QCloseEvent *event);

    // Handler for dropping torrent file on client
    void dropEvent(QDropEvent *e);
};

#endif // MAIN_WINDOW_HPP
