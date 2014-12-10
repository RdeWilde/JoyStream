#include "SellerTorrentPluginConfiguration.hpp"

SellerTorrentPluginConfiguration::SellerTorrentPluginConfiguration(qint32 maxPrice, qint32 maxBurn, bool enableBanningSets, qint32 maxNumberOfSimultaneousSales)
    : TorrentPluginConfiguration(maxPrice, maxBurn, enableBanningSets)
    , _maxNumberOfSimultaneousSales(maxNumberOfSimultaneousSales) {
}

qint32 SellerTorrentPluginConfiguration::getMaxNumberOfSimultaneousSales() {
    return _maxNumberOfSimultaneousSales;
}
