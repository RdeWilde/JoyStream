#include "BuyerTorrentPluginViewModel.hpp"
#include "TorrentViewModel.hpp"
#include "BuyerPeerPluginViewModel.hpp"

const char * BuyerTorrentPluginViewModel::columnTitles[] = {"Host", "State", "Output", "Funds", "Lock", "Price", "#Payments", "Balance"};
const int BuyerTorrentPluginViewModel::numberOfColumns = sizeof(BuyerTorrentPluginViewModel::columnTitles)/sizeof(char *);

BuyerTorrentPluginViewModel::BuyerTorrentPluginViewModel(TorrentViewModel * parent, const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration & configuration)
    : QObject(parent)
    // TorrentPluginViewModel(infoHash)
    , _infoHash(infoHash)
    , _configuration(configuration) {

    // Add columns to model view model
    for(int i = 0;i < SellerTorrentPluginViewModel::numberOfColumns;i++)
        _buyerPeerPluginTableViewModel.setHorizontalHeaderItem(i, new QStandardItem(SellerTorrentPluginViewModel::columnTitles[i]));

}

void BuyerTorrentPluginViewModel::addPeer(const libtorrent::tcp::endpoint & endPoint) {

    Q_ASSERT(!_buyerPeerPluginViewModels.contains(endPoint));

    // Create view model
    SellerPeerPluginViewModel * buyerPeerPluginViewModel = new BuyerPeerPluginViewModel(this, endPoint, &_buyerPeerPluginTableViewModel);

    // Add to map
    _buyerPeerPluginViewModels[endPoint] = buyerPeerPluginViewModel;
}

void BuyerTorrentPluginViewModel::update(const BuyerTorrentPlugin::Status & status) {

    // Update flat parts of view model
    BuyerTorrentPlugin::State pluginState = status.state();
    const Payor::Status & payorStatus = status.payor();

    /**
     * Update peers particpating in contract
     */

    // We do not update peer plugins if we dont have anyone part of payor
    if(pluginState == BuyerTorrentPlugin::State::waiting_for_payor_to_be_ready)
        return;

    // Iterate peer statuses, and update status for those part of contract
    const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> & peerPluginStatuses = status.peerPluginStatuses();
    for(QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status>::const_iterator
        i = peerPluginStatuses.constBegin(),
        end = peerPluginStatuses.constEnd(); i != end;i++) {

        // Get Peer Status
        const BuyerPeerPlugin::Status & peerStatus = i.value();

        // If peer is part of payor, then get channel and update peer state
        if(peerStatus.clientState() == ...) {

            // Check that peer model has indeed been created for this peer
            Q_ASSERT(_buyerPeerPluginViewModels.contains(i.key()));

            // Get channel status
            const Payor::Channel::Status & channelStatus = payorStatus.channels()[peerStatus.payorSlot()];

            Q_ASSERT(channelStatus.state() != Payor::Channel::State::unassigned);

            // Update view model for peer
            _buyerPeerPluginViewModels[i.key()]->updatePeer(i.key(), peerStatus, channelStatus);
        }
    }
}

void BuyerTorrentPluginViewModel::updatePeer(const libtorrent::tcp::endpoint & endPoint, const BuyerPeerPlugin::Status & status) {

    Q_ASSERT(_buyerPeerPluginViewModels.contains(endPoint));

    // Update view model
    _buyerPeerPluginViewModels[endPoint]->update(status);
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
