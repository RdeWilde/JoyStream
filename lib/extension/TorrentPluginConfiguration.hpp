#ifndef TORRENT_PLUGIN_CONFIGURATION_HPP
#define TORRENT_PLUGIN_CONFIGURATION_HPP

#include "PluginMode.hpp"

#include <libtorrent/entry.hpp> // because you cant forward declare typedefs (libtorrent::entry::dictionary_type)

#include <QtGlobal> // quint8,...

// Was factored out of TorrentConfiguration because Plugin does not need to know about all parts of torrent configuration
// only the plugin spesific ones.
class TorrentPluginConfiguration
{
public:

    // Constructor from members
    TorrentPluginConfiguration(PluginMode pluginMode, bool enableBanningSets, bool pluginOn);

    // Constructor from dictionary
    TorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

    // Getters
    bool getEnableBanningSets();
    PluginMode getPluginMode();

protected:

    // Mode of plugin. All peers have same mode.
    PluginMode _pluginMode;

    // Use the two sets below when accepting new peers in new_connect
    bool _enableBanningSets;

    // Plugin is on: Write more
    bool _pluginOn;

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
};

#endif // TORRENT_PLUGIN_CONFIGURATION_HPP
