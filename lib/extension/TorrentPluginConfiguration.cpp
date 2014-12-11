#include "TorrentPluginConfiguration.hpp"

qint32 TorrentPluginConfiguration::getMaxPrice() {
    return _maxPrice;
}

qint32 TorrentPluginConfiguration::getMaxBurn() {
    return _maxBurn;
}

bool TorrentPluginConfiguration::getEnableBanningSets() {
    return _enableBanningSets;
}

TorrentPluginConfiguration::TorrentPluginConfiguration(qint32 maxPrice, qint32 maxBurn, bool enableBanningSets)
    : _maxPrice(maxPrice)
    , _maxBurn(maxBurn)
    , _enableBanningSets(enableBanningSets) {

}

TorrentPluginConfiguration::TorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry) {
    // NOT IMPLEMENTED
}

TorrentPluginConfiguration::~TorrentPluginConfiguration() {

}

void TorrentPluginConfiguration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {
    // NOT IMPLEMENTED
}
