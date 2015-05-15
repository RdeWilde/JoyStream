#include "BuyerTorrentPluginViewModel.hpp"
#include "TorrentViewModel.hpp"
#include "BuyerPeerPluginViewModel.hpp"

BuyerTorrentPluginViewModel::BuyerTorrentPluginViewModel(QObject * parent,
                                                         const BuyerTorrentPlugin::Status & status)
    : QObject(parent)
    , _state(status.state())
    , _payorViewModel(this, status.payor()) {

    // Create view models for all peer plugins
    QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> statuses = status.peerPluginStatuses();

    for(QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status>::const_iterator
        i = statuses.constBegin(),
        end = statuses.constEnd();i != end;i++)
        addPeer(i.key(), i.value());
}

/**
BuyerTorrentPluginViewModel::~BuyerTorrentPluginViewModel() {

    for(QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *>::const_iterator
        i = _buyerPeerPluginViewModels.constBegin(),
        end = _buyerPeerPluginViewModels.constEnd();
        i != end;i++)
        delete i.value();
}
*/

void BuyerTorrentPluginViewModel::addPeer(const libtorrent::tcp::endpoint & endPoint, const BuyerPeerPlugin::Status & status) {

    Q_ASSERT(!_buyerPeerPluginViewModels.contains(endPoint));

    // Create new view model
    BuyerPeerPluginViewModel * model = new BuyerPeerPluginViewModel(this, endPoint, status);

    // Add to map
    _buyerPeerPluginViewModels[endPoint] = model;

    // Notify that peer was added
    emit peerAdded(model);
}

void BuyerTorrentPluginViewModel::update(const BuyerTorrentPlugin::Status & status) {

    if(_state != status.state()) {
        _state = status.state();
        emit stateChanged(_state);
    }

    // Peer statuses
    const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> & peerPluginStatuses = status.peerPluginStatuses();

    for(QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status>::const_iterator
        i = peerPluginStatuses.constBegin(),
        end = peerPluginStatuses.constEnd(); i != end;i++) {

        // Get peer endpoint
        libtorrent::tcp::endpoint endPoint = i.key();

        Q_ASSERT(_buyerPeerPluginViewModels.contains(endPoint));

        // Update status
        _buyerPeerPluginViewModels[endPoint]->update(i.value());
    }


    // Payor status
    _payorViewModel.update(status.payor());
}

BuyerTorrentPlugin::State BuyerTorrentPluginViewModel::state() const {
    return _state;
}

QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> BuyerTorrentPluginViewModel::buyerPeerPluginViewModels() const {
    return _buyerPeerPluginViewModels;
}

const PayorViewModel * BuyerTorrentPluginViewModel::payorViewModel() const {
    return &_payorViewModel;
}
