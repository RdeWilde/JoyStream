#include "TorrentPluginConfiguration.hpp"

TorrentPluginConfiguration::TorrentPluginConfiguration(qint32 maxPrice, qint32 maxBurn, bool enableBanningSets)
    : _maxPrice(maxPrice)
    , _maxBurn(maxBurn)
    , _enableBanningSets(enableBanningSets)
{}

qint32 TorrentPluginConfiguration::getMaxPrice() {
    return _maxPrice;
}

qint32 TorrentPluginConfiguration::getMaxBurn() {
    return _maxBurn;
}

bool TorrentPluginConfiguration::getEnableBanningSets() {
    return _enableBanningSets;
}
