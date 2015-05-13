#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include "AddTorrentDialog.hpp"
#include "WalletDialog.hpp"
#include "TorrentView.hpp"
#include "controller/SellerTorrentPluginViewModel.hpp"
#include "controller/BuyerTorrentPluginViewModel.hpp"
#include "controller/TorrentViewModel.hpp"

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
        title = "POC I";

    this->setWindowTitle(title);

    /**
     * Add columns to model view model
     */
    QStringList columnNames;

    columnNames << "Name"
                << "Size"
                << "State"
                << "Peers"
                << "Mode"
                << "Balance";

    _torrentTableViewModel.setHorizontalHeaderLabels(columnNames);

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
    AddTorrentDialog addTorrentDialog(_controller, _category, torrentFile, true);
    addTorrentDialog.exec();
}

void MainWindow::showAddTorrentFromMagnetLinkDialog(const QString & magnetLink) {

    // Show window for adding torrent with magnet link
    AddTorrentDialog addTorrentDialog(_controller, _category, magnetLink, false);
    addTorrentDialog.exec();
}

void MainWindow::showAddTorrentPluginConfigurationDialog(const libtorrent::torrent_info & torrentInfo, const libtorrent::torrent_status & torrentStatus) {

    // Show window for adding torrent with magnet link
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

void MainWindow::showTorrentPluginDialog(const libtorrent::sha1_hash & infoHash) {

    Q_ASSERT(_torrentViews.contains(infoHash));
    Q_ASSERT(_torrentViewModels.contains(infoHash));

    // Grab model
    TorrentViewModel * model = _torrentViewModels[infoHash];

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


}

void MainWindow::showBuyerTorrentPluginDialog(const BuyerTorrentPluginViewModel * model) {

    /**
    // Create dialog
    BuyerTorrentPluginDialog dialog(buyerTorrentPluginViewModel);

    // Connect signals to main window and controller slots

    // Show view
    dialog.exec();
    */
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

void MainWindow::addTorrent(const TorrentViewModel * model) {

    // Create torrent view model
    TorrentView * view = new TorrentView(model->status(),
                                         model->torrentInfo()->total_size(),
                                         model->pluginInstalled(),
                                         &_torrentTableViewModel);

    // Connect: model signals to view slots
    QObject::connect(model,
                     SIGNAL(pluginInstalledChanged(PluginInstalled)),
                     view,
                     SLOT(updatePluginInstalled(PluginInstalled)));

    QObject::connect(model,
                     SIGNAL(torrentStatusChanged(const libtorrent::torrent_status &)),
                     view,
                     SLOT(updateStatus(const libtorrent::torrent_status &)));

    // Connect: view menu actions to model slots
    QObject::connect(view->pauseAction(),
                     SIGNAL(triggered()),
                     model,
                     SLOT(pause()));

    QObject::connect(view->startAction(),
                     SIGNAL(triggered()),
                     model,
                     SLOT(start()));

    QObject::connect(view->removeAction(),
                     SIGNAL(triggered()),
                     model,
                     SLOT(remove()));

    // Connect: view menu action to main window slot
    QObject::connect(view,
                     SIGNAL(requestedViewingExtension(libtorrent::sha1_hash)),
                     this,
                     SLOT(showTorrentPluginDialog(libtorrent::sha1_hash)));

    // Insert into view map
    _torrentViews[model->infoHash()] = view;

    // Insert model into view map
    _torrentViewModels[model->infoHash()] = model;

    // Insert view into row mapping
    _rowToInfoHash.append(model->infoHash());

    Q_ASSERT(_torrentViews.size() == _rowToInfoHash.size());
    Q_ASSERT(_torrentViews.size() == _torrentViewModels.size());
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

void MainWindow::updatePluginStatus(const Plugin::Status & status) {
    //ui->balanceLabel->setText(QString::number(p->balance()*1000) + "mBTC");

    qCDebug(_category) << "MainWindow::updatePluginStatus()";
}

void MainWindow::updateWalletBalance(quint64 balance) {
    qCDebug(_category) << "updateWalletBalance" << balance;
}

void MainWindow::on_walletPushButton_clicked() {

    // Create dialog
    WalletDialog dialog(_wallet);

    // Start processing new event loop
    dialog.exec();
}

void MainWindow::torrentTableClicked(const QModelIndex & index) {

    // Get torrent view model for torrent clicked on
    //TorrentViewModel * torrentViewModel = torrentViewModelInTableRow(index.row());
    //qCCritical(_category) << "Clicked torrent with info hash"<< _rowToInfoHash[index.row()].to_string().c_str();
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
