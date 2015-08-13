/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef BUYER_TORRENT_PLUGIN_VIEW_MODEL_HPP
#define BUYER_TORRENT_PLUGIN_VIEW_MODEL_HPP

#include <core/extension/BuyerTorrentPlugin.hpp>
#include <core/controller/PayorViewModel.hpp>

#include <libtorrent/socket_io.hpp>

class BuyerPeerPluginViewModel;

class BuyerTorrentPluginViewModel : public QObject {

    Q_OBJECT

public:

    // Constructor
    BuyerTorrentPluginViewModel(QObject * parent,
                                const BuyerTorrentPlugin::Status & status);

    // Destructor
    //~BuyerTorrentPluginViewModel();

    // Add a model view for a new buyer peer plugin
    void addPeer(const libtorrent::tcp::endpoint & endPoint, const BuyerPeerPlugin::Status & status);

    // Remove a model view for an existing seller peer plugin
    void removePeer(const libtorrent::tcp::endpoint & endPoint);

    // Update
    void update(const BuyerTorrentPlugin::Status & status);

    // Update statics: does not send signals
    void setStatics(const BuyerTorrentPlugin::Status & status);

    // Getters
    BuyerTorrentPlugin::State state() const;

    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> buyerPeerPluginViewModels() const;

    const PayorViewModel * payorViewModel() const;

    quint32 numberOfClassicPeers() const;
    quint32 numberOfObserverPeers() const;
    quint32 numberOfSellerPeers() const;
    quint32 numberOfBuyerPeers() const;
    qint64 balance() const;

signals:

    // State change
    void stateChanged(BuyerTorrentPlugin::State state);

    void numberOfClassicPeersChanged(quint32 num);
    void numberOfObserverPeersChanged(quint32 num);
    void numberOfSellerPeersChanged(quint32 num);
    void numberOfBuyerPeersChanged(quint32 num);
    void balanceChanged(qint64 balance);

    // Peer added
    void peerAdded(const BuyerPeerPluginViewModel * model);
    void peerRemoved(const libtorrent::tcp::endpoint & endPoint);

private:

    // State of plugin
    BuyerTorrentPlugin::State _state;

    // Maps endpoint to view model for corresponding seller buyer plugin
    // ** We use pointers here since they are QObjects, which cannot be copied **
    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> _buyerPeerPluginViewModels;

    // View model of payor
    PayorViewModel _payorViewModel;

    /**
     * Summary statistics
     */
    quint32 _numberOfClassicPeers,
            _numberOfObserverPeers,
            _numberOfSellerPeers,
            _numberOfBuyerPeers;

    qint64 _balance;
};

#endif // BUYER_TORRENT_PLUGIN_VIEW_MODEL_HPP
