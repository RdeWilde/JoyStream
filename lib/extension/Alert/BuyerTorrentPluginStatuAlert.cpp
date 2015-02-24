#include "BuyerTorrentPluginStatuAlert.hpp"


BuyerTorrentPluginStatusAlert::BuyerTorrentPluginStatusAlert(const libtorrent::sha1_hash & infoHash, BuyerTorrentPlugin::Status status)
    : TorrentPluginAlert(infoHash)
    , _status(status){
}

BuyerTorrentPluginStatusAlert::BuyerTorrentPluginStatusAlert(const BuyerTorrentPluginStatusAlert & alert)
    : TorrentPluginAlert(alert.infoHash())
    , _status(alert.status()){
}

int BuyerTorrentPluginStatusAlert::type() const {
    return alert_type;
}

char const * BuyerTorrentPluginStatusAlert::what() const {
    return "BuyerTorrentPluginStatusAlert";
}

std::string BuyerTorrentPluginStatusAlert::message() const {
    return std::string("BuyerTorrentPluginStatusAlert::message: IMPLEMENT LATER");
}

int BuyerTorrentPluginStatusAlert::category() const {
    return libtorrent::alert::stats_notification;
}

std::auto_ptr<libtorrent::alert> BuyerTorrentPluginStatusAlert::clone() const {
    return std::auto_ptr<alert>(new BuyerTorrentPluginStatusAlert(*this));
}

BuyerTorrentPlugin::Status BuyerTorrentPluginStatusAlert::status() const {
    return _status;
}

void BuyerTorrentPluginStatusAlert::setStatus(const BuyerTorrentPlugin::Status &status) {
    _status = status;
}
