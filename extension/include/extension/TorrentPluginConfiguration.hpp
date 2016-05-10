/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#ifndef JOYSTREAM_EXTENSION_TORRENT_PLUGIN_CONFIGURATION_HPP
#define JOYSTREAM_EXTENSION_TORRENT_PLUGIN_CONFIGURATION_HPP

#include <libtorrent/entry.hpp> // can't forward declare libtorrent::entry::dictionary_type

namespace joystream {
namespace extension {

    enum class PluginMode;

    class TorrentPluginConfiguration {

    public:

        // Default constructor
        TorrentPluginConfiguration();

        // Copy constructor
        TorrentPluginConfiguration(const TorrentPluginConfiguration & configuration);

        // Constructor from members
        TorrentPluginConfiguration(bool enableBanningSets);

        // Constructor from dictionary
        TorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

        // Determnes the plugin mode of TorrentPlugin::Configuration object encoded in dictionary
        static PluginMode pluginMode(libtorrent::entry::dictionary_type & dictionaryEntry);

        // Turn into dictionary
        void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

        // Getters and setters
        virtual PluginMode pluginMode() const = 0;

        bool enableBanningSets() const;
        void setEnableBanningSets(bool enableBanningSets);

    protected:

        // Whether peers are banned for bad conduct
        bool _enableBanningSets;
    };

}
}

#endif // JOYSTREAM_EXTENSION_TORRENT_PLUGIN_CONFIGURATION_HPP

