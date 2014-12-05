#include "TorrentViewModel.hpp"
#include "peerPluginViewModel.hpp"

#include "extension/PeerPlugin.hpp"
#include "extension/PeerPluginStatus.hpp"

#include <QStandardItemModel>
#include <QStandardItem>

#include <libtorrent/socket_io.hpp>

const char * TorrentViewModel::columnTitles[] = {"Name", "Size", "Status", "Speed", "Peers", "Mode", "#Channels", "Balance"};
const int TorrentViewModel::numberOfColumns = sizeof(TorrentViewModel::columnTitles)/sizeof(char *);

TorrentViewModel::TorrentViewModel(const libtorrent::sha1_hash & info_hash, QStandardItemModel & torrentTableViewModel, QLoggingCategory & category)
    : info_hash_(info_hash)
    , torrentTableViewModel_(torrentTableViewModel)
    , peerPluginsTableViewModel_(0, PeerPluginViewModel::numberOfColumns)
    , category_(category)
{

    // Allocate view items
    // These are later owned by the external QStandardItem
    // model for the torrent table, hence we do not need to delete
    nameItem = new QStandardItem();
    sizeItem  = new QStandardItem();
    stateItem = new QStandardItem();
    speedItem = new QStandardItem();
    peersItem = new QStandardItem();
    modeItem = new QStandardItem();
    peerPluginsItem = new QStandardItem();
    balanceItem = new QStandardItem();

    // Set item data, so this is recoverable
    nameItem->setData(QVariant::fromValue(this));
    sizeItem->setData(QVariant::fromValue(this));
    stateItem->setData(QVariant::fromValue(this));
    speedItem->setData(QVariant::fromValue(this));
    peersItem->setData(QVariant::fromValue(this));
    modeItem->setData(QVariant::fromValue(this));
    peerPluginsItem->setData(QVariant::fromValue(this));
    balanceItem->setData(QVariant::fromValue(this));

    // Add as row to torrentTableViewModel
    QList<QStandardItem *> row;

    row.append(nameItem);
    row.append(sizeItem);
    row.append(stateItem);
    row.append(speedItem);
    row.append(peersItem);
    row.append(modeItem);
    row.append(peerPluginsItem);
    row.append(balanceItem);

    torrentTableViewModel_.appendRow(row);

    // Add columns to model
    for(int i = 0;i < PeerPluginViewModel::numberOfColumns;i++)
        peerPluginsTableViewModel_.setHorizontalHeaderItem(i, new QStandardItem(PeerPluginViewModel::columnTitles[i]));
}

TorrentViewModel::~TorrentViewModel(){

    // Delete peer plugin view models
    for(std::map<boost::asio::ip::tcp::endpoint, PeerPluginViewModel *>::iterator i = peerPluginViewModels.begin(),
            end(peerPluginViewModels.end());i != end;i++)
        delete i->second;
}

QStandardItemModel * TorrentViewModel::getPeerPluginsTableViewModel() {
    return &peerPluginsTableViewModel_;
}

void TorrentViewModel::update(const libtorrent::torrent_status & torrentStatus) {

    // State
    updateState(torrentStatus.paused, torrentStatus.state, torrentStatus.progress);

    // Up/Down rates
    updateSpeed(torrentStatus.download_rate, torrentStatus.upload_rate);
}

void TorrentViewModel::updateName(const QString & name) {
    nameItem->setText(name);
}

void TorrentViewModel::updateSize(int size) {
    sizeItem->setText(QString::number(size));
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
    stateItem->setText(itemText);
}

void TorrentViewModel::updateSpeed(int downloadRate, int uploadRate) {

    // Speed
    QString speed;

    speed.append(QString::number(downloadRate/1024))
         .append("Kb/s | ")
         .append(QString::number(uploadRate/1024))
         .append("Kb/s");

    // Update item
    speedItem->setText(speed);
}

void TorrentViewModel::updatePeers(int numberOfPeers, int numberOfPeersWithExtension) {

    QString peers = QString::number(numberOfPeers) + "|" + QString::number(numberOfPeersWithExtension);

    // Update item
    peersItem->setText(peers);
}

void TorrentViewModel::updateMode(bool pluginOn) {

    QString mode(pluginOn ? "BitSwapr" : "Classic");

    // Update item
    modeItem->setText(mode);
}

void TorrentViewModel::updateBalance(int tokensReceived, int tokensSent) {

    QString balance = QString::number(tokensReceived) + "|" + QString::number(tokensSent);

    // Update item
    balanceItem->setText(balance);
}

void TorrentViewModel::addPeerPlugin(PeerPlugin * peerPlugin) {

    const boost::asio::ip::tcp::endpoint & endPoint = peerPlugin->getEndPoint();

    // Add to map
    peerPluginViewModels.insert(std::make_pair(endPoint, new PeerPluginViewModel(endPoint, peerPluginsTableViewModel_)));

    // Notify
    std::string endPointString = libtorrent::print_endpoint(endPoint);
    qCDebug(category_) << "addPeerPlugin" << endPointString.c_str();
}

void TorrentViewModel::removePeerPlugin(const boost::asio::ip::tcp::endpoint & endPoint) {
    qCDebug(category_) << "removePeerPlugin: NOT IMPLEMENTED";
}

void TorrentViewModel::updatePeerPluginState(PeerPluginStatus status) {

    // If extension is not enabled, then it should not be registered with us.
    // If extension is enabled, it may still be it is not registered here,
    // simply because peerAdded() signal has not been processed yet.
    if(status.peerPlugin_->getPeerBEP43SupportedStatus() != PeerPlugin::PEER_BEP_SUPPORTED_STATUS::supported)
        return;

    // Find Peer
    const boost::asio::ip::tcp::endpoint & endPoint = status.peerPlugin_->getEndPoint();
    std::map<boost::asio::ip::tcp::endpoint,PeerPluginViewModel *>::iterator mapIterator = peerPluginViewModels.find(endPoint);

    std::string endPointString = libtorrent::print_endpoint(endPoint);
    qCDebug(category_) << "updatePeerPluginState" << endPointString.c_str();

    if(mapIterator == peerPluginViewModels.end()) {
        qCCritical(category_) << "No mathching end point found.";
        return;
    } else
        qCCritical(category_) << "end point found.";

    PeerPluginViewModel * peerPluginViewModel = mapIterator->second;

    // Update
    peerPluginViewModel->update(status);
}

const libtorrent::sha1_hash & TorrentViewModel::getInfoHash() const {
    return info_hash_;
}
