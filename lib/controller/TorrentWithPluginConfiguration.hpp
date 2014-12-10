#ifndef TORRENT_WITH_PLUGIN_CONFIGURATION_HPP
#define TORRENT_WITH_PLUGIN_CONFIGURATION_HPP

#include "TorrentConfiguration.hpp"
#include "extension/TorrentPluginConfiguration.hpp"

/**
 * Configurations in common for torrents with seller and buyer
 * plugins. Is effectively abstract due to protected constructor,
 * but may still be instantiated through copying.
 *
 * May be removed in the future if the two forms of plugin
 * enabled torrents really dont have configurations in common.
 */
class TorrentWithPluginConfiguration : public TorrentConfiguration
{

public:

    // Pure virtual routine where subclasses must return portion of their torrent plugin configuration type
    const TorrentPluginConfiguration & getTorrentPluginConfiguration() = virtual;

protected:

    // Constructor from fields
    TorrentConfiguration(const libtorrent::sha1_hash & info_hash, const std::string & name, const std::string & save_path, const std::vector<char> & resume_data, quint64 flags, qint32 maxPrice, qint32 maxBurn, bool enableBanningSets);

    // Constructor from dictionary
    TorrentConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);
};

#endif // TORRENT_WITH_PLUGIN_CONFIGURATION_HPP
