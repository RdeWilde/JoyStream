/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#include <extension/configuration/BuyerTorrentPlugin.hpp>

#include <QLoggingCategory>

#include <libtorrent/entry.hpp>

namespace joystream {
namespace extension {
namespace configuration {

    BuyerTorrentPlugin::BuyerTorrentPlugin() {
    }

    BuyerTorrentPlugin::BuyerTorrentPlugin(bool enableBanningSets,
                                                     quint64 maxPrice,
                                                     quint32 maxLock,
                                                     quint64 maxFeePerByte,
                                                     quint32 numberOfSellers)
        : TorrentPlugin::TorrentPlugin(enableBanningSets)
        , _maxPrice(maxPrice)
        , _maxLock(maxLock)
        , _maxFeePerKb(maxFeePerByte)
        , _numberOfSellers(numberOfSellers) {
    }

    BuyerTorrentPlugin::BuyerTorrentPlugin(const libtorrent::entry::dictionary_type & dictionaryEntry)
        : TorrentPlugin::TorrentPlugin(dictionaryEntry) {
        // IMPLEMENT LATER
    }

    void BuyerTorrentPlugin::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

        // Call super version
        //TorrentPlugin::Configuration::toDictionaryEntry(dictionaryEntry);

        // IMPLEMENT LATER
    }

    PluginMode BuyerTorrentPlugin::pluginMode() const {
        return PluginMode::Buyer;
    }

    quint64 BuyerTorrentPlugin::maxPrice() const {
        return _maxPrice;
    }

    void BuyerTorrentPlugin::setMaxPrice(quint64 maxPrice) {
        _maxPrice = maxPrice;
    }

    quint32 BuyerTorrentPlugin::maxLock() const{
        return _maxLock;
    }

    void BuyerTorrentPlugin::setMaxLock(quint32 maxLock) {
        _maxLock = maxLock;
    }

    quint64 BuyerTorrentPlugin::maxFeePerKb() const {
        return _maxFeePerKb;
    }

    void BuyerTorrentPlugin::setMaxFeePerKb(quint64 maxFeePerByte) {
        _maxFeePerKb = maxFeePerByte;
    }

    quint32 BuyerTorrentPlugin::numberOfSellers() const {
        return _numberOfSellers;
    }

    void BuyerTorrentPlugin::setNumberOfSellers(quint32 numberOfSellers) {
        _numberOfSellers = numberOfSellers;
    }

}
}
}
