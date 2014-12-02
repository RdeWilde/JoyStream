#include "PaymentChannelViewModel.hpp"
#include "extension/PeerPluginStatus.hpp"
#include "extension/PeerPluginState.hpp"

#include <QString>
#include <QList>
#include <QStandardItem>

PaymentChannelViewModel::PaymentChannelViewModel(PeerPlugin * peerPlugin, QStandardItemModel * paymentChannelsTableViewModel)
    : paymentChannelsTableViewModel_(paymentChannelsTableViewModel)

    // save peerPlugin later, we dont need for now
{

    // Allocate view items
    // These are later owned by the external QStandardItem
    // model for the torrent table, hence we do not need to delete
    hostItem = new QStandardItem();
    stateItem = new QStandardItem();
    balanceItem = new QStandardItem();
    progressItem = new QStandardItem();

    // Set item data, so this is recoverable
    hostItem->setData(QVariant::fromValue(this));
    stateItem->setData(QVariant::fromValue(this));
    balanceItem->setData(QVariant::fromValue(this));
    progressItem->setData(QVariant::fromValue(this));

    // Add as row to
    QList<QStandardItem *> row;

    row.append(hostItem);
    row.append(stateItem);
    row.append(balanceItem);
    row.append(progressItem);

    // Add row to payment channel table view-model for this torrent
    paymentChannelsTableViewModel_->appendRow(row);
}

PaymentChannelViewModel::~PaymentChannelViewModel() {

    // Nothing to delete at moment

    // items are owned by paymentChannelsTableViewModel_, which we
    // do not own.

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

void PaymentChannelViewModel::updateState(PeerPluginState state) {

    switch(state) {
        case PeerPluginState::started: stateItem->setText("Started");
        case PeerPluginState::buy_message_received: stateItem->setText("Sent buy offer");
        case PeerPluginState::sell_message_received: stateItem->setText("Sent sell offer");
    }
}

void PaymentChannelViewModel::updateBalance(int balance) {
    balanceItem->setText(QString::number(balance));
}

void PaymentChannelViewModel::updateProgress() {

}
