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
    , controller_(controller)
    , torrentTableContextMenu(new QMenu(this))
    , peerPluginsTableContextMenu(new QMenu(this))
    , category_(category)
{
    ui->setupUi(this);

    // Alter window title
    QString categoryName = category_.categoryName();
    QString title;

    // Set title depending on if we are logging or not
    if(categoryName.compare("default"))
        title = QString("Logging: ") + categoryName;
    else
        title = "BitSwapr - Proof of Concept 1";

    this->setWindowTitle(title);

    // Create table view model
    const char * columns[] = {"Name", "Size", "Status", "Speed", "Peers", "Mode", "#Channels", "Balance"};
    const int numberOfColumns = sizeof(columns)/sizeof(char *);

    torrentTableViewModel = new QStandardItemModel(0, numberOfColumns); // this no longer parent, does it matter?

    // Add columns to model
    for(int i = 0;i < numberOfColumns;i++)
        torrentTableViewModel->setHorizontalHeaderItem(i, new QStandardItem(columns[i]));

    // Set table model to view model
    ui->torrentsTable->setModel(torrentTableViewModel);

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

    // Add menu buttons
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

    // Delete torrent view models
    for(std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator i = torrentViewModels.begin(),
        end(torrentViewModels.end()); i != end;i++)
        delete i->second;
}

void MainWindow::showContextMenu(QPoint pos) {

    // Figure out where user clicked
    torrentTableLastIndexClicked = ui->torrentsTable->indexAt(pos);

    // Check that the clicked item is valid, e.g. if view is empty
    if(!torrentTableLastIndexClicked.isValid())
        return;

    // Show menu
    torrentTableContextMenu->popup(ui->torrentsTable->viewport()->mapToGlobal(pos));
}

void MainWindow::torrentTableClicked(const QModelIndex & index) {

    // Get torrent view model for torrent clicked on

    // Get item that was clicked
    QStandardItem * clickedItem = torrentTableViewModel->itemFromIndex(index);

    // Get torrent view stored in data field
    QVariant data = clickedItem->data();
    TorrentViewModel * torrentViewModel = data.value<TorrentViewModel *>();

    // Switch model for peer plugin table view
        ui->peerPluginsTable->setModel(torrentViewModel->getPeerPluginsTableViewModel());
}

void MainWindow::pauseMenuAction() {

    bool paused = controller_->pauseTorrent(getInfoHashOfLastClickedTorrent());

    // Torrent was actually paused, i.e. was an actual match found
    if(paused)
        qCDebug(category_) << "No info_hash match found.";
}

void MainWindow::startMenuAction() {

    bool started = controller_->startTorrent(getInfoHashOfLastClickedTorrent());

    // Torrent was actually started, i.e. was an actual match found
    if(!started)
        qCDebug(category_) << "No info_hash match found.";
}

void MainWindow::removeMenuAction() {

    bool removed = controller_->removeTorrent(getInfoHashOfLastClickedTorrent());

    // Torrent was actually started, i.e. was an actual match found
    if(!removed)
        qCDebug(category_) << "No info_hash match found.";
}

const libtorrent::sha1_hash & MainWindow::getInfoHashOfLastClickedTorrent() {

    // Get view item corresponding to index
    QStandardItem * torrentTableItem = torrentTableViewModel->itemFromIndex(torrentTableLastIndexClicked);

    // Get torrent view model for item
    QVariant variant = torrentTableItem->data();
    TorrentViewModel * torrentViewModel = variant.value<TorrentViewModel *>();

    return torrentViewModel->getInfoHash();
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

void MainWindow::addTorrent(const libtorrent::sha1_hash & info_hash, const QString & torrentName, int totalSize) {

    // Create torrent view model
    TorrentViewModel * torrentViewModel = new TorrentViewModel(info_hash, torrentTableViewModel, category_);

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
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator mapIterator = torrentViewModels.find(info_hash);

    if(mapIterator == torrentViewModels.end()) {
        qCCritical(category_) << "No match info_hash found.";
        return;
    }

    TorrentViewModel * torrentViewModel = mapIterator->second;

    // Remove from torrentViewModels map
    torrentViewModels.erase(mapIterator);

    // Clear model for peerPluginsTable
    ui->peerPluginsTable->setModel(0);

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
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator mapIterator = torrentViewModels.find(torrentStatus.info_hash);

    if(mapIterator == torrentViewModels.end()) {
        qCCritical(category_) << "No match info_hash found.";
        return;
    }

    TorrentViewModel * torrentViewModel = mapIterator->second;

    // Update
    torrentViewModel->update(torrentStatus);
}

void MainWindow::updateTorrentPluginStatus(TorrentPluginStatus status) {

    // Find corresponding TorrentViewModel
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator mapIterator = torrentViewModels.find(status.info_hash_);

    if(mapIterator == torrentViewModels.end()) {
        qCCritical(category_) << "No matching info_hash found.";
        return;
    }

    TorrentViewModel * torrentViewModel = mapIterator->second;

    // Peers
    torrentViewModel->updatePeers(status.numberOfPeers_, status.numberOfPeersWithExtension_);

    // Mode
    torrentViewModel->updateMode(status.pluginOn_);

    // Balance
    torrentViewModel->updateBalance(status.tokensReceived_, status.tokensSent_);
}

void MainWindow::addPeerPlugin(PeerPlugin * peerPlugin) {

    qCDebug(category_) << "addPeerPlugin()";

    // Find corresponding TorrentViewModel
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator mapIterator = torrentViewModels.find(peerPlugin->getInfoHash());

    if(mapIterator == torrentViewModels.end()) {
        qCCritical(category_) << "No match info_hash found.";
        return;
    }

    TorrentViewModel * torrentViewModel = mapIterator->second;

    // Add plugin
    torrentViewModel->addPeerPlugin(peerPlugin);
}

void MainWindow::updatePeerPluginStatus(PeerPluginStatus status) {

    qCDebug(category_) << "updatePeerPluginStatus()";

    // Find corresponding TorrentViewModel
    libtorrent::sha1_hash info_hash = status.peerPlugin_->getInfoHash();
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator mapIterator = torrentViewModels.find(info_hash);

    if(mapIterator == torrentViewModels.end()) {
        qCCritical(category_) << "No match info_hash found.";
        return;
    }

    TorrentViewModel * torrentViewModel = mapIterator->second;

    // Update
    torrentViewModel->updatePeerPluginState(status);
}
