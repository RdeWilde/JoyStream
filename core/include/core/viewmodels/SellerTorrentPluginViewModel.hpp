/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef SELLER_TORRENT_PLUGIN_VIEW_MODEL_HPP
#define SELLER_TORRENT_PLUGIN_VIEW_MODEL_HPP

#include <core/extension/SellerTorrentPlugin.hpp>

#include <libtorrent/socket_io.hpp>

class SellerPeerPluginViewModel;

class SellerTorrentPluginViewModel : public QObject
{
    Q_OBJECT

public:

    // Constructor
    SellerTorrentPluginViewModel(QObject * parent, const SellerTorrentPlugin::Status & status);

    // Add a model view for a new seller peer plugin
    void addPeer(const libtorrent::tcp::endpoint & endPoint, const SellerPeerPlugin::Status & status);

    // Remove a model view for an existing seller peer plugin
    void removePeer(const libtorrent::tcp::endpoint & endPoint);

    // Update
    void update(const SellerTorrentPlugin::Status & status);

    // Update statics: does not send signals
    void setStatics(const SellerTorrentPlugin::Status & status);

    // Getters
    quint64 minPrice() const;
    quint32 minLock() const;
    quint64 minFeePerByte() const;
    quint32 maxNumberOfSellers() const;
    quint32 maxContractConfirmationDelay() const;
    QMap<libtorrent::tcp::endpoint, SellerPeerPluginViewModel *> sellerPeerPluginViewModels() const;

    quint32 numberOfClassicPeers() const;
    quint32 numberOfObserverPeers() const;
    quint32 numberOfSellerPeers() const;
    quint32 numberOfBuyerPeers() const;
    qint64 balance() const;

signals:

    // Status changed
    void minPriceChanged(quint64 minPrice);
    void minLockChanged(quint32 minLock);
    void minFeePerByteChanged(quint64 minFeePerByte);
    void maxNumberOfSellersChanged(quint32 maxNumberOfSellers);
    void maxContractConfirmationDelayChanged(quint32 maxContractConfirmationDelay);

    void numberOfClassicPeersChanged(quint32 num);
    void numberOfObserverPeersChanged(quint32 num);
    void numberOfSellerPeersChanged(quint32 num);
    void numberOfBuyerPeersChanged(quint32 num);
    void balanceChanged(qint64 balance);

    // Peer added
    void peerAdded(const SellerPeerPluginViewModel * model);
    void peerRemoved(const libtorrent::tcp::endpoint & endPoint);

private:

    // Maximum price accepted (satoshies)
    quint64 _minPrice;

    // Minimum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
    quint32 _minLock;

    // Minimum fee per byte in contract transaction (satoshies)
    quint64 _minFeePerByte;

    // Number of sellers
    quint32 _maxNumberOfSellers;

    // Maximum time (s) for which seller is willing to seed without contract getting at least one confirmation
    quint32 _maxContractConfirmationDelay;

    // Maps endpoint to view model for corresponding seller peer plugin
    // We use pointers since SellerPeerPluginViewModel are QObjects
    QMap<libtorrent::tcp::endpoint, SellerPeerPluginViewModel *> _sellerPeerPluginViewModels;

    /**
     * Summary statistics
     */
    quint32 _numberOfClassicPeers,
            _numberOfObserverPeers,
            _numberOfSellerPeers,
            _numberOfBuyerPeers;
    qint64 _balance;
};

#endif // SELLER_TORRENT_PLUGIN_VIEW_MODEL_HPP
