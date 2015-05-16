#include "TorrentViewModel.hpp"
#include "peerPluginViewModel.hpp"
#include "SellerTorrentPluginViewModel.hpp"
#include "BuyerTorrentPluginViewModel.hpp"
#include "extension/PeerPlugin.hpp"

TorrentViewModel::TorrentViewModel(const libtorrent::sha1_hash & infoHash,
                                   const std::string & name,
                                   const std::string & savePath,
                                   libtorrent::torrent_info * torrentInfo)
    : _infoHash(infoHash)
    , _name(QString::fromStdString(name))
    , _savePath(QString::fromStdString(savePath))
    , _torrentInfo(torrentInfo)
    , _pluginInstalled(PluginInstalled::None)
    , _sellerTorrentPluginViewModel(NULL)
    , _buyerTorrentPluginViewModel(NULL) {
}

/**
TorrentViewModel::TorrentViewModel(SellerTorrentPluginViewModel * model)
    : _pluginInstalled(PluginInstalled::Seller)
    , _sellerTorrentPluginViewModel(model)
    , _buyerTorrentPluginViewModel(NULL) {
}

TorrentViewModel::TorrentViewModel(BuyerTorrentPluginViewModel * model)
    : _pluginInstalled(PluginInstalled::Buyer)
    , _sellerTorrentPluginViewModel(NULL)
    , _buyerTorrentPluginViewModel(model) {
}
*/

/**
TorrentViewModel::~TorrentViewModel() {

    switch(_pluginInstalled) {

        case PluginInstalled::Seller:
            delete _sellerTorrentPluginViewModel;
            Q_ASSERT(_buyerTorrentPluginViewModel == NULL);

        case PluginInstalled::Buyer:
            delete _buyerTorrentPluginViewModel;
            Q_ASSERT(_sellerTorrentPluginViewModel == NULL);

        case PluginInstalled::Observer:
            Q_ASSERT(false);

        case PluginInstalled::None:
            Q_ASSERT(_buyerTorrentPluginViewModel == NULL);
            Q_ASSERT(_sellerTorrentPluginViewModel == NULL);
    }
}
*/

void TorrentViewModel::addPlugin(const SellerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::None);
    _pluginInstalled = PluginInstalled::Seller;

    _sellerTorrentPluginViewModel = new SellerTorrentPluginViewModel(this, status);

    emit pluginInstalledChanged(PluginInstalled::Seller);
}

void TorrentViewModel::addPlugin(const BuyerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::None);
    _pluginInstalled = PluginInstalled::Buyer;

    // Create view model and save in map
    _buyerTorrentPluginViewModel = new BuyerTorrentPluginViewModel(this, status);

    // Connect up slots to
    emit pluginInstalledChanged(PluginInstalled::Buyer);
}

void TorrentViewModel::addPeer(const libtorrent::tcp::endpoint & endPoint, const SellerPeerPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::Seller);
    Q_ASSERT(_sellerTorrentPluginViewModel != NULL);
    Q_ASSERT(_buyerTorrentPluginViewModel == NULL);

    _sellerTorrentPluginViewModel->addPeer(endPoint, status);
}

void TorrentViewModel::addPeer(const libtorrent::tcp::endpoint & endPoint, const BuyerPeerPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::Buyer);
    Q_ASSERT(_sellerTorrentPluginViewModel == NULL);
    Q_ASSERT(_buyerTorrentPluginViewModel != NULL);

    _buyerTorrentPluginViewModel->addPeer(endPoint, status);
}

void TorrentViewModel::update(const libtorrent::torrent_status & status) {

    _status = status;
    emit torrentStatusChanged(status);
}

void TorrentViewModel::update(const SellerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::Seller);
    Q_ASSERT(_sellerTorrentPluginViewModel != NULL);
    Q_ASSERT(_buyerTorrentPluginViewModel == NULL);

    _sellerTorrentPluginViewModel->update(status);
}

void TorrentViewModel::update(const BuyerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::Buyer);
    Q_ASSERT(_sellerTorrentPluginViewModel == NULL);
    Q_ASSERT(_buyerTorrentPluginViewModel != NULL);

    _buyerTorrentPluginViewModel->update(status);
}

libtorrent::sha1_hash TorrentViewModel::infoHash() const {
    return _infoHash;
}

QString TorrentViewModel::name() const {
    return _name;
}

QString TorrentViewModel::savePath() const {
    return _savePath;
}

const libtorrent::torrent_info * TorrentViewModel::torrentInfo() const {
    return _torrentInfo;
}

PluginInstalled TorrentViewModel::pluginInstalled() const {
    return _pluginInstalled;
}

libtorrent::torrent_status TorrentViewModel::status() const {
    return _status;
}

SellerTorrentPluginViewModel * TorrentViewModel::sellerTorrentPluginViewModel() const {
    return _sellerTorrentPluginViewModel;
}

BuyerTorrentPluginViewModel * TorrentViewModel::buyerTorrentPluginViewModel() const {
    return _buyerTorrentPluginViewModel;
}

/**
void TorrentViewModel::showExtensionDialog() {

    Q_ASSERT(_pluginInstalled != PluginInstalled::None);

    switch(_pluginInstalled) {

        case PluginInstalled::Seller:
            Q_ASSERT(_sellerTorrentPluginViewModel != NULL);
            Q_ASSERT(_buyerTorrentPluginViewModel == NULL);

            emit sellerTorrentPluginDialogRequested(_sellerTorrentPluginViewModel);

        case PluginInstalled::Buyer:
            Q_ASSERT(_sellerTorrentPluginViewModel == NULL);
            Q_ASSERT(_buyerTorrentPluginViewModel != NULL);

            emit buyerTorrentPluginDialogRequested(_buyerTorrentPluginViewModel);

        case PluginInstalled::Observer:
            Q_ASSERT(false);

        case PluginInstalled::None:
            Q_ASSERT(_buyerTorrentPluginViewModel == NULL);
            Q_ASSERT(_sellerTorrentPluginViewModel == NULL);
    }
}
*/
