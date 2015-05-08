#include "StartedBuyerTorrentPlugin.hpp"

StartedBuyerTorrentPlugin::StartedBuyerTorrentPlugin() {
}

StartedBuyerTorrentPlugin::StartedBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration & configuration)
    : _infoHash(infoHash)
    , _configuration(configuration) {
}

StartedBuyerTorrentPlugin::StartedBuyerTorrentPlugin(const StartedBuyerTorrentPlugin & alert)
    : _infoHash(alert.infoHash())
    , _configuration(alert.configuration()) {
}

int StartedBuyerTorrentPlugin::type() const {
    return alert_type;
}

char const* StartedBuyerTorrentPlugin::what() const {
    return "StartedBuyerTorrentPlugin";
}

std::string StartedBuyerTorrentPlugin::message() const {
    return std::string("StartedBuyerTorrentPlugin::message: IMPLEMENT LATER");
}

int StartedBuyerTorrentPlugin::category() const {
    return libtorrent::alert::status_notification;
}

std::auto_ptr<libtorrent::alert> StartedBuyerTorrentPlugin::clone() const {
    return std::auto_ptr<libtorrent::alert>(new StartedBuyerTorrentPlugin(*this));
}

libtorrent::sha1_hash StartedBuyerTorrentPlugin::infoHash() const {
    return _infoHash;
}

void StartedBuyerTorrentPlugin::setInfoHash(const libtorrent::sha1_hash & infoHash) {
    _infoHash = infoHash;
}

BuyerTorrentPlugin::Configuration StartedBuyerTorrentPlugin::configuration() const {
    return _configuration;
}

void StartedBuyerTorrentPlugin::setConfiguration(const BuyerTorrentPlugin::Configuration & configuration) {
    _configuration = configuration;
}
