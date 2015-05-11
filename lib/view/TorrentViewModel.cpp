#include "TorrentViewModel.hpp"
#include "peerPluginViewModel.hpp"
#include "extension/PeerPlugin.hpp"
#include "controller/Controller.hpp"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>

#include <libtorrent/socket_io.hpp>

const char * TorrentViewModel::columnTitles[] = {"Name", "Size", "Status", "Speed", "Peers", "Mode", "#Channels", "Balance"};
const int TorrentViewModel::numberOfColumns = sizeof(TorrentViewModel::columnTitles)/sizeof(char *);

TorrentViewModel::TorrentViewModel(const libtorrent::sha1_hash & infoHash,
                                   //Controller * controller,
                                   QStandardItemModel * torrentTableViewModel)
    : _infoHash(infoHash)
    //, _controller(controller)
    //, _mainWindow(mainWindow)
    //, _peerPluginsTableViewModel(0, PeerPluginViewModel::numberOfColumns)
    , _nameItem(new QStandardItem())
    , _sizeItem(new QStandardItem())
    , _stateItem(new QStandardItem())
    , _speedItem(new QStandardItem())
    , _peersItem(new QStandardItem())
    , _modeItem(new QStandardItem())
    , _peerPluginsItem(new QStandardItem())
    , _balanceItem(new QStandardItem())
    , _torrentTableContextMenu()
    , _pause("Pause", this)
    , _start("Start", this)
    , _remove("Remove", this)
    , _pluginInstalled(PluginInstalled::None)
    , _viewExtension("Extension", this)
    , _sellerTorrentPluginViewModel(NULL)
    , _buyerTorrentPluginViewModel(NULL) {

    // Add as row to torrentTableViewModel
    QList<QStandardItem *> row;

    row.append(_nameItem);
    row.append(_sizeItem);
    row.append(_stateItem);
    row.append(_speedItem);
    row.append(_peersItem);
    row.append(_modeItem);
    row.append(_peerPluginsItem);
    row.append(_balanceItem);

    torrentTableViewModel->appendRow(row);

    // Add menu buttons
    QObject::connect(&_pause, SIGNAL(triggered()), this, SLOT(pauseMenuAction()));
    _torrentTableContextMenu.addAction(&_pause);

    QObject::connect(&_start, SIGNAL(triggered()), this, SLOT(startMenuAction()));
    _torrentTableContextMenu.addAction(&_start);

    QObject::connect(&_remove, SIGNAL(triggered()), this, SLOT(removeMenuAction()));
    _torrentTableContextMenu.addAction(&_remove);

    QObject::connect(&_viewExtension, SIGNAL(triggered()), this, SLOT(viewExtensionMenuAction()));
    // add action to menu only when plugin is installed
}

TorrentViewModel::~TorrentViewModel() {
}

void TorrentViewModel::addSellerPlugin(const SellerTorrentPlugin::Configuration & configuration) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::None);
    Q_ASSERT(_sellerTorrentPluginViewModel == NULL);
    Q_ASSERT(_buyerTorrentPluginViewModel == NULL);

    // Add extension menu button
    _torrentTableContextMenu.addAction(&_viewExtension);

    // Create view model for plugin
    //_sellerTorrentPluginViewModel = new SellerTorrentPluginViewModel(this, _infoHash, configuration);

    // Update mode field
    updatePluginInstalled(PluginInstalled::Seller);
}

void TorrentViewModel::addBuyerPlugin(const BuyerTorrentPlugin::Configuration & configuration, const UnspentP2PKHOutput & utxo) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::None);
    Q_ASSERT(_sellerTorrentPluginViewModel == NULL);
    Q_ASSERT(_buyerTorrentPluginViewModel == NULL);

    // Add extension menu button
    _torrentTableContextMenu.addAction(&_viewExtension);

    // Create view model for plugin
    //_buyerTorrentPluginViewModel = new BuyerTorrentPluginViewModel(this, configuration, utxo);

    // Update mode field
    updatePluginInstalled(PluginInstalled::Buyer);
}

void TorrentViewModel::pauseMenuAction() {

    // Pause torrent
    emit pauseTorrentClicked(_infoHash);

    /**
    // Pause torrent
    bool paused = _controller->pauseTorrent(_infoHash);

    // Torrent was actually paused, i.e. was an actual match found
    //if(paused)
    //    qCDebug() << "Invalid torrent handle found.";
    */
}

void TorrentViewModel::startMenuAction() {

    // Start torrent
    emit startTorrentClicked(_infoHash);

    /**
    // Start torrent
    bool started = _controller->startTorrent(_infoHash);

    // Torrent was actually started, i.e. was an actual match found
   // if(!started)
    //    qCDebug() << "Invalid torrent handle found.";
    */
}

void TorrentViewModel::removeMenuAction() {

    // Remove torrent
    emit removeTorrentClicked(_infoHash);

    /**
    // Remove torrent
    bool removed = _controller->removeTorrent(_infoHash);

    // Torrent was actually started, i.e. was an actual match found
    //if(!removed)
   //     qCDebug() << "Invalid torrent handle found.";
   */
}

void TorrentViewModel::viewExtensionMenuAction() {

    Q_ASSERT(_pluginInstalled != PluginInstalled::None);

    switch(_pluginInstalled) {

        case PluginInstalled::Buyer:

            emit showSellerTorrentPluginClicked(_sellerTorrentPluginViewModel);
            break;

        case PluginInstalled::Seller:

            emit showBuyerTorrentPluginClicked(_buyerTorrentPluginViewModel);
            break;

        case PluginInstalled::Observer:
            Q_ASSERT(false);
            break;

        case PluginInstalled::None:
            Q_ASSERT(false);
    }
}

void TorrentViewModel::update(const libtorrent::torrent_status & torrentStatus) {

    // State
    updateState(torrentStatus.paused, torrentStatus.state, torrentStatus.progress);

    // Up/Down rates
    updateSpeed(torrentStatus.download_rate, torrentStatus.upload_rate);
}

void TorrentViewModel::updateName(const QString & name) {
    _nameItem->setText(name);
}

void TorrentViewModel::updateSize(int size) {
    _sizeItem->setText(QString::number(size));
}

void TorrentViewModel::updateState(bool paused, libtorrent::torrent_status::state_t state, float progress) {

    // State
    QString itemText;

    if(paused)
        itemText = "Paused";
    else {

        // .progress reports the pogress of the relevant task, but I suspect it is being used
        // some what incorrectly in som eof the cases below.

        switch(state) {

            case libtorrent::torrent_status::queued_for_checking:

                itemText = "Queued for checking";

                break;
            case libtorrent::torrent_status::checking_files:

                itemText = "Checking files";
                itemText += " " + QString::number(progress*100,'f',0).append("%");
                break;
            case libtorrent::torrent_status::downloading_metadata:

                itemText = "Downloading metadata";
                itemText += " " + QString::number(progress*100,'f',0).append("%");
                break;
            case libtorrent::torrent_status::downloading:

                itemText = "Downloading";
                itemText += " " + QString::number(progress*100,'f',0).append("%");
                break;
            case libtorrent::torrent_status::finished:

                itemText = "Finished";
                break;
            case libtorrent::torrent_status::seeding:

                itemText = "Seeding";
                break;
            case libtorrent::torrent_status::allocating:

                itemText = "Allocating";
                itemText += " " + QString::number(progress*100,'f',0).append("%");
                break;
            case libtorrent::torrent_status::checking_resume_data:

                itemText = "Checking resume data";
                itemText += " " + QString::number(progress*100,'f',0).append("%");
                break;
        }

    }

    // Update item
    _stateItem->setText(itemText);
}

void TorrentViewModel::updateSpeed(int downloadRate, int uploadRate) {

    // Speed
    QString speed;

    speed.append(QString::number(downloadRate/1024))
         .append("Kb/s | ")
         .append(QString::number(uploadRate/1024))
         .append("Kb/s");

    // Update item
    _speedItem->setText(speed);
}

void TorrentViewModel::updatePeers(int numberOfPeers, int numberOfPeersWithExtension) {

    QString peers = QString::number(numberOfPeers) + "|" + QString::number(numberOfPeersWithExtension);

    // Update item
    _peersItem->setText(peers);
}

void TorrentViewModel::updatePluginInstalled(PluginInstalled mode) {

    QString modeText;

    switch(mode) {
        case PluginInstalled::Observer :
            modeText = "Observe";
            break;
        case PluginInstalled::Buyer:
            modeText = "Buyer";
            break;
        case PluginInstalled::Seller:
            modeText = "Seller";
            break;
        case PluginInstalled::None:
            modeText = "None";
            break;
    }

    // Update item
    _modeItem->setText(modeText);
}

void TorrentViewModel::updateBalance(int tokensReceived, int tokensSent) {

    QString balance = QString::number(tokensReceived) + "|" + QString::number(tokensSent);

    // Update item
    _balanceItem->setText(balance);
}

void TorrentViewModel::update(const BuyerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::Buyer);
    Q_ASSERT(_sellerTorrentPluginViewModel == NULL);
    Q_ASSERT(_buyerTorrentPluginViewModel != NULL);

    _buyerTorrentPluginViewModel->update(status);
}

void TorrentViewModel::update(const SellerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::Seller);
    Q_ASSERT(_sellerTorrentPluginViewModel != NULL);
    Q_ASSERT(_buyerTorrentPluginViewModel == NULL);

    _sellerTorrentPluginViewModel->update(status);
}

void TorrentViewModel::showContextMenu(QPoint pos) {
    _torrentTableContextMenu.popup(pos);
}

/**
void TorrentViewModel::addPeerPlugin(const libtorrent::tcp::endpoint & endPoint) {

    //const libtorrent::tcp::endpoint & endPoint = peerPlugin->getEndPoint();

    // Add to map
    _peerPluginViewModels[endPoint] = new PeerPluginViewModel(endPoint, _peerPluginsTableViewModel);
}

void TorrentViewModel::removePeerPlugin(const libtorrent::tcp::endpoint & endPoint) {

}


void TorrentViewModel::updatePeerPluginState(PeerPluginStatus status) {

    //
    // If extension is not enabled, then it should not be registered with us.
    // If extension is enabled, it may still be it is not registered here,
    // simply because peerAdded() signal has not been processed yet.
    //
    // DISABLE THIS CHECK UNTIL THE MESSAGE LOGIC IS SORTED OUT
    // if(status.peerPlugin_->getPeerBEP43SupportedStatus() != PeerPlugin::PEER_BEP_SUPPORTED_STATUS::supported)
    //    return;

    Q_ASSERT(_peerPluginViewModels.contains(status.peerPluginId_.endPoint_));

    // Update
    _peerPluginViewModels[status.peerPluginId_.endPoint_]->update(status);
}


libtorrent::sha1_hash TorrentViewModel::infoHash() {
    return _infoHash;
}
*/

PluginInstalled TorrentViewModel::pluginInstalled() const {
    return _pluginInstalled;
}
