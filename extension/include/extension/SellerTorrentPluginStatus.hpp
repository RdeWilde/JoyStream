/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 1 2016
 */

#ifndef EXTENSION_SELLER_TORRENT_PLUGIN_STATUS_HPP
#define EXTENSION_SELLER_TORRENT_PLUGIN_STATUS_HPP

#include <extension/SellerPeerPluginStatus.hpp>

#include <libtorrent/socket.hpp> // libtorrent::tcp::endpoint

#include <QMap>

namespace joystream {
namespace extension {

    class SellerTorrentPluginStatus {

    public:

        // Default constructor
        SellerTorrentPluginStatus();

        // Constructor from members
        SellerTorrentPluginStatus(quint64 minPrice,
               quint32 minLock,
               quint64 minFeePerByte,
               quint32 maxNumberOfSellers,
               quint32 maxContractConfirmationDelay,
               qint64 balance,
               const QMap<libtorrent::tcp::endpoint, SellerPeerPluginStatus> & peerPluginStatuses);

        // Getters and setters
        quint64 minPrice() const;
        void setMinPrice(quint64 minPrice);

        quint32 minLock() const;
        void setMinLock(quint32 minLock);

        quint64 minFeePerByte() const;
        void setMinFeePerByte(quint64 minFeePerByte);

        quint32 maxNumberOfSellers() const;
        void setMaxNumberOfSellers(quint32 maxNumberOfSellers);

        quint32 maxContractConfirmationDelay() const;
        void setMaxContractConfirmationDelay(quint32 maxContractConfirmationDelay);

        qint64 balance() const;
        void setBalance(qint64 balance);

        QMap<libtorrent::tcp::endpoint, SellerPeerPluginStatus> peerPluginStatuses() const;
        void setPeerPluginStatuses(const QMap<libtorrent::tcp::endpoint, SellerPeerPluginStatus> & peerPluginStatuses);

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

        // Net revenue across all peers since session started
        qint64 _balance;

        // Status of peer plugins
        QMap<libtorrent::tcp::endpoint, SellerPeerPluginStatus> _peerPluginStatuses;
    };

}
}
#endif // EXTENSION_SELLER_TORRENT_PLUGIN_STATUS_HPP
