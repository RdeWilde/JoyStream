#ifndef SELLER_TORRENT_PLUGIN_CONFIGURATION_HPP
#define SELLER_TORRENT_PLUGIN_CONFIGURATION_HPP

#include "TorrentPluginConfiguration.hpp"

class SellerTorrentPluginConfiguration : public TorrentPluginConfiguration
{

public:

    // Constructor from members
    SellerTorrentPluginConfiguration(qint32 maxPrice, qint32 maxBurn, bool enableBanningSets, qint32 maxNumberOfSimultaneousSales);

    // Constructor from dictionary
    SellerTorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

    /**
     * Write configuration into dictionary
     * ===============================================================
     *
     * Seller torrent plugin configuration as it persists across sessions on disk
     * encoded as entry::dictionary_type with the following keys:
     *
     * IMPLEMENT LATER
     *
     */
    void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

    // Getters
    qint32 getMaxNumberOfSimultaneousSales();

private:

    // The maximum number of peers to sell to simultanously
    qint32 _maxNumberOfSimultaneousSales;
};

#endif // SELLER_TORRENT_PLUGIN_CONFIGURATION_HPP
