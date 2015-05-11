#include "TorrentView.hpp"
#include "controller/TorrentViewModel.hpp"
#include "controller/PluginInstalled.hpp"

#include <QStandardItemModel>
#include <QStandardItem>

TorrentView::TorrentView(const TorrentViewModel * viewModel, QStandardItemModel * itemModel)
    : _nameItem(new QStandardItem())
    , _sizeItem(new QStandardItem())
    , _stateItem(new QStandardItem())
    , _speedItem(new QStandardItem())
    , _peersItem(new QStandardItem())
    , _modeItem(new QStandardItem())
    , _balanceItem(new QStandardItem())
    , _pause("Pause", this)
    , _start("Start", this)
    , _remove("Remove", this)
    , _viewExtension("Extension", this) {

    // Set initial item values
    updateStatus(viewModel->status());
    _sizeItem->setText(getSizeText(viewModel->torrentInfo()->total_size()));

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

    // Connect view model signals to this slot
    QObject::connect(viewModel,
                     SIGNAL(pluginInstalledChanged(PluginInstalled)),
                     this,
                     SLOT(updatePluginInstalled(PluginInstalled)));

    QObject::connect(viewModel,
                     SIGNAL(torrentStatusChanged(const libtorrent::torrent_status &)),
                     this,
                     SLOT(updateStatus(const libtorrent::torrent_status &)));


    // Add menu buttons
    _torrentTableContextMenu.addAction(&_start);
    _torrentTableContextMenu.addAction(&_pause);
    _torrentTableContextMenu.addAction(&_remove);
    //_torrentTableContextMenu.addAction(&_viewExtension); // add action to menu only when plugin is installed

    // Connect menu action trigger signals
    // to corresponding view model slot
    QObject::connect(&_pause,
                     SIGNAL(triggered()),
                     viewModel,
                     SLOT(pause()));

    QObject::connect(&_start,
                     SIGNAL(triggered()),
                     viewModel,
                     SLOT(start()));

    QObject::connect(&_remove,
                     SIGNAL(triggered()),
                     viewModel,
                     SLOT(remove()));

    QObject::connect(&_viewExtension,
                     SIGNAL(triggered()),
                     viewModel,
                     SLOT(showExtensionDialog()));
}

QMenu * TorrentView::torrentTableContextMenu() {
    return &_torrentTableContextMenu;
}

void TorrentView::updatePluginInstalled(PluginInstalled pluginInstalled) {

    // mode
    _modeItem->setText(getPluginInstalledText(pluginInstalled));
}

void TorrentView::updateStatus(const libtorrent::torrent_status & status) {

    // name
    QString name = QString::fromStdString(status.name);
    _nameItem->setText(name);

    // state
    _stateItem->setText(getTorrentStateText(status.paused, status.state, status.progress));

    // speed
    _speedItem->setText(getSpeedText(status.download_rate, status.upload_rate));

}

void TorrentView::updatePeers(int numberOfPeers, int numberOfPeersWithExtension) {
    _peersItem->setText(getPeersText(numberOfPeers, numberOfPeersWithExtension));
}

QString TorrentView::getSizeText(int size) {
    return QString::number(size) + QString("b");
}

QString TorrentView::getPluginInstalledText(PluginInstalled pluginInstalled) {

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

QString TorrentView::getTorrentStateText(bool paused, libtorrent::torrent_status::state_t state, float progress) {

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

QString TorrentViewModel::getSpeedText(int downloadRate, int uploadRate) {

    QString text;

    text.append(QString::number(downloadRate/1024))
         .append("Kb/s | ")
         .append(QString::number(uploadRate/1024))
         .append("Kb/s");

    return text;
}

void TorrentViewModel::getPeersText(int numberOfPeers, int numberOfPeersWithExtension) {
    return QString::number(numberOfPeers) + "|" + QString::number(numberOfPeersWithExtension);
}
