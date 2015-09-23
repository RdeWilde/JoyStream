/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <gui/MainWindow.hpp>
#include "ui_MainWindow.h"
#include <gui/AddTorrentDialog.hpp>
#include <gui/WalletDialog.hpp>
#include <gui/ReceiveFundsDialog.hpp>
#include <gui/TorrentView.hpp>
#include <gui/SellerTorrentPluginConfigurationDialog.hpp>
#include <gui/BuyerTorrentPluginConfigurationDialog.hpp>
#include <gui/SellerTorrentPluginDialog.hpp>
#include <gui/BuyerTorrentPluginDialog.hpp>
#include <core/controller/Controller.hpp>
#include <core/controller/SellerTorrentPluginViewModel.hpp>
#include <core/controller/BuyerTorrentPluginViewModel.hpp>
#include <core/controller/TorrentViewModel.hpp>
#include <core/extension/TorrentPlugin.hpp>
#include <core/extension/PeerPlugin.hpp>
#include <core/extension/Alert/PluginStatusAlert.hpp>
#include <core/extension/BuyerTorrentPlugin.hpp>
#include <core/extension/SellerTorrentPlugin.hpp>
#include <common/BitcoinRepresentation.hpp>
#include <common/BitcoinDisplaySettings.hpp>
#include <wallet/Manager.hpp>

#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_handle.hpp>

#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QInputDialog>
#include <QAction>
#include <QCloseEvent>
#include <QDebug>
#include <QMimeData>
#include <QLabel>
#include <QDesktopServices>

MainWindow::MainWindow(Controller * controller, Wallet::Manager * wallet, const QString & appendToTitle)
    : ui(new Ui::MainWindow)
    , _controller(controller)
    , _wallet(wallet)
    , _torrentTableViewModel(0, 6)
    , _walletBalanceUpdateTimer()
    , _bitcoinDisplaySettings() { //(Fiat::USD, 225) {

    ui->setupUi(this);

    /**
     * Look of main windw
     */
    // Alter window title
    setWindowTitle("JoyStream" + appendToTitle);

    // Freeze size of dialog
    setFixedSize(size());

    // Status bar
    _statusLabel.setText("Release 0.0.1, auto-update expected by: to be announced");
    _statusBar.addWidget(&_statusLabel);
    setStatusBar(&_statusBar);

    /**
     * buttons and menus
     */

    ui->walletPushButton->hide();

    /**
    // Set icon
    setWindowIcon(QIcon(":/prefix/test"));

    ui->addTorrentFilePushButton->setIcon(QIcon(":/prefix/test"));

    //QPixmap walletIcon("C:/ART/images/Bitcoin_pocket_or_wallet_32.png");
    QPixmap walletIcon("C:/ART/images/Card_wallet_32.png");
    ui->walletPushButton->setIcon(walletIcon);

    // "C:/ART/images/Settings_Work_Tool_32.png"
    QPixmap settingsIcon(":/images/add"); //
    //QIcon settingsIcon(":/images/resources/Settings_Work_Tool_32.png");

    ui->settingsPushButton->setIcon(settingsIcon);
    */

    /**
     * Setup colours of bitcon balances
     */

    QPalette earnedBalancePalette = ui->earnedBalanceLabel->palette();
    earnedBalancePalette.setColor(ui->earnedBalanceLabel->foregroundRole(), Qt::green);
    //palette.setColor(ui->pLabel->foregroundRole(), Qt::yellow);
    ui->earnedBalanceLabel->setPalette(earnedBalancePalette);
    ui->earnedTitleQlabel->setPalette(earnedBalancePalette);

    QPalette spentBalancePalette = ui->spentBalanceLabel->palette();
    spentBalancePalette.setColor(ui->spentBalanceLabel->foregroundRole(), Qt::red);
    //palette.setColor(ui->pLabel->foregroundRole(), Qt::yellow);
    ui->spentBalanceLabel->setPalette(spentBalancePalette);
    ui->spentTitleLabel->setPalette(spentBalancePalette);

    QPalette balanceLabelPalette = ui->balanceLabel->palette();
    balanceLabelPalette.setColor(ui->balanceLabel->foregroundRole(), Qt::blue);
    //palette.setColor(ui->pLabel->foregroundRole(), Qt::yellow);
    ui->balanceLabel->setPalette(balanceLabelPalette);
    ui->balanceTitleLabel->setPalette(balanceLabelPalette);

    /**
     *
    QFont torrentTableHeaderFont = this->ui->torrentsTable->header()->font();
    torrentTableHeaderFont.setPointSize(12);
    this->ui->torrentsTable->header()->setFont(torrentTableHeaderFont);
     */

    /**
     * Set initial balances
     */
    updateWalletBalance(_wallet->lastComputedZeroConfBalance());
    updatePluginStatus(Plugin::Status(0, 0, 0));

    /**
     * Add columns to model view model
     */
    QStringList columnNames;

    columnNames << "Name"
                << "Size"
                << "State"
                << "Speed"
                << "#Buyers"
                << "#Sellers"
                << "Mode"
                << "Balance";

    _torrentTableViewModel.setHorizontalHeaderLabels(columnNames);

    // Set table model to view model
    ui->torrentsTable->setModel(&_torrentTableViewModel);

    // Set column width
    ui->torrentsTable->setColumnWidth(0, 350);
    ui->torrentsTable->setColumnWidth(1, 90);
    ui->torrentsTable->setColumnWidth(2, 140);
    ui->torrentsTable->setColumnWidth(3, 190);
    ui->torrentsTable->setColumnWidth(4, 70);
    ui->torrentsTable->setColumnWidth(5, 70);
    ui->torrentsTable->setColumnWidth(6, 90);
    ui->torrentsTable->setColumnWidth(7, 50);

    // Hide buttons
    //ui->addMagnetLinkPushButton->setVisible(false);
    //ui->settingsPushButton->setVisible(false);

    // Align spending/wallet info labels
    ui->earnedBalanceLabel->setAlignment(Qt::AlignRight);
    ui->spentBalanceLabel->setAlignment(Qt::AlignRight);
    ui->balanceLabel->setAlignment(Qt::AlignRight);

    /**
     * Connect controller signals with view slots
     */
    QObject::connect(controller,
                     SIGNAL(addedTorrent(const TorrentViewModel*)),
                     this,
                     SLOT(addTorrent(const TorrentViewModel*)));

    QObject::connect(controller,
                     SIGNAL(closed()),
                     this,
                     SLOT(close()));

    QObject::connect(controller,
                     SIGNAL(failedToAddTorrent(std::string,libtorrent::sha1_hash,libtorrent::error_code)),
                     this,
                     SLOT(addTorrentFailed(std::string,libtorrent::sha1_hash,libtorrent::error_code)));

    QObject::connect(controller,
                     SIGNAL(pluginStatusUpdate(Plugin::Status)),
                     this,
                     SLOT(updatePluginStatus(Plugin::Status)));

    QObject::connect(controller,
                    SIGNAL(torrentCheckedButHasNoPlugin(libtorrent::torrent_info,libtorrent::torrent_status)),
                    this,
                    SLOT(showAddTorrentPluginConfigurationDialog(libtorrent::torrent_info,libtorrent::torrent_status)));

    /**
     * Didnt work for whatever reason, also watch out for
     * recusive closing due to close event handling in this view
     * check out closeEvent routine
    QObject::connect(controller,
                     SIGNAL(closed()),
                     this,
                     SLOT(close()));
    */

    /**
    QObject::connect(controller,
                     SIGNAL(torrentPluginStarted()),
                     this,
                     SLOT()
    */

    /**
     * Context menu on table view


    // Setup context menu capacity on table view
    ui->torrentsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->torrentsTable,
            SIGNAL(customContextMenuRequested(QPoint)),
            this,
            SLOT(showContextMenu(QPoint)));

    // Setup mouse click on torrent table view
    connect(ui->torrentsTable,
            SIGNAL(clicked(const QModelIndex &)),
            this,
            SLOT(torrentTableClicked(const QModelIndex &)));
            */

    // Accept drag and drop
    setAcceptDrops(true);

    /**
     * Wallet signals
     */

    // Update balance when it changes
    QObject::connect(_wallet,
                     SIGNAL(zeroConfBalanceChanged(quint64)),
                     this,
                     SLOT(updateWalletBalance(quint64)));

    // Capure wallet balance update timer,
    connect(&_walletBalanceUpdateTimer,
            SIGNAL(timeout()),
            this,
            SLOT(updateWalletBalanceHook()));

    // Do first round to setup initial value
    updateWalletBalance(_wallet->BLOCKCYPHER_lastAdress()._final_balance);

    // and have i fire every 30s
    _walletBalanceUpdateTimer.start(30000);
}

MainWindow::~MainWindow() {

    // Delete torrent view models
    //ui->peerPluginsTable->setModel(0); // make sure peer plugin table is not backed by a model

    for(QMap<libtorrent::sha1_hash, TorrentView *>::const_iterator
        i = _torrentViews.cbegin(),
        end = _torrentViews.cend();
        i != end;i++)
        delete i.value();

    // Delete Ui::MainWindow
    delete ui;
}

void MainWindow::showContextMenu(const QPoint & pos) {

    // Get model index of click
    QModelIndex index = ui->torrentsTable->indexAt(pos);

    // Get view
    TorrentView * view = rowToView(index.row());

    // Tell corresponding torrent view model to show context menu
    // turn into point in proper reference frame (is this really needed???)
    view->showContextMenu(ui->torrentsTable->viewport()->mapToGlobal(pos));
}

void MainWindow::showAddTorrentFromTorrentFileDialog(const QString & torrentFile) {

    // Show window for adding torrent with torrent file
    AddTorrentDialog addTorrentDialog(_controller, torrentFile, true);
    addTorrentDialog.exec();
}

void MainWindow::showAddTorrentFromMagnetLinkDialog(const QString & magnetLink) {

    // Show window for adding torrent with magnet link
    AddTorrentDialog addTorrentDialog(_controller, magnetLink, false);
    addTorrentDialog.exec();
}

void MainWindow::showAddTorrentPluginConfigurationDialog(const libtorrent::torrent_info & torrentInfo, const libtorrent::torrent_status & torrentStatus) {

    // Show window for adding torrent with magnet link
    QMessageBox msgBox;
    msgBox.setText("Choose plugin mode.");

    QPushButton * classicPushButton = msgBox.addButton(tr("Classic"), QMessageBox::ActionRole);
    //QPushButton * observerPushButton = msgBox.addButton(tr("Observer"), QMessageBox::ActionRole);
    QPushButton * buyerPushButton = msgBox.addButton(tr("Buy"), QMessageBox::ActionRole);
    QPushButton * sellerPushButton = msgBox.addButton(tr("Sell"), QMessageBox::ActionRole);

    // Show modal dialog on same thread, we block untill it is closed
    msgBox.exec();

    if(msgBox.clickedButton() == classicPushButton) {

    } /** else if (msgBox.clickedButton() == observerPushButton) {

        // Set in passive mode
        //_controller->updateTorrentPluginConfiguration(infoHash, new TorrentPluginConfiguration(true));

        qDebug() << "Not implemented.";

    } */ else if (msgBox.clickedButton() == buyerPushButton) {

        //Show buyer configuration dialog
        BuyerTorrentPluginConfigurationDialog buyerTorrentPluginConfigurationDialog(_controller, _wallet, torrentInfo, &_bitcoinDisplaySettings);
        buyerTorrentPluginConfigurationDialog.exec();

    } else if (msgBox.clickedButton() == sellerPushButton) {

        //Show seller configuration dialog
        SellerTorrentPluginConfigurationDialog sellerTorrentPluginConfigurationDialog(_controller, _wallet, torrentInfo, &_bitcoinDisplaySettings);
        sellerTorrentPluginConfigurationDialog.exec();
    }
}

void MainWindow::showTorrentPluginDialog(const libtorrent::sha1_hash & infoHash) {

    Q_ASSERT(_torrentViews.contains(infoHash));
    //Q_ASSERT(_torrentViewModels.contains(infoHash));

    // Grab model
    const TorrentViewModel * model = _controller->torrentViewModel(infoHash);// _torrentViewModels[infoHash];

    // Figure out which plugin type is installed
    PluginInstalled plugin = model->pluginInstalled();

    switch(plugin) {

        case PluginInstalled::Seller:
            showSellerTorrentPluginDialog(model->sellerTorrentPluginViewModel());
            break;

        case PluginInstalled::Buyer:
            showBuyerTorrentPluginDialog(model->buyerTorrentPluginViewModel());
            break;

        case PluginInstalled::Observer:
            Q_ASSERT(false);

        case PluginInstalled::None:
            Q_ASSERT(false);

    }
}

void MainWindow::showSellerTorrentPluginDialog(const SellerTorrentPluginViewModel * model) {

    // Create dialog
    SellerTorrentPluginDialog dialog(this, model, &_bitcoinDisplaySettings);

    // Show view
    dialog.exec();
}

void MainWindow::showBuyerTorrentPluginDialog(const BuyerTorrentPluginViewModel * model) {

    // Create dialog
    BuyerTorrentPluginDialog dialog(this, model , &_bitcoinDisplaySettings);

    // Show view
    dialog.exec();
}

void MainWindow::on_addTorrentFilePushButton_clicked()
{
    // Choose torrent file
    QString torrentFile = QFileDialog::getOpenFileName(this, tr("Open Torrent"), "C:/TORRENTS", tr("Torrent file (*.torrent)"));

    // Exit if no file chosen
    if(torrentFile.isNull())
        return;

    // Open dialog for adding torrent from file
    showAddTorrentFromTorrentFileDialog(torrentFile);
}

/**
void MainWindow::on_addMagnetLinkPushButton_clicked()
{
    // Input magnet link
    bool ok;
    QString magnetLink = QInputDialog::getText(this,
                                         tr("Add Magnet Link"),
                                         tr("Magnet link:"),
                                         QLineEdit::Normal,
                                         QString("magnet:?xt=urn:btih:781ad3adbd9b81b64e4c530712ae9199b1dfbae5&dn=Now+You+See+Me+%282013%29+1080p+EXTENDED+BrRip+x264+-+YIFY&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3"),
                                         &ok);

    // Exit if no link provided
    if (!ok || magnetLink.isEmpty())
        return;

    // Open dialog for adding torrent from magnet link
    showAddTorrentFromMagnetLinkDialog(magnetLink);
}
*/

void MainWindow::addTorrent(const TorrentViewModel * model) {

    // Create item
    // Will be taken by _torrentTableViewModel, and hence managed by it
    QStandardItem * nameItem = new QStandardItem(),
                  * sizeItem = new QStandardItem(),
                  * stateItem = new QStandardItem(),
                  * speedItem = new QStandardItem(),
                  * buyersItem = new QStandardItem(),
                  * sellersItem = new QStandardItem(),
                  * pluginInstalledItem = new QStandardItem(),
                  * balanceItem = new QStandardItem();

    // Create row for items
    QList<QStandardItem *> items;

    items << nameItem
          << sizeItem
          << stateItem
          << speedItem
          << buyersItem
          << sellersItem
          << pluginInstalledItem
          << balanceItem;

    // Add row to view model
    _torrentTableViewModel.appendRow(items);

    // Create torrent view model
    TorrentView * view = new TorrentView(this,
                                         model,
                                         &_bitcoinDisplaySettings,
                                         nameItem,
                                         sizeItem,
                                         stateItem,
                                         speedItem,
                                         buyersItem,
                                         sellersItem,
                                         pluginInstalledItem,
                                         balanceItem);

    // Connect: view menu action to main window slot
    QObject::connect(view,
                     SIGNAL(pauseTorrentRequested(libtorrent::sha1_hash)),
                     _controller,
                     SLOT(pauseTorrent(libtorrent::sha1_hash)));

    QObject::connect(view,
                     SIGNAL(startTorrentRequested(libtorrent::sha1_hash)),
                     _controller,
                     SLOT(startTorrent(libtorrent::sha1_hash)));

    QObject::connect(view,
                     SIGNAL(removeTorrentRequested(libtorrent::sha1_hash)),
                     _controller,
                     SLOT(removeTorrent(libtorrent::sha1_hash)));

    QObject::connect(view,
                     SIGNAL(requestedViewingExtension(libtorrent::sha1_hash)),
                     this,
                     SLOT(showTorrentPluginDialog(libtorrent::sha1_hash)));

    QObject::connect(view,
                     SIGNAL(requestedStreamingPlayback(libtorrent::sha1_hash)),
                     this,
                     SLOT(startVLC(libtorrent::sha1_hash)));

    // Insert into view map
    _torrentViews[model->infoHash()] = view;

    // Insert model into view map
    //_torrentViewModels[model->infoHash()] = model;

    // Insert view into row mapping
    _rowToInfoHash.append(model->infoHash());

    Q_ASSERT(_torrentViews.size() == _rowToInfoHash.size());
    //Q_ASSERT(_torrentViews.size() == _torrentViewModels.size());
}

void MainWindow::addTorrentFailed(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec) {

    // Do something more clever, this is just a hacky fix for now
    std::cerr << "Failed to add torrent " << name.c_str() << " because: " << ec.message().c_str() << std::endl;
}

/**
void MainWindow::startedSellerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration) {

    Q_ASSERT(_torrentViews.contains(infoHash));
    //_torrentViews[infoHash]->addPlugin(configuration);

}

void MainWindow::startedBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration & configuration, const UnspentP2PKHOutput & utxo) {

    Q_ASSERT(_torrentViews.contains(infoHash));
    //_torrentViews[infoHash]->addPlugin(configuration, utxo);


    //QObject::connect(torrentViewModel, SIGNAL(showSellerTorrentPluginClicked(const SellerTorrentPluginViewModel *)), _controller, SLOT(showSellerTorrentPluginDialog(const SellerTorrentPluginViewModel *)));
    //QObject::connect(torrentViewModel, SIGNAL(showBuyerTorrentPluginClicked(const BuyerTorrentPluginViewModel *)), _controller, SLOT(showBuyerTorrentPluginDialog(const BuyerTorrentPluginViewModel *)));
}

void MainWindow::addSellerPeerPlugin() {

}

void MainWindow::addBuyerPeerPlugin() {

}


void MainWindow::updateBuyerTorrentPluginStatus(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Status & status) {

    Q_ASSERT(_torrentViews.contains(infoHash));

    // Find corresponding TorrentViewModel
    TorrentView * view = _torrentViews[infoHash];

    Q_ASSERT(view->pluginInstalled() == PluginInstalled::Buyer);

    // Update view model of plugin
    view->update(status);
}

void MainWindow::updateSellerTorrentPluginStatus(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Status & status) {

    Q_ASSERT(_torrentViews.contains(infoHash));

    // Find corresponding view
    TorrentView * view = _torrentViews[infoHash];

    Q_ASSERT(view->pluginInstalled() == PluginInstalled::Seller);

    // Update view model of plugin
    view->update(status);
}
*/

/**
void MainWindow::startedTorrentPlugin(const libtorrent::sha1_hash & infoHash) {

    Q_ASSERT(_torrentViews.contains(infoHash));

    // Get view
    TorrentView * view = _torrentViews[infoHash];

    // Get view-model
    const TorrentViewModel * model = _controller->torrentViewModel(infoHash);

    Q_ASSERT(model != NULL);

    // Setup signals and slots
    PluginInstalled installed = model->pluginInstalled();

    if(installed == PluginInstalled::Buyer) {

        const BuyerTorrentPluginViewModel * buyerTorrentPluginViewModel = model->buyerTorrentPluginViewModel();



    } else if (installed == PluginInstalled::Seller) {

        const SellerTorrentPluginViewModel * sellerTorrentPluginViewModel = model->sellerTorrentPluginViewModel();


    }

}
*/

void MainWindow::updatePluginStatus(const Plugin::Status & status) {
    //ui->balanceLabel->setText(QString::number(p->balance()*1000) + "mBTC");

    QString stringSpentBalance = BitcoinRepresentation(status.totalSentSinceStart()).toString(&_bitcoinDisplaySettings);
    ui->spentBalanceLabel->setText(stringSpentBalance);

    QString stringEarnedBalance = BitcoinRepresentation(status.totalReceivedSinceStart()).toString(&_bitcoinDisplaySettings);
    ui->earnedBalanceLabel->setText(stringEarnedBalance);
}

void MainWindow::updateWalletBalance(quint64 balance) {
    //qCDebug() << "updateWalletBalance" << balance;

    QString stringBalance = BitcoinRepresentation(balance).toString(&_bitcoinDisplaySettings);

    ui->balanceLabel->setText(stringBalance);
}

void MainWindow::on_walletPushButton_clicked() {

    // Create dialog
    WalletDialog dialog(_wallet, &_bitcoinDisplaySettings);

    // Start processing new event loop
    dialog.exec();
}

void MainWindow::on_topUpWalletPushButton_clicked() {

    ReceiveFundsDialog dialog(_wallet);
    dialog.exec();

}

void MainWindow::on_updatePushButton_clicked() {

    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "Test", "Quit?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        qDebug() << "Yes was clicked";
    } else {
        qDebug() << "Yes was *not* clicked";
    }

    // Note:
    // To close application:
    // call slot (with signal or regular call) Controller::void Controller::begin_close.
    // It will start an asyncrhonous shut down process (primarily due to libtorrent at the moment).
    // When controller is done, it will emit a Controller::closed signal
    // which has already been connected to the QApplication::quit event which
    // stops the application event loop started in main.cpp
    // with the QApplication::exec()

}

void MainWindow::torrentTableClicked(const QModelIndex & index) {

    // Get torrent view mod el for torrent clicked on
    //TorrentViewModel * torrentViewModel = torrentViewModelInTableRow(index.row());
    //qCCritical() << "Clicked torrent with info hash"<< _rowToInfoHash[index.row()].to_string().c_str();
}

void MainWindow::startVLC(const libtorrent::sha1_hash & infoHash) {

    // Turn info hash into hex string
    std::string infoHashHexString = libtorrent::to_hex(infoHash.to_string());

    // Use to build path
    QString serverUrl = "http://localhost:" + QString::number(_controller->getServerPort()) + "/" + QString::fromStdString(infoHashHexString);

    QUrl url(serverUrl);

    QDesktopServices::openUrl(url);

    /**
    // Start VLC at local host on given port asking for this info hash
    qDebug() << "Starting VLC pointing at:" << url;

    // Escaped VLC path
    QString vlcExecutable = "\"C:/Program Files (x86)/VideoLAN/VLC/vlc.exe\"";

    // Launch VLC pointing at URL
    _processLauncher.start(vlcExecutable, QStringList() << url);

    // Check that it started
    if(_processLauncher.state() != QProcess::UnknownError)
        qDebug() << "Could not start VLC.";
    */
}

void MainWindow::updateWalletBalanceHook() {

    // Sync blockcypher wallet
    _wallet->BLOCKCYPHER_update_remote_wallet();

    // recalculate utxo (in case there has been any in/out to existing addresses);
    BlockCypher::Address addr = _wallet->BLOCKCYPHER_rebuild_utxo();

    // update balance
    updateWalletBalance(addr._final_balance);
}

TorrentView * MainWindow::rowToView(int row) {

    Q_ASSERT(row < _rowToInfoHash.size());
    Q_ASSERT(_torrentViews.contains(_rowToInfoHash[row]));

    return _torrentViews[_rowToInfoHash[row]];
}

void MainWindow::closeEvent(QCloseEvent * event) {

    // Notify controller
    _controller->begin_close();

    // But do not close, which causes event loop exit
    event->ignore();
}

void MainWindow::dropEvent(QDropEvent *e) {

    foreach (const QUrl &url, e->mimeData()->urls()) {
        const QString &fileName = url.toLocalFile();
        qDebug() << "Dropped file:" << fileName;
    }
}
