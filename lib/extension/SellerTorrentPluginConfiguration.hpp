#ifndef SELLER_TORRENT_PLUGIN_CONFIGURATION_HPP
#define SELLER_TORRENT_PLUGIN_CONFIGURATION_HPP

#include "TorrentPluginConfiguration.hpp"

class SellerTorrentPluginConfiguration : public TorrentPluginConfiguration
{

public:

    // Constructor
    SellerTorrentPluginConfiguration(qint32 maxPrice, qint32 maxBurn, bool enableBanningSets, qint32 maxNumberOfSimultaneousSales);

    // Getters
    qint32 getMaxNumberOfSimultaneousSales();

private:

    // The maximum number of peers to sell to simultanously
    qint32 _maxNumberOfSimultaneousSales;
};

#endif // SELLER_TORRENT_PLUGIN_CONFIGURATION_HPP
