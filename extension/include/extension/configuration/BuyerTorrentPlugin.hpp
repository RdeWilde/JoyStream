/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#ifndef JOYSTREAM_EXTENSION_CONFIGURATION_BUYER_TORRENT_PLUGIN_HPP
#define JOYSTREAM_EXTENSION_CONFIGURATION_BUYER_TORRENT_PLUGIN_HPP

#include <extension/configuration/TorrentPlugin.hpp>

#include <QtGlobal> // temporary due to quints

namespace joystream {
namespace extension {
namespace configuration {

    /**
     * @brief Configuration of buyer torrent plugin.
     */
    class BuyerTorrentPlugin : public TorrentPlugin {

    public:

        // Default constructor
        BuyerTorrentPlugin();

        // Constructor from members
        BuyerTorrentPlugin(bool enableBanningSets,
                      quint64 maxPrice,
                      quint32 maxLock,
                      quint64 maxFeePerKb,
                      quint32 numberOfSellers);

        // Constructor from copy <=== Why is this here again? who is using this
        //Configuration(const Configuration & c);

        // Constructor from dictionary
        BuyerTorrentPlugin(const libtorrent::entry::dictionary_type & dictionaryEntry);

        /**
         * Write configuration into dictionary
         * ===============================================================
         *
         * Buyer torrent plugin configuration as it persists across sessions on disk
         * encoded as entry::dictionary_type with the following keys:
         *
         * IMPLEMENT LATER
         *
         */
        void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

        // Getters and setters
        virtual PluginMode pluginMode() const;

        quint64 maxPrice() const;
        void setMaxPrice(quint64 maxPrice);

        quint32 maxLock() const;
        void setMaxLock(quint32 maxLock);

        quint64 maxFeePerKb() const;
        void setMaxFeePerKb(quint64 maxFeePerKb);

        quint32 numberOfSellers() const;
        void setNumberOfSellers(quint32 numberOfSellers);

    private:


        // Maximum price accepted (satoshies)
        quint64 _maxPrice;

        // Maximum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
        quint32 _maxLock;

        // Maximum fee per kB in contract transaction (satoshies)
        quint64 _maxFeePerKb;

        // Number of sellers
        quint32 _numberOfSellers;
    };

}
}
}

#endif // JOYSTREAM_EXTENSION_CONFIGURATION_BUYER_TORRENT_PLUGIN_HPP

