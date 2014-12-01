#include "TorrentViewModel.hpp"
#include "PaymentChannelViewModel.hpp"

#include "extension/PeerPlugin.hpp"
#include "extension/PeerPluginStatus.hpp"

#include <QStandardItemModel>
#include <QStandardItem>

TorrentViewModel::TorrentViewModel(libtorrent::sha1_hash & info_hash, QStandardItemModel * torrentTableViewModel)
    : info_hash_(info_hash)
    , torrentTableViewModel_(torrentTableViewModel)
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

    // Create view model for payment channels
    paymentChannelsTableViewModel_ = new QStandardItemModel(0, 4); //0 Rows and 4 Columns

    // Add columns to model
    paymentChannelsTableViewModel_->setHorizontalHeaderItem(0, new QStandardItem(QString("Host")));
    paymentChannelsTableViewModel_->setHorizontalHeaderItem(1, new QStandardItem(QString("State")));
    paymentChannelsTableViewModel_->setHorizontalHeaderItem(2, new QStandardItem(QString("Balance")));
    paymentChannelsTableViewModel_->setHorizontalHeaderItem(3, new QStandardItem(QString("Progress")));
}

TorrentViewModel::~TorrentViewModel(){

    // Remove corresponding row from torrentTableViewModel_
    QModelIndex index = torrentTableViewModel_->indexFromItem(nameItem);

    torrentTableViewModel_->removeRows (index.row(), 1);

    // Delete all paymentChannelViewModels
    for(std::vector<PaymentChannelViewModel *>::iterator i = paymentChannelViewModels.begin(),
            end(paymentChannelViewModels.end());i != end; i++)
        delete *i;


    //
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
    QString state;

    if(paused)
        state = "Paused";
    else {

        // .progress reports the pogress of the relevant task, but I suspect it is being used
        // some what incorrectly in som eof the cases below.

        switch(state) {

            case libtorrent::torrent_status::queued_for_checking:

                state = "Queued for checking";

                break;
            case libtorrent::torrent_status::checking_files:

                state = "Checking files";
                state += " " + QString::number(progress*100,'f',0).append("%");
                break;
            case libtorrent::torrent_status::downloading_metadata:

                state = "Downloading metadata";
                state += " " + QString::number(progress*100,'f',0).append("%");
                break;
            case libtorrent::torrent_status::downloading:

                state = "Downloading";
                state += " " + QString::number(progress*100,'f',0).append("%");
                break;
            case libtorrent::torrent_status::finished:

                state = "Finished";
                break;
            case libtorrent::torrent_status::seeding:

                state = "Seeding";
                break;
            case libtorrent::torrent_status::allocating:

                state = "Allocating";
                state += " " + QString::number(progress*100,'f',0).append("%");
                break;
            case libtorrent::torrent_status::checking_resume_data:

                state = "Checking resume data";
                state += " " + QString::number(progress*100,'f',0).append("%");
                break;
        }

    }

    // Update item
    stateItem->setText(state);
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
    paymentChannelViewModels.insert(std::make_pair(peerPlugin, paymentChannelViewModel));
}

void TorrentViewModel::updatePaymentChannel(PeerPluginStatus status) {

    // Find Peer

   ///// llokuo in map

    // Update
    update(status);
}
