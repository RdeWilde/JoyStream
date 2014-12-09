#ifndef SELLER_TORRENT_PLUGIN_PARAMETERS_HPP
#define SELLER_TORRENT_PLUGIN_PARAMETERS_HPP

#include <QtGlobal> // for typedefs, qint32

class SellerTorrentPluginParameters
{

public:

    // The maximum number of peers to sell to simultanously
    qint32 maxNumberOfSimultaneousSales_;

    // Constructor
    SellerTorrentPluginParameters(qint32 maxNumberOfSimultaneousSales);
};

#endif // SELLER_TORRENT_PLUGIN_PARAMETERS_HPP
