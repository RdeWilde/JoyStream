#include "TorrentViewModel.hpp"
#include "peerPluginViewModel.hpp"
#include "SellerTorrentPluginViewModel.hpp"
#include "BuyerTorrentPluginViewModel.hpp"
#include "extension/PeerPlugin.hpp"

#include <QDebug>

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
    , _buyerTorrentPluginViewModel(NULL)
    , _currentlyServicingRangeRequest(false) {

    // Where does file start end? what pieces,and what offset in pieces?
    //
    //int _firstPieceWhereStreamBegins;
    //int _lastPieceWhereStreamEnds;
    //_defaultRangeLength = how much
    //_contentType = what type of data is this
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

    //emit pluginInstalledChanged(PluginInstalled::Seller);
    emit startedSellerTorrentPlugin(_sellerTorrentPluginViewModel);
}

void TorrentViewModel::addPlugin(const BuyerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::None);
    _pluginInstalled = PluginInstalled::Buyer;

    // Create view model and save in map
    _buyerTorrentPluginViewModel = new BuyerTorrentPluginViewModel(this, status);

    //emit pluginInstalledChanged(PluginInstalled::Buyer);
    emit startedBuyerTorrentPlugin(_buyerTorrentPluginViewModel);
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

void TorrentViewModel::removePeer(const libtorrent::tcp::endpoint & endPoint) {

    switch(_pluginInstalled) {

        case PluginInstalled::None:
            Q_ASSERT(false);
            break;

        case PluginInstalled::Buyer:
            Q_ASSERT(_sellerTorrentPluginViewModel == NULL);
            Q_ASSERT(_buyerTorrentPluginViewModel != NULL);
            _buyerTorrentPluginViewModel->removePeer(endPoint);
            break;

        case PluginInstalled::Seller:
            Q_ASSERT(_sellerTorrentPluginViewModel != NULL);
            Q_ASSERT(_buyerTorrentPluginViewModel == NULL);
            _sellerTorrentPluginViewModel->removePeer(endPoint);
            break;

        case PluginInstalled::Observer:
            Q_ASSERT(false);
            break;

        default:
            Q_ASSERT(false);
    }

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

void TorrentViewModel::pieceRead(const libtorrent::error_code & ec,
                                 const boost::shared_array<char> & buffer,
                                 int piece,
                                 int size) {

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

void TorrentViewModel::getRange(int start, int end) {

    /**
    void dataRangeRead(const QString & contentType,
                       int start,
                       int end,
                       int total,
                       const QVector<Stream::Piece> pieces,
                       int offsetInFirstPiece,
                       int offsetInLastPiece);

    // A request starting at given position was invalid
    void receivedInvalidRange(int start);
    */

}

void TorrentViewModel::getStart(int start) {
    getRange(start, start + _defaultRangeLength);
}

void TorrentViewModel::errorOccured(Stream::Error errorOccured) {
    qDebug() << "TorrentViewModel::errorOccured";
}

/**
quint32 TorrentViewModel::numberOfClassicPeers() {

}

quint32 TorrentViewModel::numberOfObserverPeers() {

}

quint32 TorrentViewModel::numberOfSellerPeers() {

}

quint32 TorrentViewModel::numberOfBuyerPeers() {

}

quint64 TorrentViewModel::balance() {

}
*/

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
