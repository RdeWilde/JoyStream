#include "BuyerTorrentPluginConfiguration.hpp"

BuyerTorrentPluginConfiguration::BuyerTorrentPluginConfiguration(qint32 maxPrice, qint32 maxBurn, bool enableBanningSets, qint32 minimumNumberOfQuotes, qint32 numSimultanousPaymentChannels)
    : TorrentPluginConfiguration(maxPrice, maxBurn, enableBanningSets)
    , _minimumNumberOfQuotes(minimumNumberOfQuotes)
    , _numSimultanousPaymentChannels(numSimultanousPaymentChannels){

}

qint32 BuyerTorrentPluginConfiguration::getMinimumNumberOfQuotes() {
    return _minimumNumberOfQuotes;
}

qint32 BuyerTorrentPluginConfiguration::getNumSimultanousPaymentChannels() {
    return _numSimultanousPaymentChannels;
}
