#ifndef TORRENT_PLUGIN_CONFIGURATION_HPP
#define TORRENT_PLUGIN_CONFIGURATION_HPP

#include <libtorrent/entry.hpp> // because you cant forward declare typedefs (libtorrent::entry::dictionary_type)

#include <QtGlobal> // quint8,...

/**
 * Configuration for running a torrent plugin. Class
 * is practically abstract (protected constructor),
 * but instances can be created through copying.
 *
 * GET RID OF LATER IF IT TURNS OUT THAT SELLER AND BUYER
 * PLUGINS REALLY HAVE NO CONFIGURATIONS IN COMMON
 */
class TorrentPluginConfiguration
{
public:

    // Getters
    qint32 getMaxPrice();
    qint32 getMaxBurn();
    bool getEnableBanningSets();

protected:

    // Maximum price at which this plugin buys pieces
    qint32 _maxPrice;

    // Maxmimum price at which this plugin burns funds for a payment channel
    qint32 _maxBurn;

    // Use the two sets below when accepting new peers in new_connect
    bool _enableBanningSets;

    // Constructor from members
    TorrentPluginConfiguration(qint32 maxPrice, qint32 maxBurn, bool enableBanningSets);

    // Constructor from dictionary
    TorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

    // Only way to make class strictly abstract, silly C++-
    // Is needed so that dyanmic_cast actually works
    virtual ~TorrentPluginConfiguration() = 0;
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
