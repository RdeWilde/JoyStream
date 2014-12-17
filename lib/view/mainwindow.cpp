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
    , _controller(controller)
    , _torrentTableViewModel(0, TorrentViewModel::numberOfColumns)
    , _torrentTableContextMenu(new QMenu(this))
    , _peerPluginsTableContextMenu(new QMenu(this))
    , _category(category)
{
    ui->setupUi(this);

    // Alter window title
    QString categoryName = _category.categoryName();
    QString title;

    // Set title depending on if we are logging or not
    if(categoryName.compare("default"))
        title = QString("Logging: ") + categoryName;
    else
        title = "BitSwapr - Proof of Concept 1";

    this->setWindowTitle(title);

    // Add columns to model view model
    for(int i = 0;i < TorrentViewModel::numberOfColumns;i++)
        _torrentTableViewModel.setHorizontalHeaderItem(i, new QStandardItem(TorrentViewModel::columnTitles[i]));

    // Set table model to view model
    ui->torrentsTable->setModel(&_torrentTableViewModel);

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
    _torrentTableContextMenu->addAction(pauseAction);

    QAction * startAction = new QAction("Start", this);
    connect(startAction, SIGNAL(triggered()), this, SLOT(startMenuAction()));
    _torrentTableContextMenu->addAction(startAction);

    QAction * removeAction = new QAction("Remove", this);
    connect(removeAction, SIGNAL(triggered()), this, SLOT(removeMenuAction()));
    _torrentTableContextMenu->addAction(removeAction);
}

MainWindow::~MainWindow()
{
    // Delete torrent view models
    ui->peerPluginsTable->setModel(0); // make sure peer plugin table is not backed by a model

    for(std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator i = _torrentViewModels.begin(),
        end(_torrentViewModels.end()); i != end;i++)
        delete i->second;

    // Delete context menus
    // Do thet take ownership of actions, or do we need to delete them explicitly?
    delete _torrentTableContextMenu;
    delete _peerPluginsTableContextMenu;

    // Delete Ui::MainWindow
    delete ui;
}

void MainWindow::showContextMenu(QPoint pos) {

    // Figure out where user clicked
    _torrentTableLastIndexClicked = ui->torrentsTable->indexAt(pos);

    // Check that the clicked item is valid, e.g. if view is empty
    if(!_torrentTableLastIndexClicked.isValid())
        return;

    // Show menu
    _torrentTableContextMenu->popup(ui->torrentsTable->viewport()->mapToGlobal(pos));
}

void MainWindow::torrentTableClicked(const QModelIndex & index) {

    // Get torrent view model for torrent clicked on

    // Get item that was clicked
    QStandardItem * clickedItem = _torrentTableViewModel.itemFromIndex(index);

    // Get torrent view stored in data field
    QVariant data = clickedItem->data();
    TorrentViewModel * torrentViewModel = data.value<TorrentViewModel *>();

    // Switch model for peer plugin table view
    ui->peerPluginsTable->setModel(torrentViewModel->getPeerPluginsTableViewModel());
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

void MainWindow::showAddTorrentPluginConfigurationDialog(const libtorrent::torrent_info & torrent_info) {

    // Show window for adding torrent with magnet link
    /**
     * Starts new event loop,
     * no more libtorrent alerts are processed in mean time,
     * change at a later time
     */
    //AddTorrentDialog addTorrentDialog(_controller, _category, magnetLink, false);
    //addTorrentDialog.exec();


}

void MainWindow::pauseMenuAction() {

    bool paused = _controller->pauseTorrent(getInfoHashOfLastClickedTorrent());

    // Torrent was actually paused, i.e. was an actual match found
    if(paused)
        qCDebug(_category) << "No info_hash match found.";
}

void MainWindow::startMenuAction() {

    bool started = _controller->startTorrent(getInfoHashOfLastClickedTorrent());

    // Torrent was actually started, i.e. was an actual match found
    if(!started)
        qCDebug(_category) << "No info_hash match found.";
}

void MainWindow::removeMenuAction() {

    bool removed = _controller->removeTorrent(getInfoHashOfLastClickedTorrent());

    // Torrent was actually started, i.e. was an actual match found
    if(!removed)
        qCDebug(_category) << "No info_hash match found.";
}

const libtorrent::sha1_hash & MainWindow::getInfoHashOfLastClickedTorrent() {

    // Get view item corresponding to index
    QStandardItem * torrentTableItem = _torrentTableViewModel.itemFromIndex(_torrentTableLastIndexClicked);

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

void MainWindow::addTorrent(const libtorrent::sha1_hash & info_hash, const QString & torrentName, int totalSize) {

    // Create torrent view model
    TorrentViewModel * torrentViewModel = new TorrentViewModel(info_hash, _torrentTableViewModel, _category);

    // Update known members
    torrentViewModel->updateName(torrentName);
    torrentViewModel->updateSize(totalSize);

    // Insert into map
    _torrentViewModels.insert(std::make_pair(info_hash, torrentViewModel));
}

void MainWindow::addTorrentFailed(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec) {

    // Do something more clever, this is just a hacky fix for now
    std::cerr << "Failed to add torrent " << name.c_str() << " because: " << ec.message().c_str() << std::endl;
}

void MainWindow::removeTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding TorrentViewModel
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator mapIterator = _torrentViewModels.find(info_hash);

    if(mapIterator == _torrentViewModels.end()) {
        qCCritical(_category) << "No match info_hash found.";
        return;
    }

    // Reset model for peerPluginsTable,
    // which if this method call is due to a context menu click on torrent table
    // will have set the peerPluginsTable to its model.
    ui->peerPluginsTable->setModel(0);

    // Remove from map, which will call destuctor
    _torrentViewModels.erase(mapIterator);
}

void MainWindow::updateTorrentStatus(const std::vector<libtorrent::torrent_status> & torrentStatusVector) {

    // Iterate status vector, process each
    for(std::vector<libtorrent::torrent_status>::const_iterator i = torrentStatusVector.begin(),
        end(torrentStatusVector.end()); i != end;i++)
        updateTorrentStatus(*i);
}

void MainWindow::updateTorrentStatus(const libtorrent::torrent_status & torrentStatus) {

    // Find corresponding TorrentViewModel
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator mapIterator = _torrentViewModels.find(torrentStatus.info_hash);

    if(mapIterator == _torrentViewModels.end()) {
        qCCritical(_category) << "No match info_hash found.";
        return;
    }

    TorrentViewModel * torrentViewModel = mapIterator->second;

    // Update
    torrentViewModel->update(torrentStatus);
}

void MainWindow::updateTorrentPluginStatus(TorrentPluginStatus status) {

    // Find corresponding TorrentViewModel
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator mapIterator = _torrentViewModels.find(status.info_hash_);

    if(mapIterator == _torrentViewModels.end()) {
        qCCritical(_category) << "No matching info_hash found.";
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

void MainWindow::addPeerPlugin(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endPoint) {

    qCDebug(_category) << "addPeerPlugin()";

    // Find corresponding TorrentViewModel
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator mapIterator = _torrentViewModels.find(info_hash);

    if(mapIterator == _torrentViewModels.end()) {
        qCCritical(_category) << "No match info_hash found.";
        return;
    }

    // Get corresponding torrent view model
    TorrentViewModel * torrentViewModel = mapIterator->second;

    // Add plugin
    torrentViewModel->addPeerPlugin(endPoint);
}

void MainWindow::updatePeerPluginStatus(PeerPluginStatus status) {

    qCDebug(_category) << "updatePeerPluginStatus()";

    // Find corresponding TorrentViewModel
    std::map<libtorrent::sha1_hash, TorrentViewModel *>::iterator mapIterator = _torrentViewModels.find(status.peerPluginId_.info_hash_);

    if(mapIterator == _torrentViewModels.end()) {
        qCCritical(_category) << "No match info_hash found.";
        return;
    }

    TorrentViewModel * torrentViewModel = mapIterator->second;

    // Update
    torrentViewModel->updatePeerPluginState(status);
}

void MainWindow::removePeerPlugin(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endPoint) {
    qCDebug(_category) << "MainWindow::removePeer(): not implemented";
}
