#include "BuyerPeerPluginViewModel.hpp"
#include "BuyerTorrentPluginViewModel.hpp"

BuyerPeerPluginViewModel::BuyerPeerPluginViewModel(QObject * parent,
                                                   const libtorrent::tcp::endpoint & endPoint,
                                                   const BuyerPeerPlugin::Status & status)
    : QObject(parent)
    , _endPoint(endPoint)
    , _status(status) {
}

void BuyerPeerPluginViewModel::update(const BuyerPeerPlugin::Status & status) {

    if(_status.clientState() != status.clientState()) {
        _status.setClientState(status.clientState());
        emit clientStateChanged(status.clientState());
    }

    if(_status.payorSlot() != status.payorSlot()) {
        _status.setPayorSlot(status.payorSlot());
        emit payorSlotChanged(status.payorSlot());
    }
}

libtorrent::tcp::endpoint BuyerPeerPluginViewModel::endPoint() const {
    return _endPoint;
}

BuyerPeerPlugin::Status BuyerPeerPluginViewModel::status() const {
    return _status;
}
