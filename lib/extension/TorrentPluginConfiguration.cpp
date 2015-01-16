#include "TorrentPluginConfiguration.hpp"
#include "PeerPluginConfiguration.hpp"

TorrentPluginConfiguration::TorrentPluginConfiguration(bool enableBanningSets)
    : _pluginMode(PluginMode::Observe)
    , _enableBanningSets(enableBanningSets) {
}

TorrentPluginConfiguration::TorrentPluginConfiguration(bool enableBanningSets,
                                                       quint32 minPrice,
                                                       QTime minLock,
                                                       quint32 minFee,
                                                       QTime maxContractConfirmationDelay)
    : _pluginMode(PluginMode::Sell)
    , _enableBanningSets(enableBanningSets)
    , _minPrice(minPrice)
    , _minLock(minLock)
    , _minFee(minFee)
    , _maxContractConfirmationDelay(maxContractConfirmationDelay) {
}

TorrentPluginConfiguration::TorrentPluginConfiguration(bool enableBanningSets,
                                                       quint32 maxPrice,
                                                       QTime maxLock,
                                                       QTime waitTime,
                                                       qint8 minPeers,
                                                       quint32 fee)
    : _pluginMode(PluginMode::Buy)
    , _enableBanningSets(enableBanningSets)
    , _maxPrice(maxPrice)
    , _maxLock(maxLock)
    , _joinContractDelay(waitTime)
    , _numSellers(minPeers)
    , _fee(fee) {
}

TorrentPluginConfiguration::TorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry) {
    // NOT IMPLEMENTED
}

/*
void TorrentPluginConfiguration::insertPeerPluginConfiguration(const PeerPluginConfiguration * peerPluginConfiguration) {
    _peerPluginConfigurations.insert(std::make(peerPluginConfiguration->peerPluginConfiguration));
}

const std::map<libtorrent::tcp::endpoint, PeerPluginConfiguration *>::const_iterator TorrentPluginConfiguration::getBeginPeerPluginConfigurationsIterator() const {
    return _peerPluginConfigurations.begin();
}

const std::map<libtorrent::tcp::endpoint, PeerPluginConfiguration *>::const_iterator TorrentPluginConfiguration::getEndPeerPluginConfigurationsIterator() const {
    return _peerPluginConfigurations.end();
}
*/

void TorrentPluginConfiguration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {
    // NOT IMPLEMENTED
}

PluginMode TorrentPluginConfiguration::pluginMode() const {
    return _pluginMode;
}

bool TorrentPluginConfiguration::enableBanningSets() const {
    return _enableBanningSets;
}
