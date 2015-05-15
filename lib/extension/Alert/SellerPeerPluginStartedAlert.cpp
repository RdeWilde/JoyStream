#include "SellerPeerPluginStartedAlert.hpp"

SellerPeerPluginStartedAlert::SellerPeerPluginStartedAlert(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration)
    : TorrentPluginAlert(infoHash)
    , _configuration(configuration) {
}

SellerPeerPluginStartedAlert::SellerPeerPluginStartedAlert(const SellerPeerPluginStartedAlert & alert)
    : TorrentPluginAlert(alert)
    , _configuration(alert.configuration()) {
}

int SellerPeerPluginStartedAlert::type() const {
    return alert_type;
}

char const* SellerPeerPluginStartedAlert::what() const {
    return "SellerPeerPluginStartedAlert";
}

std::string SellerPeerPluginStartedAlert::message() const {
    return std::string("SellerPeerPluginStartedAlert::message: IMPLEMENT LATER");
}

int SellerPeerPluginStartedAlert::category() const {
    return libtorrent::alert::stats_notification;
}

std::auto_ptr<libtorrent::alert> SellerPeerPluginStartedAlert::clone() const {
    return std::auto_ptr<alert>(new SellerPeerPluginStartedAlert(*this));
}
SellerTorrentPlugin::Configuration SellerPeerPluginStartedAlert::configuration() const {
    return _configuration;
}

void SellerPeerPluginStartedAlert::setConfiguration(const SellerTorrentPlugin::Configuration & configuration) {
    _configuration = configuration;
}
