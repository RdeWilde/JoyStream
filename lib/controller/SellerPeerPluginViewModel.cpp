#include "SellerPeerPluginViewModel.hpp"
#include "SellerTorrentPluginViewModel.hpp"

#include <QStandardItem>

SellerPeerPluginViewModel::SellerPeerPluginViewModel(QObject * parent,
                                                     const libtorrent::tcp::endpoint & endPoint,
                                                     const SellerPeerPlugin::Status & status)
    : QObject(parent)
    , _endPoint(endPoint)
    , _clientState(status.clientState())
    , _payeeViewModel(this, status.payeeStatus()) {
}

void SellerPeerPluginViewModel::update(const SellerPeerPlugin::Status & status) {

    if(_clientState != status.clientState()) {
        _clientState = status.clientState();
        emit clientStateChanged(status.clientState());
    }

    _payeeViewModel.update(status.payeeStatus());
}

libtorrent::tcp::endpoint SellerPeerPluginViewModel::endPoint() const {
    return _endPoint;
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
