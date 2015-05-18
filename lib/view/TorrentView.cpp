#include "TorrentView.hpp"
#include "controller/TorrentViewModel.hpp"
#include "controller/SellerTorrentPluginViewModel.hpp"
#include "controller/BuyerTorrentPluginViewModel.hpp"

#include <QStandardItemModel>
#include <QStandardItem>

TorrentView::TorrentView(QObject * parent,
                         const TorrentViewModel * torrentViewModel,
                         QStandardItemModel * model)
    : QObject(parent)
    , _infoHash(torrentViewModel->infoHash())
    , _nameItem(new QStandardItem())
    , _sizeItem(new QStandardItem(sizeToString(torrentViewModel->torrentInfo()->total_size())))
    , _stateItem(new QStandardItem())
    , _speedItem(new QStandardItem())
    , _buyersItem(new QStandardItem())
    , _sellersItem(new QStandardItem())
    , _pluginInstalledItem(new QStandardItem(pluginInstalledToString(torrentViewModel->pluginInstalled())))
    , _balanceItem(new QStandardItem())
    , _pauseAction("Pause", this)
    , _startAction("Start", this)
    , _removeAction("Remove", this)
    , _viewExtensionAction("Extension", this) {

    // Set initial item values
    updateStatus(torrentViewModel->status());

    // Add row to itemModel
    QList<QStandardItem *> items;

    items << _nameItem
          << _sizeItem
          << _stateItem
          << _speedItem
          << _buyersItem
          << _sellersItem
          << _pluginInstalledItem
          << _balanceItem;

    model->appendRow(items);

    // Center content
    _nameItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _sizeItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _stateItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _speedItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _buyersItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _sellersItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _pluginInstalledItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _balanceItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    // Add menu buttons
    _torrentTableContextMenu.addAction(&_startAction);
    _torrentTableContextMenu.addAction(&_pauseAction);
    _torrentTableContextMenu.addAction(&_removeAction);
    // add action to menu only when plugin is installed

    // Connect: view menu actions to model slots
    QObject::connect(&_startAction,
                     SIGNAL(triggered()),
                     this,
                     SLOT(start()));

    QObject::connect(&_pauseAction,
                     SIGNAL(triggered()),
                     this,
                     SLOT(pause()));

    QObject::connect(&_removeAction,
                     SIGNAL(triggered()),
                     this,
                     SLOT(remove()));

    // Connect viewing extension menu choice to slot on this object
    QObject::connect(&_viewExtensionAction,
                     SIGNAL(triggered()),
                     this,
                     SLOT(viewExtension()));

    // Connect: view model signals to slots on this object
    /**
    QObject::connect(torrentViewModel,
                     SIGNAL(pluginInstalledChanged(PluginInstalled)),
                     this,
                     SLOT(updatePluginInstalled(PluginInstalled)));
    */

    QObject::connect(torrentViewModel,
                     SIGNAL(startedBuyerTorrentPlugin(const BuyerTorrentPluginViewModel*)),
                     this,
                     SLOT(updateStartedBuyerTorrentPlugin(const BuyerTorrentPluginViewModel*)));

    QObject::connect(torrentViewModel,
                     SIGNAL(startedSellerTorrentPlugin(const SellerTorrentPluginViewModel*)),
                     this,
                     SLOT(updateStartedSellerTorrentPlugin(const SellerTorrentPluginViewModel*)));

    QObject::connect(torrentViewModel,
                     SIGNAL(torrentStatusChanged(const libtorrent::torrent_status &)),
                     this,
                     SLOT(updateStatus(const libtorrent::torrent_status &)));
}

QString TorrentView::sizeToString(int size) {
    return QString::number(size) + QString("b");
}

QString TorrentView::pluginInstalledToString(PluginInstalled pluginInstalled) {

    QString text;

    switch(pluginInstalled) {

        case PluginInstalled::Seller:
            text = "Seller";
            break;
        case PluginInstalled::Buyer:
            text = "Buyer";
            break;
        case PluginInstalled::Observer:
            text = "Observer";
            break;
        case PluginInstalled::None:
            text = "None";
    }

    return text;
}

QString TorrentView::torrentStateToString(bool paused, libtorrent::torrent_status::state_t state, float progress) {

    if(paused)
        return QString("Paused");

    // State
    QString text;

    // .progress reports the pogress of the relevant task, but I suspect it is being used
    // some what incorrectly in som eof the cases below.

    switch(state) {

        case libtorrent::torrent_status::queued_for_checking:

            text = "Queued for checking";

            break;
        case libtorrent::torrent_status::checking_files:

            text = "Checking files";
            text += " " + QString::number(progress*100,'f',0).append("%");
            break;
        case libtorrent::torrent_status::downloading_metadata:

            text = "Downloading metadata";
            text += " " + QString::number(progress*100,'f',0).append("%");
            break;
        case libtorrent::torrent_status::downloading:

            text = "Downloading";
            text += " " + QString::number(progress*100,'f',0).append("%");
            break;
        case libtorrent::torrent_status::finished:

            text = "Finished";
            break;
        case libtorrent::torrent_status::seeding:

            text = "Seeding";
            break;
        case libtorrent::torrent_status::allocating:

            text = "Allocating";
            text += " " + QString::number(progress*100,'f',0).append("%");
            break;
        case libtorrent::torrent_status::checking_resume_data:

            text = "Checking resume data";
            text += " " + QString::number(progress*100,'f',0).append("%");
            break;
    }

    return text;
}

QString TorrentView::speedToString(int downloadRate, int uploadRate) {

    QString text;

    text.append(QString::number(downloadRate/1024))
         .append("Kb/s | ")
         .append(QString::number(uploadRate/1024))
         .append("Kb/s");

    return text;
}

QString TorrentView::peersToString(int numberOfPeers, int numberOfPeersWithExtension) {
    return QString::number(numberOfPeers) + "|" + QString::number(numberOfPeersWithExtension);
}

QString TorrentView::balanceToString(quint64 balance) {
    return QString::number(balance) + "Ƀ";
}

/**
void TorrentView::updatePluginInstalled(PluginInstalled pluginInstalled) {

    // Should just happen once

    // pluginInstalledItem
    _pluginInstalledItem->setText(pluginInstalledToString(pluginInstalled));

    // add action to menu
    _torrentTableContextMenu.addAction(&_viewExtensionAction);

    // Notify rest of vie
    emit pluginInstalled(_infoHash);
}
*/

void TorrentView::updateStartedBuyerTorrentPlugin(const BuyerTorrentPluginViewModel * model) {

    // pluginInstalledItem
    _pluginInstalledItem->setText("Buyer");

    // add action to menu
    _torrentTableContextMenu.addAction(&_viewExtensionAction);

    // Set initial values
    updateNumberOfBuyers(model->numberOfBuyerPeers());
    updateNumberOfSellers(model->numberOfSellerPeers());
    updateBalance(model->balance());

    // Setup signal/slot connections
    QObject::connect(model,
                     SIGNAL(numberOfBuyerPeersChanged(quint32)),
                     this,
                     SLOT(updateNumberOfBuyers(quint32)));

    QObject::connect(model,
                     SIGNAL(numberOfSellerPeersChanged(quint32)),
                     this,
                     SLOT(updateNumberOfSellers(quint32)));

    QObject::connect(model,
                     SIGNAL(balanceChanged(quint64)),
                     this,
                     SLOT(updateBalance(quint64)));
}

void TorrentView::updateStartedSellerTorrentPlugin(const SellerTorrentPluginViewModel * model) {

    // Set initial values
    updateNumberOfBuyers(model->numberOfBuyerPeers());
    updateNumberOfSellers(model->numberOfSellerPeers());
    updateBalance(model->balance());

    // Setup signal/slot connections
    QObject::connect(model,
                     SIGNAL(numberOfBuyerPeersChanged(quint32)),
                     this,
                     SLOT(updateNumberOfBuyers(quint32)));

    QObject::connect(model,
                     SIGNAL(numberOfSellerPeersChanged(quint32)),
                     this,
                     SLOT(updateNumberOfSellers(quint32)));

    QObject::connect(model,
                     SIGNAL(balanceChanged(quint64)),
                     this,
                     SLOT(updateBalance(quint64)));
}


void TorrentView::updateStatus(const libtorrent::torrent_status & status) {

    // name
    QString name = QString::fromStdString(status.name);
    _nameItem->setText(name);

    // state
    _stateItem->setText(torrentStateToString(status.paused, status.state, status.progress));

    // speed
    _speedItem->setText(speedToString(status.download_rate, status.upload_rate));

}

/**
void TorrentView::updatePeers(int numberOfPeers, int numberOfPeersWithExtension) {
    _peersItem->setText(peersToString(numberOfPeers, numberOfPeersWithExtension));
}
*/

void TorrentView::updateNumberOfBuyers(quint32 num) {
    _buyersItem->setText(QString::number(num));
}

void TorrentView::updateNumberOfSellers(quint32 num) {
    _sellersItem->setText(QString::number(num));
}

void TorrentView::updateBalance(quint64 balance) {
    _balanceItem->setText(balanceToString(balance));
}

void TorrentView::showContextMenu(const QPoint & point) {
    _torrentTableContextMenu.popup(point);
}

void TorrentView::pause() {
    emit pauseTorrentRequested(_infoHash);
}

void TorrentView::start() {
    emit startTorrentRequested(_infoHash);
}

void TorrentView::remove() {
    emit removeTorrentRequested(_infoHash);
}

void TorrentView::viewExtension() {
    emit requestedViewingExtension(_infoHash);
}
