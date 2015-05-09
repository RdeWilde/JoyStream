#include "BuyerPeerPluginViewModel.hpp"
#include "BuyerTorrentPluginViewModel.hpp"

BuyerPeerPluginViewModel::BuyerPeerPluginViewModel(BuyerTorrentPluginViewModel * parent, const libtorrent::tcp::endpoint & endPoint)
    : QObject(parent)
    , _endPoint(endPoint) {
}

libtorrent::tcp::endpoint BuyerPeerPluginViewModel::endPoint() const {
    return _endPoint;
}

BuyerPeerPlugin::Status BuyerPeerPluginViewModel::status() const {
    return _status;
}

void BuyerPeerPluginViewModel::update(const BuyerPeerPlugin::Status & status) {

    //Q_ASSERT(no end point information to assert)

    if(_status.clientState() != status.clientState())
        emit clientStateChanged(status.clientState());

    if(_status.payorSlot() != status.payorSlot())
        emit payorSlotChanged(status.payorSlot());

    // Save new status
    _status = status;
}
