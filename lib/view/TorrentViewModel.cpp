#include "TorrentViewModel.hpp"
#include "PaymentChannelViewModel.hpp"

#include "extension/PeerPlugin.hpp"
#include "extension/PeerPluginStatus.hpp"

#include <QStandardItemModel>
#include <QStandardItem>


TorrentViewModel::TorrentViewModel(const libtorrent::sha1_hash & info_hash, QStandardItemModel * torrentTableViewModel,  QLoggingCategory & category)
    : info_hash_(info_hash)
    , torrentTableViewModel_(torrentTableViewModel)
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
    paymentChannelsItem = new QStandardItem();
    balanceItem = new QStandardItem();

    // Set item data, so this is recoverable
    nameItem->setData(QVariant::fromValue(this));
    sizeItem->setData(QVariant::fromValue(this));
    stateItem->setData(QVariant::fromValue(this));
    speedItem->setData(QVariant::fromValue(this));
    peersItem->setData(QVariant::fromValue(this));
    modeItem->setData(QVariant::fromValue(this));
    paymentChannelsItem->setData(QVariant::fromValue(this));
    balanceItem->setData(QVariant::fromValue(this));

    // Add as row to torrentTableViewModel
    QList<QStandardItem *> row;

    row.append(nameItem);
    row.append(sizeItem);
    row.append(stateItem);
    row.append(speedItem);
    row.append(peersItem);
    row.append(modeItem);
    row.append(paymentChannelsItem);
    row.append(balanceItem);

    torrentTableViewModel_->appendRow(row);

    // Create model
    const char * columns[] = {"Host", "State", "Balance", "Progress"};
    const int numberOfColumns = sizeof(columns)/sizeof(char *);
    paymentChannelsTableViewModel_ = new QStandardItemModel(0, numberOfColumns);

    // Add columns to model
    for(int i = 0;i < numberOfColumns;i++)
        paymentChannelsTableViewModel_->setHorizontalHeaderItem(i, new QStandardItem(columns[i]));
}

TorrentViewModel::~TorrentViewModel(){

    // Remove corresponding row from torrentTableViewModel_
    QModelIndex index = torrentTableViewModel_->indexFromItem(nameItem);

    torrentTableViewModel_->removeRows(index.row(), 1);

    // Delete all paymentChannelViewModels for all payment channels
    for(std::map<boost::asio::ip::tcp::endpoint,PaymentChannelViewModel *>::iterator i = paymentChannelViewModels.begin(),
            end(paymentChannelViewModels.end());i != end; i++)
        delete i->second;

    // Delete view model for payment channels table,
    // this also will automatically delete all items in model
    delete paymentChannelsTableViewModel_;
}

QStandardItemModel * TorrentViewModel::getPaymentChannelsTableViewModel() {
    return paymentChannelsTableViewModel_;
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

void TorrentViewModel::addPaymentChannel(PeerPlugin * peerPlugin) {

    // Create view model for payment channel
    PaymentChannelViewModel * paymentChannelViewModel = new PaymentChannelViewModel(peerPlugin, paymentChannelsTableViewModel_);

    // Add to map
    paymentChannelViewModels.insert(std::make_pair(peerPlugin->getEndPoint(), paymentChannelViewModel));
}

void TorrentViewModel::updatePaymentChannel(PeerPluginStatus status) {

    // Find Peer
    std::map<boost::asio::ip::tcp::endpoint,PaymentChannelViewModel *>::iterator mapIterator = paymentChannelViewModels.find(status.peerPlugin_->getEndPoint());

    if(mapIterator == paymentChannelViewModels.end()) {
        qCCritical(category_) << "No match info_hash found.";
        return;
    }

    PaymentChannelViewModel * paymentChannelViewModel = mapIterator->second;

    // Update
    paymentChannelViewModel->update(status);
}

const libtorrent::sha1_hash & TorrentViewModel::getInfoHash() {
    return info_hash_;
}
