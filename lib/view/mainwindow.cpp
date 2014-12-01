#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include "AddTorrentDialog.hpp"
#include "TorrentViewModel.hpp"

#include "controller/Controller.hpp"
#include "extension/TorrentPlugin.hpp"
#include "extension/PeerPlugin.hpp"
#include "extension/TorrentPluginStatus.hpp"
#include "extension/PeerPluginStatus.hpp"

#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QInputDialog>
#include <QAction>
#include <QCloseEvent>

#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_handle.hpp>

MainWindow::MainWindow(Controller * controller, QLoggingCategory & category)
    : ui(new Ui::MainWindow)
    , torrentTableViewModel(new QStandardItemModel(0, 7, this)) //0 Rows and 4 Columns
    , controller_(controller)
    , torrentTableContextMenu(new QMenu(this))
    , paymentChannelsTableContextMenu(new QMenu(this))
    , category_(category)
{
    ui->setupUi(this);

    // Alter window title
    if(!QString(category_.categoryName()).compare("default")) {

        QString title = QString("Logging: ") + QString(category_.categoryName());
        this->setWindowTitle(title);
    }

    // Setup tableView
    torrentTableViewModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    torrentTableViewModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Size")));
    torrentTableViewModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Status")));
    torrentTableViewModel->setHorizontalHeaderItem(3, new QStandardItem(QString("Speed")));
    torrentTableViewModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Peers")));
    torrentTableViewModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Mode")));
    torrentTableViewModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Payment Channels")));
    torrentTableViewModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Balance")));
    ui->tableView->setModel(torrentTableViewModel);

    // Setup context menu capacity on table view
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView,
            SIGNAL(customContextMenuRequested(QPoint)),
            this,
            SLOT(customMenuRequested(QPoint)));

    // Setup mouse click on torrent table view
    connect(ui->tableView,
            SIGNAL(clicked(const QModelIndex &)),
            this,
            SLOT(torrentQTableViewClicked(const QModelIndex &)));

    // add menu buttons
    QAction * pauseAction = new QAction("Pause", this);
    connect(pauseAction, SIGNAL(triggered()), this, SLOT(pauseMenuAction()));
    torrentTableContextMenu->addAction(pauseAction);

    QAction * startAction = new QAction("Start", this);
    connect(startAction, SIGNAL(triggered()), this, SLOT(startMenuAction()));
    torrentTableContextMenu->addAction(startAction);

    QAction * removeAction = new QAction("Remove", this);
    connect(removeAction, SIGNAL(triggered()), this, SLOT(removeMenuAction()));
    torrentTableContextMenu->addAction(removeAction);
}

MainWindow::~MainWindow()
{
    // Delete Ui::MainWindow
    delete ui;

    // Delete everything in model, should be recursive
    // does it take ownership of actions, or do we need to delete them explicitly
    delete torrentTableViewModel;

    // Delete context menu
    // does it take ownership of actions, or do we need to delete them explicitly
    delete torrentTableContextMenu;

    /*
     *
     * DELET ALL THE VIEW MODEL STUFF HERE, DONT FORGET
     *
     *
     */

    // Delet stuff with payment channel tabel view models

    //Torrentview model vector
}

void MainWindow::customMenuRequested(QPoint pos) {

    // Figure out where user clicked
    torrentTableLastIndexClicked = ui->tableView->indexAt(pos);

    // Check that the clicked item is valid, e.g. if view is empty
    if(!torrentTableLastIndexClicked.isValid())
        return;

    // Show menu
    torrentTableContextMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void MainWindow::torrentQTableViewClicked(const QModelIndex & index) {

    // Switch model for payment channel table view
    ui->paymentChannelTableView->setModel(paymentChannelTableViewModels[index.row()]);
}

void MainWindow::pauseMenuAction() {

    // Get handle
    libtorrent::torrent_handle torrentHandle = getTorrentHandleLastClicked();

    // Notify controller
    // is it valid, i.e. was an actual match found?
    if(!torrentHandle.is_valid())
        qCDebug(category_) << "No match found!!";
    else
       controller_->pauseTorrent(torrentHandle);
}

void MainWindow::startMenuAction() {

    // Get handle
    libtorrent::torrent_handle torrentHandle = getTorrentHandleLastClicked();

    // Is it valid, i.e. was an actual match found?
    if(!torrentHandle.is_valid())
        qCDebug(category_) << "No match found!!";
    else
        controller_->startTorrent(torrentHandle);
}

void MainWindow::removeMenuAction() {

    // Remove from info hash container
    libtorrent::torrent_handle torrentHandle = getTorrentHandleLastClicked();

    // Notify controller to remove torrent
    controller_->removeTorrent(torrentHandle);
}

libtorrent::torrent_handle MainWindow::getTorrentHandleLastClicked() {

    // Get row
    int row = torrentTableLastIndexClicked.row();

    // Get info_hash
    libtorrent::sha1_hash & info_hash = infoHashInRow[row];

    // Get handle
    return controller_->getTorrentHandleFromInfoHash(info_hash);
    //return  libtorrent::torrent_handle(); // CHANGE LATER
}

void MainWindow::on_addTorrentFilePushButton_clicked()
{
    // Choose torrent file
    QString torrentFile = QFileDialog::getOpenFileName(this, tr("Open Torrent"), "/home/jana", tr("Torrent file (*.torrent)"));

    // Exit if no file chosen
    if(torrentFile.isNull())
        return;

    // Tell controller
    controller_->addTorrentFromTorrentFile(torrentFile);
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

    // Notify controller
    controller_->addTorrentFromMagnetLink(magnetLink);
}

void MainWindow::closeEvent(QCloseEvent * event) {

    // Notify controller
    controller_->begin_close();

    // But do not close, which causes event loop exit
    event->ignore();
}

void MainWindow::addTorrent(const libtorrent::sha1_hash & info_hash, const std::string & torrentName, int totalSize) {

    // Create torrent view model
    TorrentViewModel * torrentViewModel = new TorrentViewModel(info_hash);

    // Add view model
    torrentTableViewModel->appendRow(torrentViewModel->getModelAsQStandardItemRow());

    // Update known fields
    torrentViewModel->updateName(torrentName);
    torrentViewModel->updateSize(totalSize);

    // Add to torrent view model map
    torrentViewModels.insert(std::make_pair(info_hash, torrentViewModel));
}

void MainWindow::addTorrentFailed(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec) {

    // Do something more clever, this is just a hacky fix for now
    std::cerr << "Failed to add torrent " << name.c_str() << " because: " << ec.message().c_str() << std::endl;
}

void MainWindow::removeTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding TorrentViewModel
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator & mapIterator = torrentViewModels.find(info_hash);

    if(mapIterator == torrentViewModels.end()) {
        qCCritical(category_) << "No match info_hash found.";
        return;
    }

    TorrentViewModel * torrentViewModel = mapIterator->second();

    // Remove from torrentViewModels map
    torrentViewModels.erase(mapIterator);

    // Clear model for paymentChannelsTable
    ui->paymentChannelsTable->setModel(0);

    // Delete, also removes itself from torrentTableViewModel
    delete torrentViewModel;
}

void MainWindow::updateTorrentStatus(const std::vector<libtorrent::torrent_status> & torrentStatusVector) {

    // Iterate status vector, process each
    for(std::vector<libtorrent::torrent_status>::const_iterator i = torrentStatusVector.begin(),
        end(torrentStatusVector.end()); i != end;i++)
        updateTorrentStatus(*i);
}

void MainWindow::updateTorrentStatus(const libtorrent::torrent_status & torrentStatus) {

    // Find corresponding TorrentViewModel
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator & mapIterator = torrentViewModels.find(torrentStatus.info_hash);

    if(mapIterator == torrentViewModels.end()) {
        qCCritical(category_) << "No match info_hash found.";
        return;
    }

    TorrentViewModel * torrentViewModel = mapIterator->second();

    // Update
    torrentViewModel->update(torrentStatus);
}

void MainWindow::updateTorrentPluginStatus(TorrentPluginStatus status) {

    // Find corresponding TorrentViewModel
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator & mapIterator = torrentViewModels.find(status.info_hash_);

    if(mapIterator == torrentViewModels.end()) {
        qCCritical(category_) << "No match info_hash found.";
        return;
    }

    TorrentViewModel * torrentViewModel = mapIterator->second();

    // Peers
    torrentViewModel->updatePeers(status.numberOfPeers_, status.numberOfPeersWithExtension_);

    // Mode
    torrentViewModel->updateMode(status.pluginOn_);

    // Balance
    torrentViewModel->updateBalance(status.tokensReceived_, status.tokensSent_);
}

void MainWindow::addPaymentChannel(PeerPlugin * peerPlugin) {

    qCDebug(category_) << "addPaymentChannel()";

    // Find corresponding TorrentViewModel
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator & mapIterator = torrentViewModels.find(peerPlugin->getInfoHash());

    if(mapIterator == torrentViewModels.end()) {
        qCCritical(category_) << "No match info_hash found.";
        return;
    }

    TorrentViewModel * torrentViewModel = mapIterator->second();

    // Add channel for plugin
    torrentViewModel->addPaymentChannel(peerPlugin);
}

void MainWindow::updatePeerPluginStatus(PeerPluginStatus status) {

    qCDebug(category_) << "updatePeerPluginStatus()";

    // Find corresponding TorrentViewModel
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator & mapIterator = torrentViewModels.find(peerPlugin->getInfoHash());

    if(mapIterator == torrentViewModels.end()) {
        qCCritical(category_) << "No match info_hash found.";
        return;
    }

    // Update
    torrentViewModel->updatePaymentChannel(status);
}
