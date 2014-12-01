#include "PaymentChannelViewModel.hpp"
#include "extension/PeerPluginStatus.hpp"

#include <QList>
#include <QStandardItem>

PaymentChannelViewModel::PaymentChannelViewModel(PeerPlugin * peerPlugin, QStandardItemModel * paymentChannelsTableViewModel)
    : peerPlugin_(peerPlugin)
    , paymentChannelsTableViewModel_(paymentChannelsTableViewModel)
{

    // Allocate view items
    // These are later owned by the external QStandardItem
    // model for the torrent table, hence we do not need to delete
    hostItem = new QStandardItem();
    stateItem = new QStandardItem();
    balanceItem = new QStandardItem();
    progressItem = new QStandardItem();

    // Add as row to
    QList<QStandardItem *> row;

    row.append(hostItem);
    row.append(stateItem);
    row.append(balanceItem);
    row.append(progressItem);

    // Add row to payment channel table view-model for this torrent
    paymentChannelsTableViewModel_->appendRow(row);
}

void PaymentChannelViewModel::update(PeerPluginStatus status) {

    // State
    updateState(status.peerPluginState_);

    // Balance
    updateBalance(status.balance_);

    // Progress
    updateProgress();
}

void PaymentChannelViewModel::updateHost(const QString & host) {
    hostItem->setText(host);
}

void PaymentChannelViewModel::updateState() {

}

void PaymentChannelViewModel::updateBalance(int balance) {
    balanceItem->setText(QString::number(balance));
}

void PaymentChannelViewModel::updateProgress() {

}
