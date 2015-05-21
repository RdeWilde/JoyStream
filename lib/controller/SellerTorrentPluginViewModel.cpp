#include "SellerTorrentPluginViewModel.hpp"
#include "TorrentViewModel.hpp"
#include "SellerPeerPluginViewModel.hpp"

SellerTorrentPluginViewModel::SellerTorrentPluginViewModel(QObject * parent, const SellerTorrentPlugin::Status & status)
    : QObject(parent)
    , _minPrice(status.minPrice())
    , _minLock(status.minLock())
    , _minFeePerByte(status.minFeePerByte())
    , _maxNumberOfSellers(status.maxNumberOfSellers())
    , _maxContractConfirmationDelay(status.maxContractConfirmationDelay())
    /**
    , _numberOfClassicPeers(0)
    , _numberOfObserverPeers(0)
    , _numberOfSellerPeers(0)
    , _numberOfBuyerPeers(0)
    , _balance(0)*/
    {

    // Add peers
    QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status> peerPluginStatuses = status.peerPluginStatuses();

    for(QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status>::const_iterator
        i = peerPluginStatuses.constBegin(),
        end = peerPluginStatuses.constEnd();
        i != end;i++)
        addPeer(i.key(), i.value());

    // Sets statistics variables
    setStatics(status);
}

/**
SellerTorrentPluginViewModel::~SellerTorrentPluginViewModel() {

    for(QMap<libtorrent::tcp::endpoint, SellerPeerPluginViewModel *>::const_iterator
        i = _sellerPeerPluginViewModels.constBegin(),
        end = _sellerPeerPluginViewModels.constEnd();
        i != end;i++)
        delete i.value();
}

*/

#include <libtorrent/socket_io.hpp> // libtorrent::print_endpoint
#include <QDebug>

void SellerTorrentPluginViewModel::addPeer(const libtorrent::tcp::endpoint & endPoint, const SellerPeerPlugin::Status & status) {

    /**
    // Convert to std::string
    std::string endPointStdString = libtorrent::print_endpoint(endPoint);

    // Convert to QString
    QString endPointQString = endPointStdString.c_str();

    qDebug() << "addPeer:" << endPointQString;
    */

    Q_ASSERT(!_sellerPeerPluginViewModels.contains(endPoint));

    // Create new view model
    SellerPeerPluginViewModel * model = new SellerPeerPluginViewModel(this, endPoint, status);

    // Add to map
    _sellerPeerPluginViewModels[endPoint] = model;

    // Notify that peer was added
    emit peerAdded(model);
}

void SellerTorrentPluginViewModel::removePeer(const libtorrent::tcp::endpoint & endPoint) {

    Q_ASSERT(_sellerPeerPluginViewModels.contains(endPoint));

    // Remove view model
    SellerPeerPluginViewModel * model = _sellerPeerPluginViewModels.take(endPoint);

    // Delete model
    delete model;

    // Notify about event
    emit peerRemoved(endPoint);
}

void SellerTorrentPluginViewModel::update(const SellerTorrentPlugin::Status & status) {

    if(_minPrice != status.minPrice()) {
        _minPrice = status.minPrice();
        emit minPriceChanged(status.minPrice());
    }

    if(_minLock != status.minLock()) {
        _minLock = status.minLock();
        emit minLockChanged(status.minLock());
    }

    if(_minFeePerByte != status.minFeePerByte()) {
        _minFeePerByte = status.minFeePerByte();
        emit minFeePerByteChanged(status.minFeePerByte());
    }

    if(_maxNumberOfSellers != status.maxNumberOfSellers()) {
        _maxNumberOfSellers = status.maxNumberOfSellers();
        emit maxNumberOfSellersChanged(status.maxNumberOfSellers());
    }

    if(_maxContractConfirmationDelay != status.maxContractConfirmationDelay()) {
        _maxContractConfirmationDelay = status.maxContractConfirmationDelay();
        emit maxContractConfirmationDelayChanged(status.maxContractConfirmationDelay());
    }

    // Update peers
    QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status> peerPluginStatuses = status.peerPluginStatuses();

    for(QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status>::const_iterator
        i = peerPluginStatuses.constBegin(),
        end = peerPluginStatuses.constEnd();
        i != end;i++) {

        // Get endpoint
        libtorrent::tcp::endpoint endPoint = i.key();

        Q_ASSERT(_sellerPeerPluginViewModels.contains(endPoint));

        // Update
        _sellerPeerPluginViewModels[endPoint]->update(i.value());
    }

    /*
     *  Update statistics
     */

    // Copy old stats
    quint32 numberOfClassicPeers = _numberOfClassicPeers,
            numberOfObserverPeers = _numberOfObserverPeers,
            numberOfSellerPeers = _numberOfSellerPeers,
            numberOfBuyerPeers = _numberOfBuyerPeers;

    quint64 balance = _balance;

    // Recompute and save new values
    setStatics(status);

    // Send signal for changed values
    if(numberOfClassicPeers != _numberOfClassicPeers)
        emit numberOfClassicPeersChanged(_numberOfClassicPeers);

    if(numberOfObserverPeers != _numberOfObserverPeers)
        emit numberOfObserverPeersChanged(_numberOfObserverPeers);

    if(numberOfSellerPeers != _numberOfSellerPeers)
        emit numberOfSellerPeersChanged(_numberOfSellerPeers);

    if(numberOfBuyerPeers != _numberOfBuyerPeers)
        emit numberOfBuyerPeersChanged(_numberOfBuyerPeers);

    if(balance != _balance)
        emit balanceChanged(_balance);

}

void SellerTorrentPluginViewModel::setStatics(const SellerTorrentPlugin::Status & status) {

    // Reset counters
    _numberOfClassicPeers = 0;
    _numberOfObserverPeers = 0;
    _numberOfSellerPeers = 0;
    _numberOfBuyerPeers = 0;
    _balance = 0;

    // Update peers
    QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status> peerPluginStatuses = status.peerPluginStatuses();

    for(QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status>::const_iterator
        i = peerPluginStatuses.constBegin(),
        end = peerPluginStatuses.constEnd();
        i != end;i++) {

        // Get endpoint
        const SellerPeerPlugin::Status & peer = i.value();

        // Check mode towards mode counters
        BEPSupportStatus supportStatus = peer.peerBitSwaprBEPSupportStatus();

        if(supportStatus == BEPSupportStatus::supported) {

            switch(peer.peerModeAnnounced()) {

                case PeerPlugin::PeerModeAnnounced::none:
                    break;
                case PeerPlugin::PeerModeAnnounced::buyer:
                    _numberOfBuyerPeers++;
                    break;
                case PeerPlugin::PeerModeAnnounced::seller:
                    _numberOfSellerPeers++;
                    break;
                case PeerPlugin::PeerModeAnnounced::observer:
                    _numberOfObserverPeers;
                    break;
            }

        } else if(supportStatus == BEPSupportStatus::not_supported)
            _numberOfClassicPeers++;

        // count balance
        const Payee::Status & payeeStatus = peer.payeeStatus();
        _balance += payeeStatus.price() * payeeStatus.numberOfPaymentsMade();
    }

}

quint64 SellerTorrentPluginViewModel::minPrice() const {
    return _minPrice;
}

quint32 SellerTorrentPluginViewModel::minLock() const {
    return _minLock;
}

quint64 SellerTorrentPluginViewModel::minFeePerByte() const {
    return _minFeePerByte;
}

quint32 SellerTorrentPluginViewModel::maxNumberOfSellers() const {
    return _maxNumberOfSellers;
}

quint32 SellerTorrentPluginViewModel::maxContractConfirmationDelay() const {
    return _maxContractConfirmationDelay;
}

QMap<libtorrent::tcp::endpoint, SellerPeerPluginViewModel *> SellerTorrentPluginViewModel::sellerPeerPluginViewModels() const {
    return _sellerPeerPluginViewModels;
}

quint32 SellerTorrentPluginViewModel::numberOfClassicPeers() const {
   return _numberOfClassicPeers;
}

quint32 SellerTorrentPluginViewModel::numberOfObserverPeers() const {
    return _numberOfObserverPeers;
}

quint32 SellerTorrentPluginViewModel::numberOfSellerPeers() const {
    return _numberOfSellerPeers;
}

quint32 SellerTorrentPluginViewModel::numberOfBuyerPeers() const {
    return _numberOfBuyerPeers;
}

quint64 SellerTorrentPluginViewModel::balance() const {
    return _balance;
}
