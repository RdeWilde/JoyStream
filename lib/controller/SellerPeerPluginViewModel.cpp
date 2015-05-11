#include "SellerPeerPluginViewModel.hpp"
#include "SellerTorrentPluginViewModel.hpp"

#include <QStandardItem>

SellerPeerPluginViewModel::SellerPeerPluginViewModel(const SellerPeerPlugin::Status & status)
    : _clientState(status.clientState())
    , _payeeViewModel(status.payeeStatus()) {
}

void SellerPeerPluginViewModel::update(const SellerPeerPlugin::Status & status) {

    if(_clientState != status.clientState()) {
        _clientState = status.clientState();
        emit clientStateChanged(status.clientState());
    }

    _payeeViewModel.update(status.payeeStatus());
}

SellerPeerPlugin::ClientState SellerPeerPluginViewModel::clientState() const {
    return _clientState;
}

const PayeeViewModel * SellerPeerPluginViewModel::payeeViewModel() const {
    return &_payeeViewModel;
}

/**
QList<int> SellerPeerPluginViewModel::fullPiecesSent() const {
    return _fullPiecesSent;
}
*/
