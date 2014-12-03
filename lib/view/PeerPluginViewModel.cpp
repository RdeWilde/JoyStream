#include "PeerPluginViewModel.hpp"
#include "extension/PeerPluginStatus.hpp"
#include "extension/PeerPluginState.hpp"

#include <libtorrent/socket_io.hpp>

#include <QString>
#include <QList>
#include <QStandardItem>

PeerPluginViewModel::PeerPluginViewModel(const boost::asio::ip::tcp::endpoint & endPoint, QStandardItemModel * peerPluginsTableViewModel)
    : endPoint_(endPoint)
    , peerPluginsTableViewModel_(peerPluginsTableViewModel)

    // save peerPlugin later, we dont need for now
{

    // Allocate view items
    // These are later owned by the external QStandardItem
    // model for the torrent table, hence we do not need to delete

    std::string hostAddress = libtorrent::print_endpoint(endPoint);
    hostItem = new QStandardItem(QString(hostAddress.c_str()));
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

    // Add row to peer plugins table view-model for this torrent
    peerPluginsTableViewModel_->appendRow(row);
}

PeerPluginViewModel::~PeerPluginViewModel() {

    // Nothing to delete at moment

    // items are owned by peerPluginsTableViewModel_, which we
    // do not own.

}

void PeerPluginViewModel::update(PeerPluginStatus status) {

    // State
    updateState(status.peerPluginState_);

    // Balance
    updateBalance(status.balance_);

    // Progress
    updateProgress();
}

void PeerPluginViewModel::updateHost(const QString & host) {
    hostItem->setText(host);
}

void PeerPluginViewModel::updateState(PeerPluginState state) {

    switch(state) {
        case PeerPluginState::started: stateItem->setText("Started");
        case PeerPluginState::handshake_received: stateItem->setText("Received handshake");
        case PeerPluginState::buy_message_received: stateItem->setText("Received buy offer");
        case PeerPluginState::sell_message_received: stateItem->setText("Received sell offer");
    }
}

void PeerPluginViewModel::updateBalance(int balance) {
    balanceItem->setText(QString::number(balance));
}

void PeerPluginViewModel::updateProgress() {

}
