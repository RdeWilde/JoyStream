#include "PeerPluginViewModel.hpp"
#include "extension/PeerPluginStatus.hpp"
#include "extension/PeerPluginState.hpp"

#include <libtorrent/socket_io.hpp>

#include <QString>
#include <QList>
#include <QStandardItem>

const char * PeerPluginViewModel::columnTitles[] = {"Host", "State", "Balance", "Progress"};
const int PeerPluginViewModel::numberOfColumns = sizeof(PeerPluginViewModel::columnTitles)/sizeof(char *);

PeerPluginViewModel::PeerPluginViewModel(const libtorrent::tcp::endpoint & endPoint, QStandardItemModel & peerPluginsTableViewModel)
    : endPoint_(endPoint)
    , peerPluginsTableViewModel_(peerPluginsTableViewModel)
{

    /**
     * Allocate view items
     * These are later owned by the external QStandardItem
     * model for the torrent table, hence we do not need to delete.
     */

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

    // Add as row to model
    QList<QStandardItem *> row;

    row.append(hostItem);
    row.append(stateItem);
    row.append(balanceItem);
    row.append(progressItem);

    // Add row to peer plugins table view-model for this torrent
    peerPluginsTableViewModel_.appendRow(row);
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

void PeerPluginViewModel::updateState(PeerState state) {

    switch(state) {
        case PeerState::started: stateItem->setText("Started"); break;
        case PeerState::BEP10_handshake_received: stateItem->setText("Received handshake"); break;
        case PeerState::observe_mode_announced: stateItem->setText("Observe mode announced");
        case PeerState::buy_mode_announced: stateItem->setText("Buy mode announced");
        case PeerState::sell_mode_announced: stateItem->setText("Sell mode announced");

        break;
    }
}

void PeerPluginViewModel::updateBalance(int balance) {
    balanceItem->setText(QString::number(balance));
}

void PeerPluginViewModel::updateProgress() {

}

const libtorrent::tcp::endpoint & PeerPluginViewModel::getEndPoint() const {
    return endPoint_;
}
