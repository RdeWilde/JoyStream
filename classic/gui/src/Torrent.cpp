/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <gui/Torrent.hpp>

#include <common/DataSizeRepresentation.hpp>
#include <common/BitcoinRepresentation.hpp>
#include <common/BitcoinDisplaySettings.hpp>

#include <QStandardItem>

namespace joystream {
namespace classic {
namespace gui {

Torrent::Torrent(QObject * parent,
                         const TorrentViewModel * torrentViewModel,
                         const BitcoinDisplaySettings * settings,
                         QStandardItem * nameItem,
                         QStandardItem * sizeItem,
                         QStandardItem * stateItem,
                         QStandardItem * speedItem,
                         QStandardItem * buyersItem,
                         QStandardItem * sellersItem,
                         QStandardItem * pluginInstalledItem,
                         QStandardItem * balanceItem)
    : QObject(parent)
    , _infoHash(torrentViewModel->infoHash())
    , _settings(settings)
    , _nameItem(nameItem)
    , _sizeItem(sizeItem)
    , _stateItem(stateItem)
    , _speedItem(speedItem)
    , _numberOfBuyerPeersItem(buyersItem)
    , _numberOfSellerPeersitem(sellersItem)
    , _sessionModeItem(pluginInstalledItem)
    , _balanceItem(balanceItem)
    , _pauseAction("Pause", this)
    , _startAction("Start", this)
    , _removeAction("Remove", this)
    , _viewExtensionAction("Extension", this)
    , _streamAction("Stream", this) {

    // Set values
    updateStatus(torrentViewModel->status());
    //const libtorrent::torrent_info * torrentInfo = torrentViewModel->torrentInfo();
    boost::intrusive_ptr<libtorrent::torrent_info> torrentFile = torrentViewModel->torrentFile();
    updateSize(torrentFile->total_size());
    updatePluginInstalled(torrentViewModel->pluginInstalled());

    // Center content
    _nameItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _sizeItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _stateItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _speedItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _numberOfBuyerPeersItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _numberOfSellerPeersitem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _sessionModeItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _balanceItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    // Add menu buttons
    /**
    _torrentTableContextMenu.addAction(&_startAction);
    _torrentTableContextMenu.addAction(&_pauseAction);
    _torrentTableContextMenu.addAction(&_removeAction);
    */
    // add action to menu only when plugin is installed

    // disable (temporary) main menu actions
    _startAction.setEnabled(false);
    _pauseAction.setEnabled(false);
    _removeAction.setEnabled(false);

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

    QObject::connect(&_streamAction,
                     SIGNAL(triggered()),
                     this,
                     SLOT(startStreamPlayback()));


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


    /**
    _nameItem->setEditable(false);
    _sizeItem->setEditable(false);
    _stateItem->setEditable(false);
    _speedItem->setEditable(false);
    _numberOfBuyerPeersItem->setEditable(false);
    _numberOfSellerPeersitem->setEditable(false);
    _sessionModeItem->setEditable(false);
    _balanceItem->setEditable(false);
    */
}

QString Torrent::sizeToString(qint64 size) {
    return DataSizeRepresentation(size, DataSizeRepresentation::Base::Byte).toString();
}

QString Torrent::pluginInstalledToString(PluginInstalled pluginInstalled) {

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

QString Torrent::torrentStateToString(bool paused, libtorrent::torrent_status::state_t state, float progress) {

    if(paused)
        return QString("Paused");

    // State
    QString text;

    // .progress reports the pogress of the relevant task, but I suspect it is being used
    // some what incorrectly in som eof the cases below.

    switch(state) {

        //case libtorrent::torrent_status::unused_enum_for_backwards_compatibility:
        case libtorrent::torrent_status::queued_for_checking:



            text = "Queued for checking";
            //text = "Unknown state";

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

QString Torrent::speedToString(int downloadRate, int uploadRate) {

    QString downloadSpeedString = DataSizeRepresentation(downloadRate, DataSizeRepresentation::Base::Byte).toString() + "/s";
    QString uploadSpeedString = DataSizeRepresentation(uploadRate, DataSizeRepresentation::Base::Byte).toString() + "/s";

    return downloadSpeedString + QString(" | ") + uploadSpeedString;
}

QString Torrent::peersToString(int numberOfPeers, int numberOfPeersWithExtension) {
    return QString::number(numberOfPeers) + QString(" | ") + QString::number(numberOfPeersWithExtension);
}

QString Torrent::balanceToString(qint64 balance, const BitcoinDisplaySettings * settings) {
    //return BitCoinRepresentation(balance).toString(BitCoinRepresentation::Fiat::USD, rate);
    return BitcoinRepresentation(balance).toString(settings);
}

void Torrent::updatePluginInstalled(PluginInstalled pluginInstalled) {
    _sessionModeItem->setText(pluginInstalledToString(pluginInstalled));
}

void Torrent::updateStartedBuyerTorrentPlugin(const BuyerTorrentPluginViewModel * model) {

    // pluginInstalledItem
    _sessionModeItem->setText("Buyer");

    // add action to menu
    //_torrentTableContextMenu.addSection("");
    _torrentTableContextMenu.addAction(&_viewExtensionAction);

    // add streaming action to menu
    //_torrentTableContextMenu.addAction(&_streamAction);

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
                     SIGNAL(balanceChanged(qint64)),
                     this,
                     SLOT(updateBalance(qint64)));
}

void Torrent::updateStartedSellerTorrentPlugin(const SellerTorrentPluginViewModel * model) {

    // pluginInstalledItem
    _sessionModeItem->setText("Seller");

    // add action to menu
    _torrentTableContextMenu.addSection("");
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
                     SIGNAL(balanceChanged(qint64)),
                     this,
                     SLOT(updateBalance(qint64)));
}

void Torrent::updateStatus(const libtorrent::torrent_status & status) {

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

void Torrent::updateSize(qint64 totalSize) {
    _sizeItem->setText(sizeToString(totalSize));
}

void Torrent::updateNumberOfBuyers(quint32 num) {
    _numberOfBuyerPeersItem->setText(QString::number(num));
}

void Torrent::updateNumberOfSellers(quint32 num) {
    _numberOfSellerPeersitem->setText(QString::number(num));
}

void Torrent::updateBalance(qint64 balance) {
    _balanceItem->setText(balanceToString(balance, _settings));
}

void Torrent::showContextMenu(const QPoint & point) {
    _torrentTableContextMenu.popup(point);
}

void Torrent::pause() {
    emit pauseTorrentRequested(_infoHash);
}

void Torrent::start() {
    emit startTorrentRequested(_infoHash);
}

void Torrent::remove() {
    emit removeTorrentRequested(_infoHash);
}

void Torrent::viewExtension() {
    emit requestedViewingExtension(_infoHash);
}

void Torrent::startStreamPlayback() {
    emit requestedStreamingPlayback(_infoHash);
}

}
}
}
