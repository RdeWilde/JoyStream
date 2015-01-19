#ifndef TORRENT_PLUGIN_CONFIGURATION_HPP
#define TORRENT_PLUGIN_CONFIGURATION_HPP

#include "PluginMode.hpp"

#include <libtorrent/entry.hpp> // because you cant forward declare typedefs (libtorrent::entry::dictionary_type)

class TorrentPluginConfiguration {

public:

    // Observe constructor from members
    TorrentPluginConfiguration(bool enableBanningSets);

    // Constructor from dictionary
    TorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

    /**
     * Write configuration into dictionary
     * ===============================================================
     *
     * Torrent plugin configuration as it persists across sessions on disk
     * encoded as entry::dictionary_type with the following keys:
     *
     * IMPLEMENT LATER
     *
     */
    void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

    // Getters and setters
    bool enableBanningSets() const;

protected:

    // Whether peers are banned for bad conduct
    bool _enableBanningSets;
};

#endif // TORRENT_PLUGIN_CONFIGURATION_HPP
