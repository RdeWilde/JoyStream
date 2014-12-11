#include "SellerTorrentPluginConfiguration.hpp"

SellerTorrentPluginConfiguration::SellerTorrentPluginConfiguration(qint32 maxPrice, qint32 maxBurn, bool enableBanningSets, qint32 maxNumberOfSimultaneousSales)
    : TorrentPluginConfiguration(maxPrice, maxBurn, enableBanningSets)
    , _maxNumberOfSimultaneousSales(maxNumberOfSimultaneousSales) {
}

SellerTorrentPluginConfiguration::SellerTorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry)
    : TorrentPluginConfiguration(dictionaryEntry) // call super constructor
{
    // IMPLEMENT LATER
}

void SellerTorrentPluginConfiguration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

    // Call super version
    TorrentPluginConfiguration::toDictionaryEntry(dictionaryEntry);

    // IMPLEMENT LATER
}

qint32 SellerTorrentPluginConfiguration::getMaxNumberOfSimultaneousSales() {
    return _maxNumberOfSimultaneousSales;
}
