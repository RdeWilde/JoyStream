#include "BuyerTorrentPluginConfiguration.hpp"

qint32 BuyerTorrentPluginConfiguration::getMinimumNumberOfQuotes() {
    return _minimumNumberOfQuotes;
}

qint32 BuyerTorrentPluginConfiguration::getNumSimultanousPaymentChannels() {
    return _numSimultanousPaymentChannels;
}

BuyerTorrentPluginConfiguration::BuyerTorrentPluginConfiguration(qint32 maxPrice, qint32 maxBurn, bool enableBanningSets, qint32 minimumNumberOfQuotes, qint32 numSimultanousPaymentChannels)
    : TorrentPluginConfiguration(maxPrice, maxBurn, enableBanningSets)
    , _minimumNumberOfQuotes(minimumNumberOfQuotes)
    , _numSimultanousPaymentChannels(numSimultanousPaymentChannels) {

}

BuyerTorrentPluginConfiguration::BuyerTorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry)
    : TorrentPluginConfiguration(dictionaryEntry) // call super constructor
{
    // IMPLEMENT LATER
}

void BuyerTorrentPluginConfiguration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

    // Call super version
    TorrentPluginConfiguration::toDictionaryEntry(dictionaryEntry);

    // IMPLEMENT LATER
}


