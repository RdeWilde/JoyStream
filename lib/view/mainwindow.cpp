#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include "AddTorrentDialog.hpp"
#include "WalletDialog.hpp"
#include "TorrentView.hpp"
#include "controller/SellerTorrentPluginViewModel.hpp"
#include "controller/BuyerTorrentPluginViewModel.hpp"
#include "TorrentViewModel.hpp"

// Dialogs
#include "SellerTorrentPluginConfigurationDialog.hpp"
#include "BuyerTorrentPluginConfigurationDialog.hpp"
#include "SellerTorrentPluginDialog.hpp"
#include "BuyerTorrentPluginDialog.hpp"

#include "controller/Controller.hpp"
#include "extension/TorrentPlugin.hpp"
#include "extension/PeerPlugin.hpp"
//#include "extension/Alert/TorrentPluginStatusAlert.hpp"
#include "extension/Alert/PluginStatusAlert.hpp"
//#include "extension/PeerPluginStatus.hpp"

//#include "extension/TorrentPluginConfiguration.hpp"
#include "extension/BuyerTorrentPlugin.hpp"
#include "extension/SellerTorrentPlugin.hpp"

#include "Utilities.hpp" // Utilities::PluginModeToPluginInstalled(PluginMode mode)

#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QInputDialog>
#include <QAction>
#include <QCloseEvent>

#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_handle.hpp>

MainWindow::MainWindow(Controller * controller, Wallet * wallet, QLoggingCategory & category)
    : ui(new Ui::MainWindow)
    , _controller(controller)
    , _wallet(wallet)
    , _torrentTableViewModel(0, 6)
    , _category(category) {

    ui->setupUi(this);

    // Alter window title
    QString categoryName = _category.categoryName();
    QString title;

    // Set title depending on if we are logging or not
    if(categoryName.compare("default"))
        title = QString("Logging: ") + categoryName;
    else
        title = "Proof of Concept 1";

    this->setWindowTitle(title);

    /**
     * Add columns to model view model
     */
    QStringList torrentTableColumnNames;

    torrentTableColumnNames << "Name"
                            << "Size"
                            << "State"
                            << "Peers"
                            << "Mode"
                            << "Balance";

    _torrentTableViewModel.setHorizontalHeaderLabels(torrentTableColumnNames);

    // Set table model to view model
    ui->torrentsTable->setModel(&_torrentTableViewModel);

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
}

MainWindow::~MainWindow() {

    // Delete torrent view models
    //ui->peerPluginsTable->setModel(0); // make sure peer plugin table is not backed by a model

    for(QMap<libtorrent::sha1_hash, TorrentViewModel *>::iterator i = _torrentViewModels.begin(),
        end(_torrentViewModels.end()); i != end;i++)
        delete i.value();

    // Delete Ui::MainWindow
    delete ui;
}

TorrentViewModel * MainWindow::torrentViewModelInTableRow(int row) {

    Q_ASSERT(row < _torrentInTableRow.size());

    // Find info hash of torrent
    libtorrent::sha1_hash infoHash = _torrentInTableRow[row];

    Q_ASSERT(_torrentViewModels.contains(infoHash));

    // Get torrent view model
    return _torrentViewModels[infoHash];
}

void MainWindow::showContextMenu(QPoint pos) {

    // Get model index of click
    QModelIndex index = ui->torrentsTable->indexAt(pos);

    // Tell corresponding torrent view model to show context menu
    torrentViewModelInTableRow(index.row())->showContextMenu(ui->torrentsTable->viewport()->mapToGlobal(pos));
}

void MainWindow::torrentTableClicked(const QModelIndex & index) {

    // Get torrent view model for torrent clicked on
    //TorrentViewModel * torrentViewModel = torrentViewModelInTableRow(index.row());

    qCCritical(_category) << "Clicked torrent with info hash"<< _torrentInTableRow[index.row()].to_string().c_str();
}

void MainWindow::showSellerTorrentPluginDialog(const SellerTorrentPluginViewModel * sellerTorrentPluginViewModel) {


}

void MainWindow::showBuyerTorrentPluginDialog(const BuyerTorrentPluginViewModel * buyerTorrentPluginViewModel) {

    /**
    // Create dialog
    BuyerTorrentPluginDialog dialog(buyerTorrentPluginViewModel);

    // Connect signals to main window and controller slots

    // Show view
    dialog.exec();
    */
}

void MainWindow::showAddTorrentFromTorrentFileDialog(const QString & torrentFile) {

    // Check that torrent file exists
    if(!QFile::exists(torrentFile)) {
        qCCritical(_category) << "Torrent file " << torrentFile.toStdString().c_str() << " does not exist.";
        return;
    }

    // Show window for adding torrent with torrent file
    /**
     * Starts new event loop,
     * no more libtorrent alerts are processed in mean time,
     * change at a later time
     */
    AddTorrentDialog addTorrentDialog(_controller, _category, torrentFile, true);
    addTorrentDialog.exec();
}

void MainWindow::showAddTorrentFromMagnetLinkDialog(const QString & magnetLink) {

    // Show window for adding torrent with magnet link
    /**
     * Starts new event loop,
     * no more libtorrent alerts are processed in mean time,
     * change at a later time
     */
    AddTorrentDialog addTorrentDialog(_controller, _category, magnetLink, false);
    addTorrentDialog.exec();
}

void MainWindow::showAddTorrentPluginConfigurationDialog(const libtorrent::torrent_info & torrentInfo, const libtorrent::torrent_status & torrentStatus) {

    // Show window for adding torrent with magnet link
    /**
     * Starts new event loop,
     * no more libtorrent alerts are processed in mean time,
     * change at a later time
     *
     * I think the statement above is incorrect, check later.
     */

    QMessageBox msgBox;
    msgBox.setText("Choose plugin mode.");

    QPushButton * passivePushButton = msgBox.addButton(tr("Observe"), QMessageBox::ActionRole);
    QPushButton * buyerPushButton = msgBox.addButton(tr("Buy"), QMessageBox::ActionRole);
    QPushButton * sellerPushButton = msgBox.addButton(tr("Sell"), QMessageBox::ActionRole);

    // Show modal dialog on same thread, we block untill it is closed
    msgBox.exec();

    if (msgBox.clickedButton() == passivePushButton) {

        // Set in passive mode
        //_controller->updateTorrentPluginConfiguration(infoHash, new TorrentPluginConfiguration(true));

        qCDebug(_category) << "Not implemented.";

    } else if (msgBox.clickedButton() == buyerPushButton) {

        //Show buyer configuration dialog
        BuyerTorrentPluginConfigurationDialog buyerTorrentPluginConfigurationDialog(_controller, _wallet, torrentInfo);
        buyerTorrentPluginConfigurationDialog.exec();

    } else if (msgBox.clickedButton() == sellerPushButton) {

        //Show seller configuration dialog
        SellerTorrentPluginConfigurationDialog sellerTorrentPluginConfigurationDialog(_controller, _wallet, torrentInfo);
        sellerTorrentPluginConfigurationDialog.exec();
    }
}

void MainWindow::on_addTorrentFilePushButton_clicked()
{
    // Choose torrent file
    QString torrentFile = QFileDialog::getOpenFileName(this, tr("Open Torrent"), "/home/jana", tr("Torrent file (*.torrent)"));

    // Exit if no file chosen
    if(torrentFile.isNull())
        return;

    // Open dialog for adding torrent from file
    showAddTorrentFromTorrentFileDialog(torrentFile);
}

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

void MainWindow::closeEvent(QCloseEvent * event) {

    // Notify controller
    _controller->begin_close();

    // But do not close, which causes event loop exit
    event->ignore();
}

void MainWindow::addTorrent(const libtorrent::sha1_hash & info_hash, const TorrentViewModel * model) {

    Q_ASSERT(!_torrentViewModels.contains(info_hash));

    // Create torrent view model
    TorrentView * view = new TorrentView(model, &_torrentTableViewModel);

    // Insert into map
    _torrentViewModels[info_hash] = view;
}

void MainWindow::addTorrentFailed(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec) {

    // Do something more clever, this is just a hacky fix for now
    std::cerr << "Failed to add torrent " << name.c_str() << " because: " << ec.message().c_str() << std::endl;
}

void MainWindow::removeTorrent(const libtorrent::sha1_hash & info_hash) {

    Q_ASSERT(_torrentViewModels.contains(info_hash));

    // Remove
    _torrentViewModels.remove(info_hash);

    // Reset model for peerPluginsTable,
    // which if this method call is due to a context menu click on torrent table
    // will have set the peerPluginsTable to its model.
    //ui->peerPluginsTable->setModel(0);
}

/**
void MainWindow::addTorrentPlugin(const libtorrent::sha1_hash & infoHash, PluginMode mode) {

    Q_ASSERT(_torrentViewModels.contains(infoHash));

    // Update
    switch(mode) {

        case PluginMode::Buyer: _torrentViewModels[infoHash]->addBuyerPlugin();
            break;

        case PluginMode::Seller: _torrentViewModels[infoHash]->addSellerPlugin();
            break;

        case PluginMode::Observer:
            Q_ASSERT(false);
            break;

        Q_ASSERT(false);
    }


}*/

void MainWindow::registerSellerTorrentPluginStarted(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration) {

    Q_ASSERT(_torrentViewModels.contains(infoHash));
    _torrentViewModels[infoHash]->addPlugin(configuration);

    //QObject::connect(torrentViewModel, SIGNAL(showSellerTorrentPluginClicked(const SellerTorrentPluginViewModel *)), _controller, SLOT(showSellerTorrentPluginDialog(const SellerTorrentPluginViewModel *)));
    //QObject::connect(torrentViewModel, SIGNAL(showBuyerTorrentPluginClicked(const BuyerTorrentPluginViewModel *)), _controller, SLOT(showBuyerTorrentPluginDialog(const BuyerTorrentPluginViewModel *)));
}

void MainWindow::registerBuyerTorrentPluginStarted(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration & configuration, const UnspentP2PKHOutput & utxo) {

    Q_ASSERT(_torrentViewModels.contains(infoHash));
    _torrentViewModels[infoHash]->addPlugin(configuration, utxo);

    //QObject::connect(torrentViewModel, SIGNAL(showSellerTorrentPluginClicked(const SellerTorrentPluginViewModel *)), _controller, SLOT(showSellerTorrentPluginDialog(const SellerTorrentPluginViewModel *)));
    //QObject::connect(torrentViewModel, SIGNAL(showBuyerTorrentPluginClicked(const BuyerTorrentPluginViewModel *)), _controller, SLOT(showBuyerTorrentPluginDialog(const BuyerTorrentPluginViewModel *)));
}

/**
void MainWindow::updateTorrentPluginStatus(const TorrentPluginStatusAlert * torrentPluginStatusAlert) {

    // Find corresponding TorrentViewModel
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator mapIterator = _torrentViewModels.find(torrentPluginStatusAlert->infoHash());

    if(mapIterator == _torrentViewModels.end()) {
        qCCritical(_category) << "No matching info_hash found.";
        return;
    }

    TorrentViewModel * torrentViewModel = mapIterator->second;

    // Peers
    torrentViewModel->updatePeers(torrentPluginStatusAlert->numberOfPeers(),torrentPluginStatusAlert->numberOfPeersWithExtension());

    // Mode
    torrentViewModel->updateMode(torrentPluginStatusAlert->pluginStarted(), torrentPluginStatusAlert->mode());

    // Balance
    torrentViewModel->updateBalance(torrentPluginStatusAlert->tokensReceived(), torrentPluginStatusAlert->tokensSent());
}
*/

void MainWindow::updateBuyerTorrentPluginStatus(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Status & status) {

    Q_ASSERT(_torrentViewModels.contains(infoHash));

    // Find corresponding TorrentViewModel
    TorrentViewModel * torrentViewModel = _torrentViewModels[infoHash];

    Q_ASSERT(torrentViewModel->pluginInstalled() == PluginInstalled::Buyer);

    // Update view model of plugin
    torrentViewModel->update(status);
}

void MainWindow::updateSellerTorrentPluginStatus(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Status & status) {

    Q_ASSERT(_torrentViewModels.contains(infoHash));

    // Find corresponding TorrentViewModel
    TorrentViewModel * torrentViewModel = _torrentViewModels[infoHash];

    Q_ASSERT(torrentViewModel->pluginInstalled() == PluginInstalled::Seller);

    // Update view model of plugin
    torrentViewModel->update(status);
}

void MainWindow::updatePluginStatus(const Plugin::Status & status) {
    //ui->balanceLabel->setText(QString::number(p->balance()*1000) + "mBTC");

    qCDebug(_category) << "MainWindow::updatePluginStatus()";
}

void MainWindow::updateWalletBalance(quint64 balance) {
    qCDebug(_category) << "updateWalletBalance" << balance;
}

void MainWindow::addPeerPlugin(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endPoint) {

    Q_ASSERT(_torrentViewModels.contains(info_hash));

    // Update
    //_torrentViewModels[info_hash]->addPeerPlugin(endPoint);
}

/**
void MainWindow::updatePeerPluginStatus(const PeerPluginStatus & status) {

    Q_ASSERT(_torrentViewModels.contains(status.peerPluginId_.info_hash_));

    // Update
    _torrentViewModels[status.peerPluginId_.info_hash_]->updatePeerPluginState(status);
}
*/

void MainWindow::removePeerPlugin(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endPoint) {
    qCDebug(_category) << "MainWindow::removePeer(): not implemented";
}

void MainWindow::on_walletPushButton_clicked() {

    // Create dialog
    WalletDialog dialog(_wallet);

    // Start processing new event loop
    dialog.exec();
}
