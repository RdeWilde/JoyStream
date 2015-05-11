#include "TorrentViewModel.hpp"
#include "peerPluginViewModel.hpp"
#include "SellerTorrentPluginViewModel.hpp"
#include "BuyerTorrentPluginViewModel.hpp"
#include "extension/PeerPlugin.hpp"

TorrentViewModel::TorrentViewModel()
    : _pluginInstalled(PluginInstalled::None)
    , _sellerTorrentPluginViewModel(NULL)
    , _buyerTorrentPluginViewModel(NULL) {
}

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

void TorrentViewModel::addPlugin(const SellerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::None);
    _pluginInstalled = PluginInstalled::Seller;

    _sellerTorrentPluginViewModel = new SellerTorrentPluginViewModel(status);

    emit pluginInstalledChanged(PluginInstalled::Seller);
}

void TorrentViewModel::addPlugin(const BuyerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::None);
    _pluginInstalled = PluginInstalled::Buyer;

    _buyerTorrentPluginViewModel = new BuyerTorrentPluginViewModel(status);

    emit pluginInstalledChanged(PluginInstalled::Buyer);
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
    return _infoHash();
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

void TorrentViewModel::pause() {
    emit pause(_infoHash);
}

void TorrentViewModel::start() {
    emit start(_infoHash);
}

void TorrentViewModel::remove() {
    emit remove(_infoHash);
}

void TorrentViewModel::showExtensionDialog() {
    emit showExtensionDialog(_infoHash);
}
