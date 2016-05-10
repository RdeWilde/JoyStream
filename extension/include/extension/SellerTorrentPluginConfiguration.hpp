/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 1 2016
 */

#ifndef JOYSTREAM_EXTENSION_SELLER_TORRENT_PLUGIN_CONFIGURATION_HPP
#define JOYSTREAM_EXTENSION_SELLER_TORRENT_PLUGIN_CONFIGURATION_HPP

#include <extension/TorrentPluginConfiguration.hpp>

#include <QtGlobal> // quints for now

namespace joystream {
namespace extension {

    class SellerTorrentPluginConfiguration : public TorrentPluginConfiguration {

    public:

        // Default constructor
        SellerTorrentPluginConfiguration();

        // Constructor from members
        SellerTorrentPluginConfiguration(bool enableBanningSets,
                      quint64 minPrice,
                      quint32 minLock,
                      quint64 minFeePerByte,
                      quint32 maxNumberOfSellers,
                      quint32 maxContractConfirmationDelay);

        // Constructor from dictionary
        SellerTorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

        // Getters and setters
        virtual PluginMode pluginMode() const;

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

    private:

        // Maximum price accepted (satoshies)
        quint64 _minPrice;

        // Minimum lock time
        // Whould have been nice to use QTime, however it is limited to 24h cycle.
        quint32 _minLock;

        // Minimum fee per byte in contract transaction (satoshies)
        quint64 _minFeePerByte;

        // Number of sellers
        quint32 _maxNumberOfSellers;

        // Maximum time (s) for which seller is willing to seed without contract getting at least one confirmation
        quint32 _maxContractConfirmationDelay;
    };


}
}

#endif // JOYSTREAM_EXTENSION_SELLER_TORRENT_PLUGIN_CONFIGURATION_HPP

