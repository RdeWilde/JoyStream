#ifndef TORRENT_PLUGIN_PARAMETERS_HPP
#define TORRENT_PLUGIN_PARAMETERS_HPP

#include <QtGlobal> // quint8,...

class TorrentPluginParameters
{
public:

    // Maximum price at which this plugin buys pieces
    qint32 maxPrice_;

    // Maxmimum price at which this plugin burns funds for a payment channel
    qint32 maxBurn_;

    // Use the two sets below when accepting new peers in new_connect
    bool enableBanningSets_;

    // Constructor
    TorrentPluginParameters(qint32 maxPrice, qint32 maxBurn, bool enableBanningSets);
};

#endif // TORRENT_PLUGIN_PARAMETERS_HPP
