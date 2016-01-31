/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#include <extension/TorrentPluginConfiguration.hpp>
#include <extension/PluginMode.hpp>

namespace joystream {
namespace extension {

    TorrentPluginConfiguration::TorrentPluginConfiguration() {
    }

    TorrentPluginConfiguration::TorrentPluginConfiguration(const TorrentPluginConfiguration & configuration)
        : _enableBanningSets(configuration.enableBanningSets()){
    }

    TorrentPluginConfiguration::TorrentPluginConfiguration(bool enableBanningSets)
        : _enableBanningSets(enableBanningSets) {
    }

    TorrentPluginConfiguration::TorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry) {

    }

    PluginMode TorrentPluginConfiguration::pluginMode(libtorrent::entry::dictionary_type & dictionaryEntry) {

        if(dictionaryEntry.count("pluginMode") == 1) {

            // Get entry
            libtorrent::entry pluginModeEntry = dictionaryEntry.find("pluginMode")->second;

            // Check that entry is of type entry::string_type
            if(pluginModeEntry.type() == libtorrent::entry::string_t) {

                // Use as entry::string_type
                libtorrent::entry::string_type pluginModeStringEntry = pluginModeEntry.string();

                if(pluginModeStringEntry.compare("Buyer") == 0)
                    return PluginMode::Buyer;
                else if(pluginModeStringEntry.compare("Seller") == 0)
                    return PluginMode::Seller;
                else if(pluginModeStringEntry.compare("Observer") == 0)
                    return PluginMode::Seller;
                //else
                //    throw InvalidBitSwaprStateEntryException(dictionaryEntry, "pluginMode value must be among {Buyer, Seller, Observer}.");

            } //else
              //  throw InvalidBitSwaprStateEntryException(dictionaryEntry, "pluginMode key is not of type entry::string_type.");

        } //else
            //throw InvalidBitSwaprStateEntryException(dictionaryEntry, "pluginMode key should have .count == 1.");
    }

    void TorrentPluginConfiguration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

    }
    bool TorrentPluginConfiguration::enableBanningSets() const {
        return _enableBanningSets;
    }

    void TorrentPluginConfiguration::setEnableBanningSets(bool enableBanningSets) {
        _enableBanningSets = enableBanningSets;
    }

}
}
