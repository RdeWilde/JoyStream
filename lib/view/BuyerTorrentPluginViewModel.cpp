#include "BuyerTorrentPluginViewModel.hpp"
#include "TorrentViewModel.hpp"

BuyerTorrentPluginViewModel::BuyerTorrentPluginViewModel(){
}

BuyerTorrentPluginViewModel::BuyerTorrentPluginViewModel(TorrentViewModel * parent, const libtorrent::sha1_hash & infoHash)
    //: TorrentPluginViewModel(infoHash) {
    : QObject(parent)
    , _infoHash(infoHash) {
}

void BuyerTorrentPluginViewModel::update(const BuyerTorrentPlugin::Status & status) {

    /**
    const QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status> & peerStatuses =  status.peerStatuses();

    for(QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status>::const_iterator
        i = peerStatuses.constBegin(),
        end = peerStatuses.constEnd(); i != end;i++)
            updatePeer(i.key(), i.value());
    */
}

void BuyerTorrentPluginViewModel::updatePeer(const libtorrent::tcp::endpoint & endPoint, const BuyerPeerPlugin::Status & status) {

}

QStandardItemModel * BuyerTorrentPluginViewModel::buyerPeerPluginTableViewModel() {
    return &_buyerPeerPluginTableViewModel;
}
