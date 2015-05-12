#include "TorrentView.hpp"
#include "controller/PluginInstalled.hpp"

#include <QStandardItemModel>
#include <QStandardItem>

TorrentView::TorrentView(const libtorrent::torrent_status & status,
                         int size,
                         QStandardItemModel * itemModel)
    : _nameItem(new QStandardItem())
    , _sizeItem(new QStandardItem())
    , _stateItem(new QStandardItem())
    , _speedItem(new QStandardItem())
    , _peersItem(new QStandardItem())
    , _modeItem(new QStandardItem())
    , _balanceItem(new QStandardItem())
    , _pauseAction("Pause", this)
    , _startAction("Start", this)
    , _removeAction("Remove", this)
    , _viewExtensionAction("Extension", this) {

    // Set initial item values
    updateStatus(status);
    _sizeItem->setText(sizeToString(size));

    // Add row to itemModel
    QList<QStandardItem *> items;

    items << _nameItem
          << _sizeItem
          << _stateItem
          << _speedItem
          << _peersItem
          << _modeItem
          << _balanceItem;

    itemModel->appendRow(items);

    // Add menu buttons
    _torrentTableContextMenu.addAction(&_startAction);
    _torrentTableContextMenu.addAction(&_pauseAction);
    _torrentTableContextMenu.addAction(&_removeAction);
    //_torrentTableContextMenu.addAction(&_viewExtension); // add action to menu only when plugin is installed
}

QString TorrentView::sizeToString(int size) const {
    return QString::number(size) + QString("b");
}

QString TorrentView::pluginInstalledToString(PluginInstalled pluginInstalled) const {

    QString text;

    switch(pluginInstalled) {

        case PluginInstalled::Seller:
            text = "Seller";
        case PluginInstalled::Buyer:
            text = "Buyer";
        case PluginInstalled::Observer:
            text = "Observer";
        case PluginInstalled::None:
            text = "None";
    }

    return text;
}

QString TorrentView::torrentStateToString(bool paused, libtorrent::torrent_status::state_t state, float progress) const {

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

QString TorrentView::speedToString(int downloadRate, int uploadRate) const {

    QString text;

    text.append(QString::number(downloadRate/1024))
         .append("Kb/s | ")
         .append(QString::number(uploadRate/1024))
         .append("Kb/s");

    return text;
}

void TorrentView::peersToString(int numberOfPeers, int numberOfPeersWithExtension) const {
    return QString::number(numberOfPeers) + "|" + QString::number(numberOfPeersWithExtension);
}


QMenu * TorrentView::torrentTableContextMenu() {
    return &_torrentTableContextMenu;
}

const QAction * TorrentView::pauseAction() const {
    return &_pauseAction;
}

const QAction * TorrentView::startAction() const {
    return &_startAction;
}

const QAction * TorrentView::removeAction() const {
    return &_removeAction;
}

const QAction * TorrentView::viewExtensionAction() const {
    return &_viewExtensionAction;
}

void TorrentView::updatePluginInstalled(PluginInstalled pluginInstalled) {

    // mode
    _modeItem->setText(pluginInstalledToString(pluginInstalled));
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

void TorrentView::updatePeers(int numberOfPeers, int numberOfPeersWithExtension) {
    _peersItem->setText(peersToString(numberOfPeers, numberOfPeersWithExtension));
}
