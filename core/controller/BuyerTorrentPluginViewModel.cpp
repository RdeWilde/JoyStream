/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

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


    // Compute and save new values
    setStatics(status);
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

void BuyerTorrentPluginViewModel::removePeer(const libtorrent::tcp::endpoint & endPoint) {

    Q_ASSERT(_buyerPeerPluginViewModels.contains(endPoint));

    // Remove view model
    BuyerPeerPluginViewModel * model = _buyerPeerPluginViewModels.take(endPoint);

    // Delete model
    delete model;

    // Notify about event
    emit peerRemoved(endPoint);
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

    /*
     *  Update statistics
     */

    // Copy old stats
    quint32 numberOfClassicPeers = _numberOfClassicPeers,
            numberOfObserverPeers = _numberOfObserverPeers,
            numberOfSellerPeers = _numberOfSellerPeers,
            numberOfBuyerPeers = _numberOfBuyerPeers;

    qint64 balance = _balance;

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

void BuyerTorrentPluginViewModel::setStatics(const BuyerTorrentPlugin::Status & status) {

    // Reset counters
    _numberOfClassicPeers = 0;
    _numberOfObserverPeers = 0;
    _numberOfSellerPeers = 0;
    _numberOfBuyerPeers = 0;
    _balance = 0;

    // Update peers
    QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> peerPluginStatuses = status.peerPluginStatuses();

    for(QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status>::const_iterator
        i = peerPluginStatuses.constBegin(),
        end = peerPluginStatuses.constEnd();
        i != end;i++) {

        // Get endpoint
        const BuyerPeerPlugin::Status & peer = i.value();

        // Check mode towards mode counters
        BEPSupportStatus supportStatus = peer.peerJoyStreamBEPSupportStatus();

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
    }

    // Iterate payor status
    const Payor::Status & payorStatus = status.payor();
    QVector<Payor::Channel::Status> channels = payorStatus.channels();

    // Total amount of funds in outputs, is used to deduce tx fee
    quint64 netOutputValue = payorStatus.changeValue();

    for(QVector<Payor::Channel::Status>::const_iterator
        i = channels.constBegin(),
        end = channels.constEnd();
        i != end;i++) {

        // Get channel status
        const Payor::Channel::Status & channelStatus = *i;

        // Count towards balance
        _balance -= channelStatus.price() * channelStatus.numberOfPaymentsMade();

        // Count output funds
        netOutputValue += channelStatus.funds();
    }

    // Count spent fees towards total spending in plugin IF  payor has actually started
    quint64 contractFee = payorStatus.utxo().value() - netOutputValue;
    Q_ASSERT(contractFee >= 0);

    if(_state != BuyerTorrentPlugin::State::waiting_for_payor_to_be_ready)
        _balance -= contractFee;
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

quint32 BuyerTorrentPluginViewModel::numberOfClassicPeers() const {
    return _numberOfClassicPeers;
}

quint32 BuyerTorrentPluginViewModel::numberOfObserverPeers() const {
    return _numberOfObserverPeers;
}

quint32 BuyerTorrentPluginViewModel::numberOfSellerPeers() const {
    return _numberOfSellerPeers;
}

quint32 BuyerTorrentPluginViewModel::numberOfBuyerPeers() const {
    return _numberOfBuyerPeers;
}

qint64 BuyerTorrentPluginViewModel::balance() const {
    return _balance;
}
