#include "PeerPluginViewModel.hpp"
//#include "extension/PeerPluginState.hpp"

#include <libtorrent/socket_io.hpp>

#include <QString>
#include <QList>
#include <QStandardItem>

//const char * PeerPluginViewModel::columnTitles[] = {"Host", "State", "Balance", "Progress"};
//const int PeerPluginViewModel::numberOfColumns = sizeof(PeerPluginViewModel::columnTitles)/sizeof(char *);

PeerPluginViewModel::PeerPluginViewModel(const libtorrent::tcp::endpoint & endPoint)
    : _endPoint(endPoint)
    //, _peerPluginsTableViewModel(peerPluginsTableViewModel) {
    , _hostItem(new QStandardItem())
    , _stateItem(new QStandardItem())
    , _balanceItem(new QStandardItem())
    , _progressItem(new QStandardItem()) {

    // Set host text
    //std::string hostAddress = libtorrent::print_endpoint(endPoint);
    //_hostItem->setText(hostAddress.c_str());
}

/**
void PeerPluginViewModel::update(PeerPluginStatus status) {

    // State
    //updateState(status.peerPluginState_);

    // Balance
    updateBalance(status.balance_);

    // Progress
    updateProgress();
}
*/

void PeerPluginViewModel::updateHost(const QString & host) {
    _hostItem->setText(host);
}

/*
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
*/

void PeerPluginViewModel::updateBalance(int balance) {
    _balanceItem->setText(QString::number(balance));
}

void PeerPluginViewModel::updateProgress() {

}

libtorrent::tcp::endpoint PeerPluginViewModel::endPoint() {
    return _endPoint;
}
