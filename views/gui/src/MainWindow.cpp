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
#include <gui/FundingWalletProgressDialog.hpp>
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
#include <bitcoin/SPVWallet.hpp>

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

MainWindow::MainWindow(Controller * controller, const QString & appendToTitle)
    : ui(new Ui::MainWindow)
    , _controller(controller)
    , _wallet(controller->wallet())
    , _torrentTableViewModel(0, 6)
    , _bitcoinDisplaySettings() //(Fiat::USD, 225) {
    , _openJoyStream("Open JoyStream", this)
    , _minimizeToTray("Minimize to tray", this)
    , _quit("Quit", this) {

    ui->setupUi(this);

    /**
     * Look of main windw
     */

    // Alter window title
    setWindowTitle("JoyStream" + appendToTitle);

    //setWindowIcon(QIcon(":/prefix/test") );

    // Freeze size of dialog
    setFixedSize(size());

    // Status bar
    _statusLabel.setText("Release v" + QString::number(APPLICATION_VERSION_MAJOR) + "." + QString::number(APPLICATION_VERSION_MINOR) + "." + QString::number(APPLICATION_VERSION_PATCH));
    _statusBar.addWidget(&_statusLabel);
    setStatusBar(&_statusBar);

    /**
     * buttons and menus
     */

    ui->walletPushButton->hide();
    ui->updatePushButton->hide();

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

    // earnedBalanceLabel
    QPalette earnedBalancePalette = ui->earnedBalanceLabel->palette();
    earnedBalancePalette.setColor(ui->earnedBalanceLabel->foregroundRole(), Qt::green);
    //palette.setColor(ui->pLabel->foregroundRole(), Qt::yellow);
    ui->earnedBalanceLabel->setPalette(earnedBalancePalette);
    ui->earnedTitleQlabel->setPalette(earnedBalancePalette);

    // spentBalanceLabel
    QPalette spentBalancePalette = ui->spentBalanceLabel->palette();
    spentBalancePalette.setColor(ui->spentBalanceLabel->foregroundRole(), Qt::red);
    //palette.setColor(ui->pLabel->foregroundRole(), Qt::yellow);
    ui->spentBalanceLabel->setPalette(spentBalancePalette);
    ui->spentTitleLabel->setPalette(spentBalancePalette);

    // unconfirmedBalanceLabel
    QPalette unconfirmedBalanceLabelPalette = ui->unconfirmedBalanceLabel->palette();

    unconfirmedBalanceLabelPalette.setColor(ui->unconfirmedBalanceLabel->foregroundRole(), Qt::magenta);
    ui->unconfirmedBalanceLabel->setPalette(unconfirmedBalanceLabelPalette);
    ui->unconfirmedBalanceTitleLabel->setPalette(unconfirmedBalanceLabelPalette);

    // balanceLabel
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
    //updateWalletBalances(_wallet->lastComputedZeroConfBalance());
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
    ui->unconfirmedBalanceLabel->setAlignment(Qt::AlignRight);
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
     */

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


    // Accept drag and drop
    setAcceptDrops(true);

    /**
     * Tray icon menu
     */

    _trayIconContextMenu.addAction(&_openJoyStream);
    _trayIconContextMenu.addAction(&_minimizeToTray);
    //_trayIconContextMenu.addSeparator();
    _trayIconContextMenu.addAction(&_quit);

    QIcon icon(":/icon/trayicon.png");
    _trayIcon.setIcon(icon);

    _trayIcon.setContextMenu(&_trayIconContextMenu);
    _trayIcon.setToolTip("JoyStream");
    _trayIcon.show();

    QObject::connect(&_openJoyStream,
                     SIGNAL(triggered()),
                     this,
                     SLOT(maximize()));

    QObject::connect(&_minimizeToTray,
                     SIGNAL(triggered()),
                     this,
                     SLOT(minimizeToTray()));

    QObject::connect(&_quit,
                     SIGNAL(triggered()),
                     this,
                     SLOT(quitApplication()));


    /**
     * Wallet signals
     */

    // Update balance when it changes
    qRegisterMetaType<uint64_t>("uint64_t");

/*
     QObject::connect(_controller->wallet(),
                     SIGNAL(balanceChanged(uint64_t, uint64_t)),
                     this,
                     SLOT(on_updatedWalletBalance(uint64_t, uint64_t)));
*/
    QObject::connect(_controller->wallet(),
                    &joystream::bitcoin::SPVWallet::balanceChanged,
                    this,
                    &MainWindow::on_updatedWalletBalance);

    // Wallet Synchronised
    QObject::connect(_controller->wallet(),
                     SIGNAL(synched()),
                     this,
                     SLOT(on_walletSynched()));

    // Show stating wallet balance
    updateWalletBalances(_wallet->balance(), _wallet->unconfirmedBalance());
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

    // Check if this was invalid click
    if(index.row() == -1)
        return;

    // Get view
    TorrentView * view = rowToView(index.row());

    // Tell corresponding torrent view model to show context menu
    // turn into point in proper reference frame (is this really needed???)
    view->showContextMenu(ui->torrentsTable->viewport()->mapToGlobal(pos));
}

void MainWindow::showAddTorrentFromTorrentFileDialog(const Controller::Torrent::Configuration & config) {

    // Show window for adding torrent with torrent file
    AddTorrentDialog addTorrentDialog(_controller, config);
    addTorrentDialog.exec();
}

void MainWindow::showAddTorrentFromMagnetLinkDialog(const Controller::Torrent::Configuration & config) {

    // Show window for adding torrent with magnet link
    AddTorrentDialog addTorrentDialog(_controller, config);
    addTorrentDialog.exec();
}

void MainWindow::showAddTorrentPluginConfigurationDialog(const libtorrent::torrent_info & torrentInfo, const libtorrent::torrent_status & torrentStatus) {

    libtorrent::torrent_status::state_t s = torrentStatus.state;

    // If fully downloaded, show seller configuration dialog
    if(s == libtorrent::torrent_status::state_t::seeding) {
        SellerTorrentPluginConfigurationDialog sellerTorrentPluginConfigurationDialog(_controller, _wallet, torrentInfo, &_bitcoinDisplaySettings);
        sellerTorrentPluginConfigurationDialog.exec();
    }
    // If fully downloaded, show seller configuration dialog
    else if (s == libtorrent::torrent_status::state_t::downloading) {
        BuyerTorrentPluginConfigurationDialog buyerTorrentPluginConfigurationDialog(_controller, _wallet, torrentInfo, &_bitcoinDisplaySettings);
        buyerTorrentPluginConfigurationDialog.exec();
    }
    else
        Q_ASSERT(false); //

    /*
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

      // } else if (msgBox.clickedButton() == observerPushButton) {
      //
      //  // Set in passive mode
      //  //_controller->updateTorrentPluginConfiguration(infoHash, new TorrentPluginConfiguration(true));
      //
      //  qDebug() << "Not implemented.";
      //
    }  else if (msgBox.clickedButton() == buyerPushButton) {

        //Show buyer configuration dialog
        BuyerTorrentPluginConfigurationDialog buyerTorrentPluginConfigurationDialog(_controller, _wallet, torrentInfo, &_bitcoinDisplaySettings);
        buyerTorrentPluginConfigurationDialog.exec();

    } else if (msgBox.clickedButton() == sellerPushButton) {

        //Show buyer configuration dialog
        SellerTorrentPluginConfigurationDialog sellerTorrentPluginConfigurationDialog(_controller, _wallet, torrentInfo, &_bitcoinDisplaySettings);
        sellerTorrentPluginConfigurationDialog.exec();

    }
    */
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
    BuyerTorrentPluginDialog dialog(this, model , &_bitcoinDisplaySettings, _wallet->network());

    // Show view
    dialog.exec();
}

void MainWindow::on_addTorrentFilePushButton_clicked()
{
    // Choose torrent file - start in application sample_torrents folder
    QString sample_torrents;
#ifdef _WIN32
    sample_torrents = QCoreApplication::applicationDirPath() + "/torrents/";
#elif __APPLE__
    sample_torrents = QCoreApplication::applicationDirPath() + "/../../../torrents";
#elif __linux
    sample_torrents = QCoreApplication::applicationDirPath() + "/../torrents";
#endif
    QString torrentFile = QFileDialog::getOpenFileName(this, tr("Open Torrent"), sample_torrents, tr("Torrent file (*.torrent)"));

    // Exit if no file chosen
    if(torrentFile.isNull())
        return;

    Controller::Torrent::Configuration config;

    try{
        // create configuration from torrent file
        config = Controller::Torrent::Configuration::fromTorrentFile(torrentFile);

    } catch(std::runtime_error e){
        //invalid torrent file
        return;
    }

    // use standard download path
    QString save_path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    // check path exists and is writable
    if(!save_path.isNull() && QFile::exists(save_path) && QFileInfo(save_path).isWritable()){
        config.setSavePath(save_path.toStdString());
        _controller->addTorrent(config);
    } else {
        // fallback to prompting user for a location
        showAddTorrentFromTorrentFileDialog(config);
    }

}

void MainWindow::on_addMagnetLinkPushButton_clicked()
{
    // Input magnet link
    bool ok;
    QString magnetLink = QInputDialog::getText(this,
                                         tr("Add Magnet Link"),
                                         tr("Magnet link:"),
                                         QLineEdit::Normal,
                                         //QString("magnet:?xt=urn:btih:781ad3adbd9b81b64e4c530712ae9199b1dfbae5&dn=Now+You+See+Me+%282013%29+1080p+EXTENDED+BrRip+x264+-+YIFY&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3"),
                                         QString(""),
                                         &ok);

    // Exit if no link provided
    if (!ok || magnetLink.isEmpty())
        return;

    Controller::Torrent::Configuration config;

    try{
        // create configuration from torrent file
        config = Controller::Torrent::Configuration::fromMagnetLink(magnetLink);

    } catch(std::runtime_error e){
        //invalid magnet link
        return;
    }

    // use standard download path
    QString save_path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    // check path exists and is writable
    if(!save_path.isNull() && QFile::exists(save_path) && QFileInfo(save_path).isWritable()){
        config.setSavePath(save_path.toStdString());
        _controller->addTorrent(config);
    } else {
        // Open dialog for adding torrent
        showAddTorrentFromMagnetLinkDialog(config);
    }
}

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

void MainWindow::updateWalletBalances(quint64 confirmedBalance, quint64 unconfirmedBalance) {
    //qCDebug() << "updateWalletBalance" << balance;

    QString stringConfirmedBalance = BitcoinRepresentation(confirmedBalance).toString(&_bitcoinDisplaySettings);
    ui->balanceLabel->setText(stringConfirmedBalance);

    QString stringUnconfirmedBalance = BitcoinRepresentation(unconfirmedBalance).toString(&_bitcoinDisplaySettings);
    ui->unconfirmedBalanceLabel->setText(stringUnconfirmedBalance);
}

void MainWindow::on_walletPushButton_clicked() {

    // Create dialog
    // WalletDialog dialog(_wallet, &_bitcoinDisplaySettings);

    // Start processing new event loop
    // dialog.exec();
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

void MainWindow::fundWalletFromFaucet() {

    //TODO - limit funding to once every hour? - check api limits

    qDebug() << "Topping up wallet from testnet faucet";

    FundingWalletProgressDialog dialog(RELOAD_WALLET_AMOUNT, &_bitcoinDisplaySettings);
    dialog.show();

    try {
        _controller->fundWallet(RELOAD_WALLET_AMOUNT);
    } catch(const std::exception & e) {
        qDebug() << "Unable to fund wallet from faucet";
    }

    dialog.hide();
}

void MainWindow::on_walletSynched() {

    updateWalletBalances(_wallet->balance(), _wallet->unconfirmedBalance());

    if(_wallet->unconfirmedBalance() < RELOAD_WALLET_LOWER_BOUND )
        fundWalletFromFaucet();
}

void MainWindow::on_updatedWalletBalance(uint64_t confirmedBalance, uint64_t unconfirmedBalance) {

    updateWalletBalances(confirmedBalance, unconfirmedBalance);

    //automatically top up wallet from testnet faucet if it goes below a threshold
    if (unconfirmedBalance < RELOAD_WALLET_LOWER_BOUND) {
        // TODO - check single largest utxo is less than the lower bound (not the total balance of the wallet)
        fundWalletFromFaucet();
    }
}

void MainWindow::maximize() {
    setWindowState(Qt::WindowState::WindowActive);
}

void MainWindow::minimizeToTray() {
    setWindowState(Qt::WindowState::WindowMinimized);
}

void MainWindow::quitApplication() {

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Quit JoyStream", "Are you sure you want to exit?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
        _controller->begin_close();
}

TorrentView * MainWindow::rowToView(int row) {

    if(row >= _rowToInfoHash.size())
        throw std::runtime_error("Index greater than number of rows.");

    Q_ASSERT(_torrentViews.contains(_rowToInfoHash[row]));

    return _torrentViews[_rowToInfoHash[row]];
}

void MainWindow::closeEvent(QCloseEvent * event) {

    // Notify controller
    _controller->begin_close();

    // Minimize window
    //setWindowState(Qt::WindowState::WindowMinimized);

    // But do not close, which causes event loop exit
    //event->ignore();
}

void MainWindow::dropEvent(QDropEvent *e) {

    foreach (const QUrl &url, e->mimeData()->urls()) {
        const QString &fileName = url.toLocalFile();
        qDebug() << "Dropped file:" << fileName;
    }
}

void MainWindow::on_bugsPushButton_clicked() {
    QDesktopServices::openUrl(QUrl("https://www.reddit.com/r/JoyStream/comments/3sazif/post_bugs_here/"));
}
