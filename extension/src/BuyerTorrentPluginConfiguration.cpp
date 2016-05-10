/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#include <extension/BuyerTorrentPluginConfiguration.hpp>
#include <extension/PluginMode.hpp>

#include <libtorrent/entry.hpp>

namespace joystream {
namespace extension {

    BuyerTorrentPluginConfiguration::BuyerTorrentPluginConfiguration() {
    }

    BuyerTorrentPluginConfiguration::BuyerTorrentPluginConfiguration(bool enableBanningSets,
                                                     quint64 maxPrice,
                                                     quint32 maxLock,
                                                     quint64 maxFeePerByte,
                                                     quint32 numberOfSellers)
        : TorrentPluginConfiguration::TorrentPluginConfiguration(enableBanningSets)
        , _maxPrice(maxPrice)
        , _maxLock(maxLock)
        , _maxFeePerKb(maxFeePerByte)
        , _numberOfSellers(numberOfSellers) {
    }

    BuyerTorrentPluginConfiguration::BuyerTorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry)
        : TorrentPluginConfiguration::TorrentPluginConfiguration(dictionaryEntry) {
        // IMPLEMENT LATER
    }

    void BuyerTorrentPluginConfiguration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

        // Call super version
        //TorrentPlugin::Configuration::toDictionaryEntry(dictionaryEntry);

        // IMPLEMENT LATER
    }

    PluginMode BuyerTorrentPluginConfiguration::pluginMode() const {
        return PluginMode::Buyer;
    }

    quint64 BuyerTorrentPluginConfiguration::maxPrice() const {
        return _maxPrice;
    }

    void BuyerTorrentPluginConfiguration::setMaxPrice(quint64 maxPrice) {
        _maxPrice = maxPrice;
    }

    quint32 BuyerTorrentPluginConfiguration::maxLock() const{
        return _maxLock;
    }

    void BuyerTorrentPluginConfiguration::setMaxLock(quint32 maxLock) {
        _maxLock = maxLock;
    }

    quint64 BuyerTorrentPluginConfiguration::maxFeePerKb() const {
        return _maxFeePerKb;
    }

    void BuyerTorrentPluginConfiguration::setMaxFeePerKb(quint64 maxFeePerByte) {
        _maxFeePerKb = maxFeePerByte;
    }

    quint32 BuyerTorrentPluginConfiguration::numberOfSellers() const {
        return _numberOfSellers;
    }

    void BuyerTorrentPluginConfiguration::setNumberOfSellers(quint32 numberOfSellers) {
        _numberOfSellers = numberOfSellers;
    }

}
}
