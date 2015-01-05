#include "TorrentPluginConfiguration.hpp"
#include "PeerPluginConfiguration.hpp"

TorrentPluginConfiguration::TorrentPluginConfiguration(bool enableBanningSets)
    : _pluginMode(PluginMode::Observe)
    , _enableBanningSets(enableBanningSets) {
}

TorrentPluginConfiguration::TorrentPluginConfiguration(bool enableBanningSets, quint32 sellerPrice)
    : _pluginMode(PluginMode::Sell)
    , _enableBanningSets(enableBanningSets)
    , _sellerPrice(sellerPrice) {
}

TorrentPluginConfiguration::TorrentPluginConfiguration(bool enableBanningSets, quint32 buyerPrice, quint32 fee, qint32 btcVersion, qint16 waitTime, qint8 minPeers)
    : _pluginMode(PluginMode::Buy)
    , _enableBanningSets(enableBanningSets)
    , _buyerPrice(buyerPrice)
    , _fee(fee)
    , _btcVersion(btcVersion)
    , _waitTime(waitTime)
    , _minPeers(minPeers) {
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
