/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 1 2016
 */

#include <extension/SellerTorrentPluginStatus.hpp>


namespace joystream {
namespace extension {

    SellerTorrentPluginStatus::SellerTorrentPluginStatus() {
    }

    SellerTorrentPluginStatus::SellerTorrentPluginStatus(quint64 minPrice,
                                        quint32 minLock,
                                        quint64 minFeePerByte,
                                        quint32 maxNumberOfSellers,
                                        quint32 maxContractConfirmationDelay,
                                        qint64 balance,
                                        const QMap<libtorrent::tcp::endpoint, SellerPeerPluginStatus> & peerPluginStatuses)
        : _minPrice(minPrice)
        , _minLock(minLock)
        , _minFeePerByte(minFeePerByte)
        , _maxNumberOfSellers(maxNumberOfSellers)
        , _maxContractConfirmationDelay(maxContractConfirmationDelay)
        , _balance(balance)
        , _peerPluginStatuses(peerPluginStatuses) {
    }

    quint64 SellerTorrentPluginStatus::minPrice() const {
        return _minPrice;
    }

    void SellerTorrentPluginStatus::setMinPrice(quint64 minPrice) {
        _minPrice = minPrice;
    }

    quint32 SellerTorrentPluginStatus::minLock() const {
        return _minLock;
    }

    void SellerTorrentPluginStatus::setMinLock(quint32 minLock) {
        _minLock = minLock;
    }

    quint32 SellerTorrentPluginStatus::maxNumberOfSellers() const {
        return _maxNumberOfSellers;
    }

    void SellerTorrentPluginStatus::setMaxNumberOfSellers(quint32 maxNumberOfSellers) {
        _maxNumberOfSellers = maxNumberOfSellers;
    }

    quint64 SellerTorrentPluginStatus::minFeePerByte() const {
        return _minFeePerByte;
    }

    void SellerTorrentPluginStatus::setMinFeePerByte(quint64 minFeePerByte) {
        _minFeePerByte = minFeePerByte;
    }

    quint32 SellerTorrentPluginStatus::maxContractConfirmationDelay() const {
        return _maxContractConfirmationDelay;
    }

    void SellerTorrentPluginStatus::setMaxContractConfirmationDelay(quint32 maxContractConfirmationDelay) {
        _maxContractConfirmationDelay = maxContractConfirmationDelay;
    }

    qint64 SellerTorrentPluginStatus::balance() const {
        return _balance;
    }

    void SellerTorrentPluginStatus::setBalance(qint64 balance) {
        _balance = balance;
    }

    QMap<libtorrent::tcp::endpoint, SellerPeerPluginStatus> SellerTorrentPluginStatus::peerPluginStatuses() const {
        return _peerPluginStatuses;
    }

    void SellerTorrentPluginStatus::setPeerPluginStatuses(const QMap<libtorrent::tcp::endpoint, SellerPeerPluginStatus> & peerStatuses) {
        _peerPluginStatuses = peerStatuses;
    }

}
}
