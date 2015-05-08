#include "SellerTorrentPluginViewModel.hpp"
#include "TorrentViewModel.hpp"
#include "SellerPeerPluginViewModel.hpp"

const char * SellerTorrentPluginViewModel::columnTitles[] = {"Host", "State", "Contract", "Funds", "Lock", "Price", "#Payments", "Balance"};
const int SellerTorrentPluginViewModel::numberOfColumns = sizeof(SellerTorrentPluginViewModel::columnTitles)/sizeof(char *);

SellerTorrentPluginViewModel::SellerTorrentPluginViewModel(TorrentViewModel * parent, const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration)
    : QObject(parent)
    // TorrentPluginViewModel(infoHash)
    , _infoHash(infoHash)
    , _configuration(configuration) {

    // Add columns to model view model
    for(int i = 0;i < SellerTorrentPluginViewModel::numberOfColumns;i++)
        _sellerPeerPluginTableViewModel.setHorizontalHeaderItem(i, new QStandardItem(SellerTorrentPluginViewModel::columnTitles[i]));
}

void SellerTorrentPluginViewModel::addPeer(const libtorrent::tcp::endpoint & endPoint) { //, const SellerTorrentPlugin::Configuration & configuration) {

    Q_ASSERT(!_sellerPeerPluginViewModels.contains(endPoint));

    // Create view model
    SellerPeerPluginViewModel * sellerPeerPluginViewModel = new SellerPeerPluginViewModel(this, endPoint, &_sellerPeerPluginTableViewModel);

    // Update view model
    //sellerPeerPluginViewModel->updatePrice(configuration.minPrice());

    // Add to map
    _sellerPeerPluginViewModels[endPoint] = sellerPeerPluginViewModel;
}

void SellerTorrentPluginViewModel::update(const SellerTorrentPlugin::Status & status) {

    const QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status> & peerPluginStatuses = status.peerPluginStatuses();

    for(QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status>::const_iterator
        i = peerPluginStatuses.constBegin(),
        end = peerPluginStatuses.constEnd(); i != end;i++)
            updatePeer(i.key(), i.value());
}

void SellerTorrentPluginViewModel::updatePeer(const libtorrent::tcp::endpoint & endPoint, const SellerPeerPlugin::Status & status) {

    Q_ASSERT(_sellerPeerPluginViewModels.contains(endPoint));

    // Update view model
    _sellerPeerPluginViewModels[endPoint]->update(status);
}

QStandardItemModel * SellerTorrentPluginViewModel::sellerPeerPluginTableViewModel() {
    return &_sellerPeerPluginTableViewModel;
}

SellerTorrentPlugin::Configuration SellerTorrentPluginViewModel::configuration() const {
    return _configuration;
}

void SellerTorrentPluginViewModel::setConfiguration(const SellerTorrentPlugin::Configuration & configuration) {
    _configuration = configuration;
}
