#ifndef BUYER_TORRENT_PLUGIN_PARAMETERS_HPP
#define BUYER_TORRENT_PLUGIN_PARAMETERS_HPP

#include <QtGlobal> // typedef types

class BuyerTorrentPluginParameters
{
public:

    // How many price quotes from seller peers must be gathered before the best one is picked
    qint32 minimumNumberOfQuotes_;

    // How many payment channels should be attempted established simultaneously
    qint32 numSimultanousPaymentChannels_;

    // Constructor
    BuyerTorrentPluginParameters(qint32 minimumNumberOfQuotes, qint32 numSimultanousPaymentChannels);
};

#endif // BUYER_TORRENT_PLUGIN_PARAMETERS_HPP
