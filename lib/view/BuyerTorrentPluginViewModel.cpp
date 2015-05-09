#include "BuyerTorrentPluginViewModel.hpp"
#include "TorrentViewModel.hpp"
#include "BuyerPeerPluginViewModel.hpp"

BuyerTorrentPluginViewModel::BuyerTorrentPluginViewModel(TorrentViewModel * parent, const BuyerTorrentPlugin::Configuration & configuration, const UnspentP2PKHOutput & utxo)
    : QObject(parent)
    // TorrentPluginViewModel(infoHash)
    //, _infoHash(infoHash)
    , _configuration(configuration)
    , _utxo(utxo)
    , _payorViewModel(configuration.numberOfSellers()) {


}

void BuyerTorrentPluginViewModel::addPeer(const libtorrent::tcp::endpoint & endPoint) {

    Q_ASSERT(!_buyerPeerPluginViewModels.contains(endPoint));

    // Create view model
    BuyerPeerPluginViewModel * buyerPeerPluginViewModel = new BuyerPeerPluginViewModel(this, endPoint, &_buyerPeerPluginTableViewModel);

    // Add to map
    _buyerPeerPluginViewModels[endPoint] = buyerPeerPluginViewModel;
}

void BuyerTorrentPluginViewModel::update(const BuyerTorrentPlugin::Status & status) {

    /**
     * Update flat parts of view model
     */

    if(_state != status.state()) {

        _state = status.state();
        emit stateChanged(_state);
    }

    /**
     * Peer statuses
     */

    // Iterate peer statuses, and update status for those part of contract
    const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> & peerPluginStatuses = status.peerPluginStatuses();

    for(QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status>::const_iterator
        i = peerPluginStatuses.constBegin(),
        end = peerPluginStatuses.constEnd(); i != end;i++)
        _buyerPeerPluginViewModels[i.key()]->update(i.value());

    /**
     * Payor status
     */
    _payorViewModel.update(status.payor());
}

void BuyerTorrentPluginViewModel::updatePeer(const libtorrent::tcp::endpoint & endPoint, const BuyerPeerPlugin::Status & status) {

    Q_ASSERT(_buyerPeerPluginViewModels.contains(endPoint));

    // Update view model
    _buyerPeerPluginViewModels[endPoint]->update(status);
}

 BuyerTorrentPlugin::State BuyerTorrentPluginViewModel::state() const {
     return _state;
 }

 BuyerTorrentPlugin::Configuration BuyerTorrentPluginViewModel::configuration() const {
     return _configuration;
 }

 UnspentP2PKHOutput BuyerTorrentPluginViewModel::utxo() const {
     return _utxo;
 }

 const PayorViewModel * BuyerTorrentPluginViewModel::payorViewModel() const {
     return &_payorViewModel;
 }

QStandardItemModel * BuyerTorrentPluginViewModel::buyerPeerPluginTableViewModel() {
    return &_buyerPeerPluginTableViewModel;
}

QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> BuyerTorrentPluginViewModel::buyerPeerPluginViewModels() const {
    return _buyerPeerPluginViewModels;
}

void BuyerTorrentPluginViewModel::setBuyerPeerPluginViewModels(const QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> &buyerPeerPluginViewModels) {
    _buyerPeerPluginViewModels = buyerPeerPluginViewModels;
}
