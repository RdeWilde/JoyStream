#include "StartBuyerTorrentPlugin.hpp"

StartBuyerTorrentPlugin::StartBuyerTorrentPlugin() {
}

StartBuyerTorrentPlugin::StartBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration configuration, const UnspentP2PKHOutput & utxo)
    : _infoHash(infoHash)
    , _configuration(configuration)
    , _utxo(utxo) {
}

PluginRequestType StartBuyerTorrentPlugin::getPluginRequestType() const {
    return PluginRequestType::StartBuyerTorrentPlugin;
}

libtorrent::sha1_hash StartBuyerTorrentPlugin::infoHash() const {
    return _infoHash;
}

void StartBuyerTorrentPlugin::setInfoHash(const libtorrent::sha1_hash & infoHash) {
    _infoHash = infoHash;
}

BuyerTorrentPlugin::Configuration StartBuyerTorrentPlugin::configuration() const {
    return _configuration;
}

void StartBuyerTorrentPlugin::setConfiguration(const BuyerTorrentPlugin::Configuration & configuration) {
    _configuration = configuration;
}

UnspentP2PKHOutput StartBuyerTorrentPlugin::utxo() const {
    return _utxo;
}

void StartBuyerTorrentPlugin::setUtxo(const UnspentP2PKHOutput & utxo) {
    _utxo = utxo;
}
