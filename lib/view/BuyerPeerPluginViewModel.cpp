#include "BuyerPeerPluginViewModel.hpp"
#include "BuyerTorrentPluginViewModel.hpp"

BuyerPeerPluginViewModel::BuyerPeerPluginViewModel(QObject * parent, const BuyerPeerPlugin::Status & status)
    : QObject(parent)
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

BuyerPeerPlugin::Status BuyerPeerPluginViewModel::status() const {
    return _status;
}
