#ifndef TORRENT_PLUGIN_CONFIGURATION_HPP
#define TORRENT_PLUGIN_CONFIGURATION_HPP

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

    // Constructor
    TorrentPluginConfiguration(qint32 maxPrice, qint32 maxBurn, bool enableBanningSets);
};

#endif // TORRENT_PLUGIN_CONFIGURATION_HPP
