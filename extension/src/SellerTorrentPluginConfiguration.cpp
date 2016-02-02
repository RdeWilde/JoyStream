/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 1 2016
 */

#include <extension/SellerTorrentPluginConfiguration.hpp>
#include <extension/PluginMode.hpp>

namespace joystream {
namespace extension {

    SellerTorrentPluginConfiguration::SellerTorrentPluginConfiguration() {

    }

    SellerTorrentPluginConfiguration::SellerTorrentPluginConfiguration(bool enableBanningSets,
                                                      quint64 minPrice,
                                                      quint32 minLock,
                                                      quint64 minFeePerByte,
                                                      quint32 maxNumberOfSellers,
                                                      quint32 maxContractConfirmationDelay)
        : TorrentPluginConfiguration(enableBanningSets)
        , _minPrice(minPrice)
        , _minLock(minLock)
        , _minFeePerByte(minFeePerByte)
        , _maxNumberOfSellers(maxNumberOfSellers)
        , _maxContractConfirmationDelay(maxContractConfirmationDelay) {
    }

    SellerTorrentPluginConfiguration::SellerTorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry) {
    }

    quint64 SellerTorrentPluginConfiguration::minPrice() const {
        return _minPrice;
    }

    void SellerTorrentPluginConfiguration::setMinPrice(quint64 minPrice) {
        _minPrice = minPrice;
    }

    quint32 SellerTorrentPluginConfiguration::minLock() const {
        return _minLock;
    }

    void SellerTorrentPluginConfiguration::setMinLock(quint32 minLock) {
        _minLock = minLock;
    }

    quint64 SellerTorrentPluginConfiguration::minFeePerByte() const {
        return _minFeePerByte;
    }

    void SellerTorrentPluginConfiguration::setMinFeePerByte(quint64 minFeePerByte) {
        _minFeePerByte = minFeePerByte;
    }

    quint32 SellerTorrentPluginConfiguration::maxNumberOfSellers() const {
        return _maxNumberOfSellers;
    }

    void SellerTorrentPluginConfiguration::setMaxNumberOfSellers(quint32 minNumberOfSellers) {
        _maxNumberOfSellers = minNumberOfSellers;
    }

    quint32 SellerTorrentPluginConfiguration::maxContractConfirmationDelay() const {
        return _maxContractConfirmationDelay;
    }

    void SellerTorrentPluginConfiguration::setMaxContractConfirmationDelay(quint32 maxContractConfirmationDelay) {
        _maxContractConfirmationDelay = maxContractConfirmationDelay;
    }

    PluginMode SellerTorrentPluginConfiguration::pluginMode() const {
        return PluginMode::Seller;
    }
}
}
