/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#include <extension/configuration/TorrentPlugin.hpp>

#include <extension/PluginMode.hpp>

namespace joystream {
namespace extension {
namespace configuration {

    TorrentPlugin::TorrentPlugin() {
    }

    TorrentPlugin::TorrentPlugin(const TorrentPlugin & configuration)
        : _enableBanningSets(configuration.enableBanningSets()){
    }

    TorrentPlugin::TorrentPlugin(bool enableBanningSets)
        : _enableBanningSets(enableBanningSets) {
    }

    TorrentPlugin::TorrentPlugin(const libtorrent::entry::dictionary_type & dictionaryEntry) {

    }

    PluginMode TorrentPlugin::pluginMode(libtorrent::entry::dictionary_type & dictionaryEntry) {

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
                else
                    throw InvalidBitSwaprStateEntryException(dictionaryEntry, "pluginMode value must be among {Buyer, Seller, Observer}.");

            } //else
              //  throw InvalidBitSwaprStateEntryException(dictionaryEntry, "pluginMode key is not of type entry::string_type.");

        } //else
            //throw InvalidBitSwaprStateEntryException(dictionaryEntry, "pluginMode key should have .count == 1.");
    }

    void TorrentPlugin::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

    }
    bool TorrentPlugin::enableBanningSets() const {
        return _enableBanningSets;
    }

    void TorrentPlugin::setEnableBanningSets(bool enableBanningSets) {
        _enableBanningSets = enableBanningSets;
    }


}
}
}
