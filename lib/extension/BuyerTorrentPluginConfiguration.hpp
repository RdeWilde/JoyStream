#ifndef BUYER_TORRENT_PLUGIN_CONFIGURATION_HPP
#define BUYER_TORRENT_PLUGIN_CONFIGURATION_HPP

#include "TorrentPluginConfiguration.hpp"

class BuyerTorrentPluginConfiguration : public TorrentPluginConfiguration {

public:

    // Constructor from members
    BuyerTorrentPluginConfiguration(qint32 maxPrice, qint32 maxBurn, bool enableBanningSets, qint32 minimumNumberOfQuotes, qint32 numSimultanousPaymentChannels);

    // Constructor from dictionary
    BuyerTorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

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

    // Getters
    qint32 getMinimumNumberOfQuotes();
    qint32 getNumSimultanousPaymentChannels();

private:

    // How many price quotes from seller peers must be gathered before the best one is picked
    qint32 _minimumNumberOfQuotes;

    // How many payment channels should be attempted established simultaneously
    qint32 _numSimultanousPaymentChannels;

};

#endif // BUYER_TORRENT_PLUGIN_CONFIGURATION_HPP
